// Partition NEW!!!! Caso cluster e mining condition.
// VEDI newPartition.lyx o newPartition.ps


#include <iostream>
#include <fstream>

#include <stdlib.h>

#include <stdio.h>
#include <math.h>

#include <string>
#include <vector>


#include "Algorithms/itemSet.h"
#include "Algorithms/itemSetList.h"

#include "Algorithms/newItemSetHashMap.h"
#include "Algorithms/sqlCoreConn.h"

#include "Algorithms/AlgorithmsOptions.h"




using namespace std;
using namespace minerule;



bool
readSourceTable(
		itemSet<ItemType>& kItem,
		odbc::ResultSet* result,
		const minerule::HeadBodySourceRowDescription& rowDes,
		const minerule::AlgorithmsOptions& options,
		bool conditionalInsert,
		int& gid,
		double& totGroups
		) {
  //  cout << endl 
  //       <<"LEGGO IL DB ED INSERISCO NELLE STRUTTURE DATI... (" << conditionalInsert << ")" << endl
  //       <<"Rows per partitions (at least):" << options.getPartitionOptions().getRowsPerPartition() << endl;
  /*****************
   * result contiene TUTTO il DB. L'algoritmo lavora partizionando
   * il result set. 
   * vincoli: 
   *  1) Una partizione deve potere stare in memoria, percio' si
   *     ammette un max di "allowedRowRead" letture all'interno di
   *     un ciclo
   *  2) La lettura di una partizione non puo' terminare prima di avere
   *     finito di leggere il gruppo corrente.
   *  3) possiamo usare solo result->next() per muoverci nel result set
   *     percio' ci accorgiamo che un gruppo e' finito solo dopo avere
   *     letto la prima riga del gruppo successivo
   * soluzione:
   *  entrando nel ciclo si suppone di avere gia' letto la prima riga
   *  del gruppo corrente, come conseguenza possiamo uscire dal ciclo
   *  dopo avere letto la prima riga del gruppo successivo, cosicche'
   *  all'iterazione successiva possiamo ricominciare tranquillamente
   *  da dove lasciato.
   */

  ItemType itemBody;
  ItemType gidB, gidOld;
  //static int gid = 1;

  bool readMoreGroupsForThisPartition = true;
  unsigned int  rowCount = 0;
  bool moreRecords = true;


  // n.b. il seguente parametro deve esserci dato dall'esterno...
  unsigned int allowedRowRead = options.getMiningAlgorithmsOptions().getRulesMiningAlgorithms().getPartitionBase().getRowsPerPartition();

  // Creo la struttura del livello 1 leggendo le tuple
  do {
    rowCount++;

    HeadBodySourceRow curRow(result, rowDes);
    
     
    gidB = curRow.getGroupBody();
    itemBody = curRow.getBody();
    
    // La prima volta che entro gidOld non e' inizializzato...
    // percio': lo inizializzo! In questo modo la prima volta
    // la riga e' comunque inserita in kItem
    if( rowCount == 1 )
      gidOld=gidB;


    // Qui testiamo se e' il caso di uscire dal ciclo (il che succede
    // quando gidOld!=gidB e rowCount>=alowedRowRead
    if (gidOld!=gidB) {
      gidOld=gidB;
      gid++;
	 
      // Continuiamo a leggere solo se non abbiamo gia' letto "allowedRowRead"
      // righe	   

      if(rowCount>=allowedRowRead ) {
	readMoreGroupsForThisPartition= false;
	//	cout << "NON INSERISCO" << gidB.c_str() << " - " << itemBody.c_str() << endl;
      } else {
	totGroups++;
	//	cout << "Inserisco:" << gidB.c_str() << " - " << itemBody.c_str() << endl;
        if(!conditionalInsert)
	  //kItem.setItemSet(itemBody,gidB);
	  kItem.setItemSet(itemBody,gid);
	else
	  //kItem.setItemSetIfExist(itemBody,gidB);
	  kItem.setItemSetIfExist(itemBody,gid);

	readMoreGroupsForThisPartition= true;
      }

    } else {
      // Inserisco a livello 1 la bid con liste e nella head hid con liste
      //      cout << "Inserisco:" << gidB.c_str() << " - " << itemBody.c_str() << endl;
      if(!conditionalInsert)
	//kItem.setItemSet(itemBody,gidB);
	kItem.setItemSet(itemBody,gid);
      else
	//kItem.setItemSetIfExist(itemBody,gidB);
	kItem.setItemSetIfExist(itemBody,gid);
    }

    // La lettura la facciamo solo se abbiamo inserito l'elemento corrente
    // nella struttura. Questo avviene ogni qual volta non si deve uscire
    // dal ciclo.
    if( readMoreGroupsForThisPartition ) {
      moreRecords = result->next();
    }

#if 1 // DEBUG
    if(rowCount % (allowedRowRead/10)==0) {
      cout << "rowcount:" << rowCount << "\r";
      cout.flush();
    }
#endif
    
  } while(readMoreGroupsForThisPartition && moreRecords  );

  //  cout << endl << "final row count:" << rowCount << endl;
   
  return moreRecords;
}





// Qui definisco il tipo TYPENODE. Tutte le classi sono dichiarate come <String>

void partitionWithoutClusters( const minerule::AlgorithmsOptions& options  )
{
  odbc::Connection* connection = options.getConnection();
  odbc::PreparedStatement* statement = options.getStatement();
  const minerule::HeadBodySourceRowDescription& 
    rowDes = options.getSourceRowDescription();


  // Variabile che contiene la struttura principale
  itemSet<ItemType> kItem;
  // Variabile con i puntatori alla lista dei livelli
  itemSetList<ItemType> prtList;
  // HashMap che simula il lattice. Usata per fare apriori
  newItemSetHashMap <ItemType> pHashMap;

  // srDescriptor allows one to obtain SQL definitions of 
  // source row elements in result.

  odbc::ResultSet* result; // result set used to read the source db

  int isPart,levelIn;
  double support,conf,
    totGroups; // Notice, before version 1.0.3d this variable was used
               // in order to evaluate the support threshold. In the code
               // it is setted to the number of groups which contain at least
               // one line which satisfy the constraints. Nowaday the support
               // is evaluated as a function of the number of groups that
               // form the unfiltered source table. We still compute the old
               // value for totGroups, but we do not use it (it may become handy
               // in future versions).
  double nSup;
  bool toContinue;
  // Connessione e ResultSet Vedi classe sqlCoreConn.h e cpp
  sqlCoreConn coreConn;


// parametri da passare.
  support= options.getSupport()*100;
  conf=    options.getConfidence()*100;
  //  cout <<endl<<"+ Supporto: "<<support<<"% Confidenza: "<< conf<<"%"<<endl;

  // Apertura della connessione e cancellazione db_temporanei
  coreConn.useConnection(connection);
  coreConn.setOutTableName(options.getOutTableName());
  coreConn.setBodyCardinalities(options.getBodyCardinalities());
  coreConn.setHeadCardinalities(options.getHeadCardinalities());


  // Ciclo fino alla fine delle partizioni

  isPart = 0;
  result = statement->executeQuery();

  bool readMorePartitions=result->next();
  SourceRowDescriptor srDescriptor(result, rowDes);

  coreConn.create_tmp_db(0, srDescriptor.getBody(), srDescriptor.getHead());

  MRLogPush("Reading the database (first phase)...");
  int gid = 1;
  while( readMorePartitions )
  {

    isPart++;

    totGroups=1;
    // Svuoto la lista dei livelli.
    prtList.init();
    
    MRLog() << "  Reading partition number:" << isPart
		     << endl;
    readMorePartitions = readSourceTable(kItem,result,rowDes,options,false,gid,totGroups);
    MRLog() << "  Done! (Reading partition number:" << isPart <<")"
		     << endl;
    
    nSup=ceil(totGroups*(support/100));

    MRLog() << "  Generating Large Itemset of current partition"
		     << endl;
    kItem.gen_Large_ItemSet_Base(nSup);
    
    levelIn=1;
    toContinue=true;
    while(toContinue)
      {
	levelIn=levelIn+1;
	toContinue=kItem.gen_Large_ItemSet(prtList,
					   nSup,
					   levelIn);
      }

    MRLog() << "  Saving large itmeset of current partition" << endl;
    kItem.save_Large_ItemSet(1,isPart,coreConn);
  
    // Svuoto la lista dei livelli per il ciclo successivo.
    prtList.removeAll();  
  
  } // fine WHILE

  MRLogPop(); //Done! (Reading database - first phase)
  
  delete result;   // trashing the trashable 

 prtList.init();

 MRLog() << "Merge phase" << endl;
 kItem.mergeItemSet(coreConn,prtList, srDescriptor);
 totGroups=0;

 result = statement->executeQuery();
 readMorePartitions = result->next();


 MRLogPush("Reading database (second phase)...");
 gid = 1;
 while( readMorePartitions ) {
   MRLog() << "  Reading partition number:" << isPart
		    << endl;

   double tmpTotGroups=1;
   readMorePartitions = readSourceTable(kItem,result,rowDes,options,true,gid,tmpTotGroups);
   totGroups+=tmpTotGroups;

   MRLog() << "  Done! (Reading partition number:" << isPart <<")"
		    << endl;
 }
 MRLogPop();// << "Done (reading database - second phase)"<<endl;


 delete result;

 // Old nSup definition: nSup=ceil(totGroups*(support/100));
 nSup=ceil(options.getTotGroups()*support/100);
 MRDebug() << "Support threshold:" << nSup << endl;

 MRLog() << "Pruning..." << endl;
 kItem.checkSupportBaseDef(nSup,pHashMap,prtList);

 toContinue=true;
 levelIn=1;

 while ( (toContinue))  {
     levelIn=levelIn+1;
     toContinue=kItem.gen_final_count(prtList,nSup,levelIn,pHashMap);
 }

 coreConn.deleteDestTable();
 coreConn.create_db_rule(0);
 
 MRLogPush("Extracting rules...");
 kItem.extractRule(coreConn, options.getTotGroups(), kItem);
 MRLogPop();
}
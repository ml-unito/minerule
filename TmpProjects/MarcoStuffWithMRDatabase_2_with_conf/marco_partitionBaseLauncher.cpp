// Partition NEW!!!! Caso cluster e mining condition.
// VEDI newPartition.lyx o newPartition.ps


#include <iostream>
#include <fstream>

#include <stdlib.h>

#include <stdio.h>
#include <math.h>

#include <string>
#include <vector>


#include "itemSet.h"
#include "itemSetList.h"

#include "newItemSetHashMap.h"
#include "Database/Connection.h"

#include "AlgorithmsOptions.h"




using namespace std;
using namespace minerule;



bool
readSourceTable(
		itemSet<ItemType>& kItem,
		const minerule::HeadBodySourceRowDescription& rowDes,
		const minerule::AlgorithmsOptions& options,
		bool conditionalInsert,
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

  bool readMoreGroupsForThisPartition = true;
  unsigned int  rowCount = 0;
  bool moreRecords = true;


  // n.b. il seguente parametro deve esserci dato dall'esterno...
  unsigned int allowedRowRead = options.getPartitionOptions().getRowsPerPartition();

  // Creo la struttura del livello 1 leggendo le tuple
  do {
    rowCount++;
	 odbc::ResultSet* result=options.getMRIterator().getResultSet();

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
	 
      // Continuiamo a leggere solo se non abbiamo gia' letto "allowedRowRead"
      // righe	   

      if(rowCount>=allowedRowRead ) {
	readMoreGroupsForThisPartition= false;
	//	std::cout << "NON INSERISCO" << gidB.c_str() << " - " << itemBody.c_str() << endl;
      } else {
	totGroups++;
	//	std::cout << "Inserisco:" << gidB.c_str() << " - " << itemBody.c_str() << endl;
        if(!conditionalInsert)
	  kItem.setItemSet(itemBody,gidB);
	else
	  kItem.setItemSetIfExist(itemBody,gidB);

	readMoreGroupsForThisPartition= true;
      }

    } else {
      // Inserisco a livello 1 la bid con liste e nella head hid con liste
      //      cout << "Inserisco:" << gidB.c_str() << " - " << itemBody.c_str() << endl;
      if(!conditionalInsert)
	kItem.setItemSet(itemBody,gidB);
      else
	kItem.setItemSetIfExist(itemBody,gidB);
    }

    // La lettura la facciamo solo se abbiamo inserito l'elemento corrente
    // nella struttura. Questo avviene ogni qual volta non si deve uscire
    // dal ciclo.
    if( readMoreGroupsForThisPartition ) {
      moreRecords = options.getMRIterator().next();
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
  odbc::Connection* connection = options.getODBCConnection();
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

  int isPart,levelIn;
  double support,conf,totGroups;
  double nSup;
  bool toContinue;
  // Connessione e ResultSet Vedi classe Database/Connection.h e cpp
  Connection connection;


// parametri da passare.
  support= options.getSupport()*100;
  conf=    options.getConfidence()*100;
  //  cout <<std::endl<<"+ Supporto: "<<support<<"% Confidenza: "<< conf<<"%"<<std::endl;

  // Apertura della connessione e cancellazione db_temporanei
  connection.useODBCConnection(connection);
  connection.setOutTableName(options.getOutTableName());
  connection.setAlgorithmOptions(options);


  // Ciclo fino alla fine delle partizioni

  isPart = 0;
  MRResultSetIterator* result = &options.getMRIterator();
  //  result = statement->executeQuery();

  int logId = MRLogGetNewID();
  int logPartId = MRLogGetNewID();
  bool readMorePartitions=result->next();
  if (readMorePartitions) {
  SourceRowDescriptor srDescriptor(result->getResultSet(), rowDes);

  connection.create_tmp_db(0, srDescriptor.getBody(), srDescriptor.getHead());


  MRLog(logId)<< "Reading the database (first phase)..." << endl;
  while( readMorePartitions )
  {

    isPart++;

    totGroups=1;
    // Svuoto la lista dei livelli.
    prtList.init();
    
    MRLog(logPartId) << "  Reading partition number:" << isPart
		     << endl;
    readMorePartitions = readSourceTable(kItem,rowDes,options,false,totGroups);
    MRLog(logPartId) << "  Done! (Reading partition number:" << isPart <<")"
		     << endl;
    
    nSup=ceil(totGroups*(support/100));

    MRLog(logPartId) << "  Generating Large Itemset of current partition"
		     << endl;
    kItem.gen_Large_ItemSet_Base(nSup);
    
    levelIn=1;
    toContinue=(nSup>0);
    while(toContinue)
      {
	levelIn=levelIn+1;
	toContinue=kItem.gen_Large_ItemSet(prtList,
					   nSup,
					   levelIn);
      }

    MRLog(logPartId) << "  Done! (Generating Large Itemsets...)"
		     << endl;

    MRLog(logPartId) << "  Saving large itmeset of current partition" << endl;
    kItem.save_Large_ItemSet(1,isPart,connection);
    MRLog(logPartId) << "  Done! (Saving large itmeset...)" << endl;
    
  
    // Svuoto la lista dei livelli per il ciclo successivo.
    prtList.removeAll();  
  
  } // fine WHILE

  MRLog(logId) << "Done! (Reading database - first phase)" << endl;
  // delete result;   // trashing the trashable 

 prtList.init();

 MRLog(logId) << "Merge phase" << endl;
 kItem.mergeItemSet(connection,prtList, srDescriptor);
 MRLog(logId) << "Done! (Merge phase)" << endl;
  } 
 totGroups=0;

 result->reset();
 readMorePartitions = result->next();


 isPart = 1;
 MRLog(logId) << "Reading database (second phase)..." << endl;
 while( readMorePartitions ) {
   MRLog(logPartId) << "  Reading partition number:" << isPart
		    << endl;

   double tmpTotGroups=1;
   readMorePartitions = readSourceTable(kItem,rowDes,options,true,tmpTotGroups);
   totGroups+=tmpTotGroups;

   MRLog(logPartId) << "  Done! (Reading partition number:" << isPart <<")"
		    << endl;
	isPart++;
 }
 MRLog(logId) << "Done (reading database - second phase)"<<std::endl;


 // delete result;

 nSup=ceil(totGroups*(support/100));

 MRLog(logId) << "Pruning..." << endl;
 kItem.checkSupportBaseDef(nSup,pHashMap,prtList);

 toContinue=(nSup>0);
 levelIn=1;

 while ( (toContinue))  {
     levelIn=levelIn+1;
     toContinue=kItem.gen_final_count(prtList,nSup,levelIn,pHashMap);
 }
 MRLog(logId) << "Done! (Pruning)" << endl;

 connection.deleteDestTables();
 connection.createResultTables();
 
 MRLog(logId) << "Extracting rules..." << endl;
 kItem.extractRule(connection, totGroups, kItem);
 MRLog(logId) << "Done! (extracting rules)"<<std::endl;
}

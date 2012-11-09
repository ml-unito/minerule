// Partition NEW!!!! Caso cluster e mining condition.
// VEDI newPartition.lyx o newPartition.ps


#include <iostream>
#include <fstream>

#include <stdlib.h>

#include <stdio.h>
#include <math.h>

#include <string>
#include <vector>
#include <stdexcept>

//#include "utils.h"



#include "Algorithms/itemSetCluster.h"
#include "Algorithms/itemSetListCluster.h"

#include "Algorithms/newItemSetHashMap.h"
#include "Database/Connection.h"
#include "Algorithms/AlgorithmsOptions.h"

#include "Utils/MinMaxPair.h"

using namespace minerule;


bool
readSourceTable(
		itemSetCluster <ItemType>& kItem,
		odbc::ResultSet* result,
		const minerule::HeadBodySourceRowDescription& rowDes,
		const minerule::AlgorithmsOptions& options,
		bool conditionalInsert,
		int& gid,
		double& totGroups
		) {
  //  std::cout << std::endl 
  //       <<"LEGGO IL DB ED INSERISCO NELLE STRUTTURE DATI... (" << conditionalInsert << ")" << std::endl
  //       <<"Rows per partitions (at least):" << options.getPartitionOptions().getRowsPerPartition() << std::endl;
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

  ItemType itemBody,itemHead;
  ItemType gidB, cidB, cidH,gidOld;
//  static int gid = 1;
  int cidBody = 1, cidHead = 1;

  bool readMoreGroupsForThisPartition = true;
  unsigned int  rowCount = 0;
  bool moreRecords = true;

  // n.b. il seguente parametro deve esserci dato dall'esterno...
  unsigned int allowedRowRead = options.getMiningAlgorithmsOptions().getRulesMiningAlgorithms().getPartitionWithClusters().getRowsPerPartition();

  // Creo la struttura del livello 1 leggendo le tuple
  do {
    rowCount++;

    HeadBodySourceRow curRow(result, rowDes);
   
     
    gidB = curRow.getGroupBody();
    cidB = curRow.getClusterBody();
    itemBody = curRow.getBody();
    cidH = curRow.getClusterHead();
    itemHead = curRow.getHead();

    // La prima volta che entro gidOld non e' inizializzato...
    // percio': lo inizializzo! In questo modo la prima volta
    // la riga e' comunque inserita in kItem
    if( rowCount == 1 )
      gidOld=gidB;


    // Qui testiamo se e' il caso di uscire dal ciclo (il che succede
    // quando gidOld!=gidB e rowCount>=allowedRowRead
    if (gidOld!=gidB) {
      gidOld=gidB;
      gid++; 
	 
      // Continuiamo a leggere solo se non abbiamo gia' letto "allowedRowRead"
      // righe	   

      if(rowCount>=allowedRowRead ) {
	readMoreGroupsForThisPartition= false;
      } else {
	totGroups++;
        if(!conditionalInsert)
	  //kItem.setItemSet(itemBody,gidB,cidB,itemHead, cidH);
	  kItem.setItemSet(itemBody,gid,cidBody,itemHead, cidHead);
	else
	  //kItem.setItemSetIfExist(itemBody,gidB,cidB,itemHead, cidH);
	  kItem.setItemSetIfExist(itemBody,gid,cidBody,itemHead, cidHead);

	readMoreGroupsForThisPartition= true;
      }

    } else {
      // Inserisco a livello 1 la bid con liste e nella head hid con liste
      if(!conditionalInsert)
	//kItem.setItemSet(itemBody,gidB,cidB,itemHead, cidH);
	kItem.setItemSet(itemBody,gid,cidBody,itemHead, cidHead);
      else
	//kItem.setItemSetIfExist(itemBody,gidB,cidB,itemHead, cidH);
	kItem.setItemSetIfExist(itemBody,gid,cidBody,itemHead, cidHead);
    }

    // La lettura la facciamo solo se abbiamo inserito l'elemento corrente
    // nella struttura. Questo avviene ogni qual volta non si deve uscire
    // dal ciclo.
    if( readMoreGroupsForThisPartition ) {
      moreRecords = result->next();
    }

#if 0 // DEBUG
    if(rowCount % (allowedRowRead/10)==0) {
      std::cout << "rowcount:" << rowCount << "\r";
      std::cout.flush();
    }
#endif
    
  } while(readMoreGroupsForThisPartition && moreRecords  );

  //  std::cout << std::endl << "final row count:" << rowCount << std::endl;
  //  std::cout << "kitem size:" << kItem.size() << std::endl;
   
  return moreRecords;
}





// Qui definisco il tipo TYPENODE. Tutte le classi sono dichiarate come <String>

void partitionWithClusters( const minerule::AlgorithmsOptions& options )
{
  odbc::Connection* odbc_connection = options.getODBCConnection();
  odbc::PreparedStatement* statement = options.getStatement();
  const minerule::HeadBodySourceRowDescription& 
    rowDes = options.getSourceRowDescription();

  //MRLog() << options.getBodyCardinalities() << std::endl;
  //MRLog() << options.getHeadCardinalities() << std::endl;

  MRLogPush("Starting `generalized partition' algorithm");

  // Variabile che contiene la struttura principale
  itemSetCluster <ItemType> kItem;
  // Variabile con i puntatori alla lista dei livelli
  itemSetListCluster <ItemType> prtList;
  // HashMap che simula il lattice. Usata per fare apriori
  newItemSetHashMap <ItemType> pHashMap;

  // srDescriptor allows one to obtain SQL definitions of 
  // source row elements in result.

  odbc::ResultSet* result; // result set used to read the source db

  int isPart,levelIn;
  double support,conf,totGroups;
  double nSup;
  bool toContinue;
  // Connessione e ResultSet Vedi classe Database/Connection.h e cpp
  Connection connection;


// parametri da passare.
  support= options.getSupport()*100;
  conf=    options.getConfidence()*100;
  //  std::cout <<std::endl<<"+ Supporto: "<<support<<"% Confidenza: "<< conf<<"%"<<std::endl;

  // Apertura della connessione e cancellazione db_temporanei
  connection.useODBCConnection(odbc_connection);
  connection.setOutTableName(options.getOutTableName());
  connection.setBodyCardinalities(options.getBodyCardinalities());
  connection.setHeadCardinalities(options.getHeadCardinalities());

  // Ciclo fino alla fine delle partizioni

  isPart = 0;
  result = statement->executeQuery();

  bool readMorePartitions=result->next();
  SourceRowDescriptor srDescriptor(result, rowDes);


  connection.create_tmp_db(1, srDescriptor.getBody(), srDescriptor.getHead());

  MRLog() << "Starting to read the Data Base" << std::endl;   
  int gid = 1;
  while( readMorePartitions ) {
    isPart++;
    
    totGroups=1;
    // Svuoto la lista dei livelli.
    prtList.init();

    MRLog() << "Reading partition number: " << isPart << std::endl;
    readMorePartitions = readSourceTable(kItem,result,rowDes,options,false,gid,totGroups);
    MRLog() << "Done! (Reading partition number: " << isPart << ")" << std::endl;
    MRLog() << "Items in this partition: " << kItem.size() << std::endl;

   // A questo punto ho inserito tutte le tuple nel livello 1 della
   // struttura kItem.
   // Posso generare le regole con supp suff! nSup contiene il numero
   // di gruppi in cui l'item deve essere presente

   /*   std::cout <<"Gruppi Totali di questa partizione "
	<<totGroups
	<<" Supporto "
	<<support<<std::endl; */

   //nSup=ceil( options.getTotGroups()*support/100 );
   nSup=ceil(totGroups*(support/100));

   //   std::cout << "Numero di gruppi Sufficienti " 
   //<< nSup 
   //<< std::endl;
   // A questo punto controllo quali item a livello 1 non hanno supporto sufficiente.
   // Se un elem del body non ha supp. suff elimino lui e la sua head.
   // Se un elem del body ha supp suff. devo controllare se esiste almeno un elemento
   // della head con supp. suff. Se esiste nessun problema altrimenti elimino elemento
   // del body e la head

   //   kItem.printToDesign("Body",isPart);

   MRLog() << "Generating large itemsets for this partition" << std::endl;
   kItem.gen_Large_Rule_Base(nSup, options.getHeadCardinalities());

   // A questo punto vado in pronfondita' a livelli.
   // Genero il livello 2 del body, controllando l'esistenza delle head e l'espolosione della
   // stessa head
   // Genero il livello 3 etc... per far ciò uso la lista prtList che contiene gli indirizzi
   // fisici dei livelli.
   // Finisco quando non posso più estendere il body
   levelIn=1;
   toContinue=true;
   while(toContinue && levelIn<options.getBodyCardinalities().getMax() ) {
      levelIn=levelIn+1;
      toContinue=kItem.gen_Large_ItemSet(prtList,
					 nSup,
					 totGroups,
					 levelIn,
					 options.getHeadCardinalities()	 );
   }
   MRLog() << "Done! (Generating large ...)" << std::endl;

//   kItem.printToDesign("explode",isPart);

  // kItem.printToDesign("BodyExt",isPart);
  // Salvo tutta la struttura in due db temporanei.
  // allo stesso tempo dealloco le strutture
  // Il tracciato del salvataggio è nel solito file lyx
   //  std::cout<<"Save_Large_ItemSet"<<std::endl;
   //  std::cout << "Num Attr. allocati (prima di saveLarge):" << 
   //    MemDebugGenericSourceRowAttribute::getInstanceCounter() << std::endl;
   
   MRLog() << "Saving large large itemsets for this partition" << std::endl;
   kItem.save_Large_ItemSet(1,isPart,connection);
   MRLog() << "Done! (Saving large ...)" << std::endl;
   // std::cout << "Num Attr. allocati: (dopo)" << 
   //    MemDebugGenericSourceRowAttribute::getInstanceCounter() << std::endl;
   
   // Svuoto la lista dei livelli per il ciclo successivo.
   prtList.removeAll();  

 } // fine WHILE

  MRLog() << "Done! (Starting to read the Data Base)" << std::endl;   
  delete result;   // trashing the trashable 
  result = NULL; 


 // std::cout<<"fase di merge"<<std::endl;
 prtList.init();

 // legge i db temporanei salvati da save_large_itemset e ricostruisce la struttura.


 MRLog() << "Reading back saved informations... (merge phase)" << std::endl;
 kItem.mergeItemSet(connection,prtList, srDescriptor);
 MRLog() << "Done! (Reading back saved informations...)" << std::endl;

 totGroups=0;

 // In questa parte leggo di nuovo il db per andare a completare il primo livello

 int partNum=0;
 MRLog() << "Reading again the Data Base" << std::endl;   
 delete statement;   // trashing the trashable 
std::string qry = "SELECT * FROM "+options.getOutTableName()+"_tmpSource";
 statement = odbc_connection->prepareStatement(qry);
 result = statement->executeQuery();
 readMorePartitions = result->next();
 gid = 1;
 assert(result!=NULL);
 while( readMorePartitions ) {
   partNum++;
   double tmpTotGroups=1;
   MRLog() << "Reading again partition number: " << partNum << std::endl;
   readMorePartitions = readSourceTable(kItem,result,rowDes,options,true,gid,tmpTotGroups);
   MRLog() << "Done! (Reading again partition number: " << partNum << ")" << std::endl;
   totGroups+=tmpTotGroups;
 }
  MRLog() << "Done! (Reading again the Data Base)" << std::endl;    

 delete result;
 delete statement;
 // kItem.printToDesign("lista",99);

 // A questo punto ho una struttura su più livelli ma solo il primo livello delle body
 // e delle head ha le gid,cid a posto.

 //  std::cout<<"Gruppi Totali "<<totGroups<<" Supporto "<<support<<std::endl;
 //  nSup=ceil(totGroups*(support/100));
 nSup = ceil( options.getTotGroups()*support/100 );
 MRDebug() << "Total number of groups:" << options.getTotGroups() << " "
	 << "support threshold:" << nSup << std::endl;


  //  std::cout<<"nSup fase II: "<<nSup<<std::endl;

  //  std::cout<<"---  checkSupporBaseDef ---"<<std::endl;
  // Qui faccio lo stesso controllo di sopra.
  // controllo se il livello 1 del body e delle rispettive head sono sufficienti.
  // Se trovo qualcuno a supp. non suff. faccio pruning.
  // se le head sono ok faccio che espandere e controllare il supporto
  // Creo anche una hashmap con il lattice del primo livello.

  //  kItem.printItemSetRecursive(0);

  //  kItem.printToDesign("beforeCheck", 0);
  MRLog() << "Generating final rules..." <<std::endl;
  kItem.checkSupportBaseDef(nSup,pHashMap,prtList);
  //  kItem.printToDesign("afterCheck", 0);
  //  std::cout << "------------------ FINE CHEKC SUPP BASE -------" << std::endl;
  MRLog() << "Items with sufficient support: " << kItem.size() << std::endl;
  toContinue=true;
  levelIn=1;

  //  pHashMap.print();

  //  std::cout<<"STO PER GENERARE GEN_FINAL_COUNT"<<std::endl;
  // Controllo i livelli sfruttando prtList.
  // Controllo i livelli del body, controllando l'esistenza delle head e l'espolosione della
  // stessa head
  while ( (toContinue))
  {
     levelIn=levelIn+1;
     toContinue=kItem.gen_final_count(prtList,nSup,levelIn,pHashMap);
     //prtList.printAddrItemSet();
  }
  MRLog() << "Done! (Generating final rules...)" <<std::endl;

  //  kItem.printToDesign("final",1);
  //  std::cout<<"FINE GEN_FINAL_COUNT"<<std::endl;
  //kItem.printToDesign("Body",numPartition+3);
  connection.deleteDestTable();

  throw std::runtime_error("Algorithm to be updated to use new createResiltTables API");
  // connection.createResultTables(SourceRowDescriptor(connection.getODBCConnection(), minerule.getParsedMinerule()));


  connection.init();

  // Salvo nel db le regole. Vedi struttura nel file lyx
  MRLog() << "Extracting rules and saving in the DB" << std::endl;
  kItem.extractRule(connection,std::vector<ItemType>(),options.getTotGroups());
  MRLog() << "Done! (Extracting rules and saving in the DB)" << std::endl;

  connection.finalize();
  //  kItem.printToDesign("check",98);
  MRLogPop();
}

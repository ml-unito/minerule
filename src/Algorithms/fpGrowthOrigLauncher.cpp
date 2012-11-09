// GO.CPP
// Main. Per ora non c'è il collegamento con i db ed uso file di testo per simulare
// i dati in input

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <stack>
#include <cmath>
#include "Algorithms/tree.h"
#include "Algorithms/Counter.h"
#include "Database/Connection.h"
#include <stdexcept>

#include "Database/SourceRow.h" // defines, among other things, which is the ItemType
#include "Algorithms/AlgorithmsOptions.h"


using namespace minerule;



void
fillCounterFromDB(odbc::ResultSet* result,
		  const minerule::HeadBodySourceRowDescription& rowDes,
		  Counter<ItemType>& gCount)  {
  ItemType item;
  do {
    HeadBodySourceRow curRow(result, rowDes);    
    
    item=curRow.getBody();
    gCount.addValue(item);
   } while( result->next() );
}


void
readDataBaseOrig(odbc::ResultSet* result,
	     const minerule::HeadBodySourceRowDescription& rowDes,
	     Tree<ItemType>&  tree,
	     Counter<ItemType>& gCount,
	     double& totGroups) {
  unsigned int rowCount=0;
  deque<ItemType> itemsOfGroup;

  ItemType gid;
  ItemType gidOld;
  ItemType item;

  do {
    rowCount++;
    HeadBodySourceRow curRow(result, rowDes);    

    gid=curRow.getGroupBody();
    item=curRow.getBody();

    if(rowCount==1)
      gidOld=gid;
    
    if (gid!=gidOld) {
      tree.insertNode( itemsOfGroup,1,gCount,true,true);
      totGroups++;
      
      gidOld=gid;
      itemsOfGroup.clear();
      itemsOfGroup.push_back(item);
    }
    else {
      itemsOfGroup.push_back(item);
    }
  } while( result->next() );

  if(!itemsOfGroup.empty()) {
      tree.insertNode( itemsOfGroup,1,gCount,true,true);
      totGroups++;
  }
}


//using std::cout;
//using std::cin;
//using std::setiosflags;
//using std::ios;
//using std::setprecision;

// Qui definisco il tipo TYPENODE. Tutte le classi sono dichiarate come <String>
//


void origFPGrowth( const minerule::AlgorithmsOptions& options ) {

  odbc::Connection* odbc_connection = options.getODBCConnection();
  odbc::PreparedStatement* statement = options.getStatement();
  const minerule::HeadBodySourceRowDescription& rowDes =
    options.getSourceRowDescription();

  Counter<ItemType> gCount;
  Tree<ItemType> tree;

  ItemType token,label,item;
  double support = options.getSupport()*100;
  //  double conf = options.getConfidence()*100;

  double totGroups,nSup;
  ItemType gid,gidOld;
  Connection connection;
  odbc::ResultSet* result;

  totGroups=0;
// Riempe il contatore generale con tutti i valori possibili.
// gCount.pushElements();

  tree.addRoot( gCount, Tree<ItemType>::ROOTNODE );
  connection.useODBCConnection(odbc_connection);
  connection.setOutTableName(options.getOutTableName());
  

  int logId = MRLogGetNewID();

  MRLog(logId) << "  Reading database (first phase)" << endl;
  // First read (only to fill the Counter)
  result= statement->executeQuery();
    
  if( result->next() ) 
    fillCounterFromDB(result, rowDes,  gCount);

  delete result;
  MRLog(logId) << "  Done! (Reading database - first phase)" << endl;


  //IMPORTANTISSIMO : supponiamo questo sia l'albero da ordinare
  // creo l'ordine dell'albero, impostando il valore ordine nel contatore generale.
  gCount.setOrdina();

  result= statement->executeQuery();

  MRLog(logId) << "  Reading database (second phase)" << endl;
  if( result->next() ) 
    readDataBaseOrig(result, rowDes, tree, gCount, totGroups);

  delete result;
  MRLog(logId) << "  Done! (Reading database - second phase)" << endl;

  //  gCount.print();
  //  tree.preOrderTraversalToDesign("bobo");
  
  //  cout<<"Gruppi Totali "<<totGroups<<" Supporto "<<support<<endl;
  nSup=ceil(totGroups*(support/100));
  //  cout<<"nSup: "<<nSup<<endl;

  throw std::runtime_error("Algorithm to be updated to use new createResiltTables API");
  // connection.createResultTables(SourceRowDescriptor(connection.getODBCConnection(), minerule.getParsedMinerule()));

  MRLog(logId) << "  Inflating conditional FPTrees..." << endl;
// Genera tutti i fp-tree conditional. 
// Eliminando quelli relativi a itemset a supporto non sufficiente
// Per ognuno di essi si occupa, infine, di salvare i risultati sul DB
  gCount.esplodi(connection, nSup, totGroups,true);
  MRLog(logId) << "  Done! (Inflating conditional FPTrees)" << endl;

  MRLog(logId) << "  Extracting rules..." << endl;
  gCount.extractRule(connection,nSup,totGroups);
  MRLog(logId) << "  Done! (Extracting rules)" << endl;
}

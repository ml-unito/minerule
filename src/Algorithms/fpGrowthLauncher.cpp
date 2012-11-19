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
readDataBase(odbc::ResultSet* result,
	     const minerule::SourceRowColumnIds& rowDes,
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
    SourceRow curRow(result, rowDes);    

    gid=curRow.getGroup();
    item=curRow.getBody();

    if(rowCount==1)
      gidOld=gid;
    
    if (gid!=gidOld) {
      gCount.addValues(itemsOfGroup);
      tree.insertNode( itemsOfGroup,1,gCount,false,false,true);
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
    gCount.addValues(itemsOfGroup);
    tree.insertNode( itemsOfGroup,1,gCount,false,false,true);
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


void newFPGrowth( const minerule::AlgorithmsOptions& options ) {

  odbc::Connection* odbc_connection = options.getODBCConnection();
  odbc::PreparedStatement* statement = options.getStatement();
  const minerule::SourceRowColumnIds& rowDes =
    options.getSourceRowDescription();

  //  int val;
//FILE *infile;
  Tree<ItemType> treeOrder;                       // ordered Tree
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
//  gCount.print();
  tree.addRoot( gCount, Tree<ItemType>::ROOTNODE );
  connection.useODBCConnection(odbc_connection);
  connection.setOutTableName(options.getOutTableName());
  connection.setBodyCardinalities(options.getBodyCardinalities());
  connection.setHeadCardinalities(options.getHeadCardinalities());  
  result= statement->executeQuery();

  MRLogPush("Reading the database...");
  if( result->next() ) 
    readDataBase(result, rowDes, tree, gCount, totGroups);
  MRLogPop();

  delete result;
  
  MRLogPush("Creating the new, sorted FPTree... ");
//IMPORTANTISSIMO : supponiamo questo sia l'albero da ordinare
// creo l'ordine dell'albero, impostando il valore ordine nel contatore generale.
  gCount.setOrdina();
  //  gCount.leggiCounter();

  treeOrder.addRoot(gCount,Tree<ItemType>::ROOTNODE);
// creo un nuovo albero leggendo il vecchio e il contatore generale.
  treeOrder.makeOrdina(&tree,&gCount,true);
  //  treeOrder.preOrderTraversalToDesign("bobi2");

  // Creo la lista che lega tutti i nodi uguali.Devo garantire che queste liste vadano
  // da sinistra verso destro
  treeOrder.makeList(gCount);
  MRLogPop();

  //  cout<<"Gruppi Totali "<<totGroups<<" Supporto "<<support<<std::endl;
  nSup=ceil(totGroups*(support/100));
  //  cout<<"nSup: "<<nSup<<std::endl;

  connection.deleteDestTables();
  
  throw std::runtime_error("Algorithm to be updated to use new createResiltTables API");
  // connection.createResultTables(SourceRowMetaInfo(connection.getODBCConnection(), minerule.getParsedMinerule()));

// Genera tutti i fp-tree conditional. 
// Eliminando quelli relativi a itemset a supporto non sufficiente
// Per ognuno di essi si occupa, infine, di salvare i risultati sul DB
  MRLog()<<"Building the conditional fptrees..."<<std::endl;
  gCount.esplodi(connection, nSup, totGroups);

  MRLog()<<"Building rules..."<<std::endl;;
  gCount.extractRule(connection,nSup,totGroups);
}

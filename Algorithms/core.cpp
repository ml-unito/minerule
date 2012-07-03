// CORE.CPP

//using std::cout;
//using std::cin;
//using std::setiosflags;
//using std::ios;
//using std::setprecision;

// Qui definisco il tipo TYPENODE. Tutte le classi sono dichiarate come <String>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>



#include "AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h"

void go();
void partitionWithoutClusters(  const minerule::AlgorithmsOptions& options );
void partitionWithClusters( const minerule::AlgorithmsOptions& options	 );

int main() {
  //  go();
#if 1
  odbc::Connection* connection;
  odbc::ResultSet* result;
  string outTableName;
  minerule::HeadBodySourceRowDescription rowDes;
  minerule::AlgorithmsOptions options;
  MineruleOptions::PartitionAlgorithms partOpt;
  connection = minerule::MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

  options.setSupport(2.0);
  options.setConfidence(75.0);
  partOpt.setRowsPerPartition(100000);
  setOutTableName="prova";


  odbc::PreparedStatement* statement=
    connection->prepareStatement("select * from PROVASOURCE4000 order by gid");
  


 // Descrizione righe nella tabella sorgente
 
  rowDes.groupBodyElems.push_back(1);
  rowDes.clusterBodyElems.push_back(2);
  rowDes.bodyElems.push_back(3);
  rowDes.clusterHeadElems.push_back(4);
  rowDes.headElems.push_back(5);


  options.setConnection(connection);
  options.setStatement(statement);
  options.setSourceRowDescription(rowDes);
  options.setOutTableName(outTableName);
  options.setPartitionOptions(parOpt);
  
  try {    
    partitionWithClusters( options );
  } catch (odbc::SQLException e) {
    cerr << e.getMessage() << endl;
    throw;
  } catch (std::exception e) {
    cerr << e.what() << endl;
    throw;
  }
    
  delete statement;
  delete connection;
#endif
}


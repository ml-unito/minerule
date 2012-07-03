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


void partitionWithoutClusters( const minerule::AlgorithmsOptions& options );
	 

int main(int argc, char** argv)
{
  int numRowsPerPart;
  if(argc==2) {
    numRowsPerPart = atoi(argv[1]);
    cout << "Num rows per partition: " << numRowsPerPart;
  }

#if 1
  odbc::Connection* connection;
  odbc::ResultSet* result;
  
  minerule::HeadBodySourceRowDescription rowDes;
  minerule::AlgorithmsOptions options;

  connection = MineruleOptions::getSharedOptions().getOdbc_db().getConnection();
  //#define CINZIADB
#ifdef CINZIADB
  options.setSupport(20.0);
  options.setConfidence(50.0);
  if(argc==2) 
    options.setRowsPerPartition(numRowsPerPart);
  else
    options.setRowsPerPartition(20);

  odbc::PreparedStatement* statement=
    connection->prepareStatement("select * from InputTableHan order by gi88d");
#else
  options.setSupport(14.0);
  options.setConfidence(75.0);
  options.setRowsPerPartition(300000);
  odbc::PreparedStatement* statement=
    connection->prepareStatement("select * from PROVABASE order by cid");
#endif



 // Descrizione righe nella tabella sorgente
 
  rowDes.groupBodyElems.push_back(1);
  rowDes.bodyElems.push_back(2);

  options.setConnection(connection);
  options.setStatement(statement);
  options.setSourceRowDescription(rowDes);
  options.setOutTableName("outRuleBase");
  
  try {
    partitionWithoutClusters( options );
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


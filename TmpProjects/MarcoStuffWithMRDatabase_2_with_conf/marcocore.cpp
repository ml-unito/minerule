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
#include "MRResultSet.h"

void go();
void partitionWithoutClusters(  const minerule::AlgorithmsOptions& options );
void partitionWithClusters( const minerule::AlgorithmsOptions& options	 );

#define MROPTIONFILE "mroptions"

int main(int argc, char *argv[]) {
  //  go();
#if 1
  odbc::Connection* connection;
  odbc::ResultSet* result;
  string outTableName;
  minerule::HeadBodySourceRowDescription rowDes;
  minerule::AlgorithmsOptions options, options1;
  MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::PartitionBase partOpt;
  //MineruleOptions::PartitionAlgorithms partOpt;
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  mrOpts.readFromFile(MROPTIONFILE);

  connection = mrOpts.getOdbc_db().getConnection();

  options.setSupport(0.001);
  options.setConfidence(75.0);
  partOpt.setRowsPerPartition(90000);
  outTableName="prova";


  odbc::PreparedStatement* statement, *stmt1;
  string queryStr;
  string queryStr1;
  


 // Descrizione righe nella tabella sorgente
 
  rowDes.groupBodyElems.push_back(1);
//  rowDes.clusterBodyElems.push_back(2);
  rowDes.bodyElems.push_back(2);
//  rowDes.clusterHeadElems.push_back(4);
//  rowDes.headElems.push_back(2);

  vector<int> groupCols;
  groupCols.insert(groupCols.end(), 1);
  vector<int> itemCols;
  itemCols.insert(itemCols.end(), 2);

  MIndex mi1("Sales","price",groupCols,itemCols);

  bt_query_t *query, *query1;
  bt_query_t::bt_oper oper = bt_query_t::bt_nooper;

  if (argc > 5 || argc < 2) {
      cerr << "Usage: marcocore {true|false} [[bt_operator] key]" << endl;
      cerr << "Usage example: marcocore true" << endl;
      cerr << "Usage example: marcocore true nooper" << endl;
      cerr << "Usage example: marcocore false lt 1000" << endl;
      cerr << "Usage example: marcocore true betw 200 500" << endl;
     return -1;
  }

  MIndexIterator * mi;
  if (!strcmp(argv[1],"true")) {
	queryStr = "select * from Sales_PriceIdx as BODY where BODY.price < 500";
	queryStr1 = "select * from Sales_PriceIdx as HEAD where HEAD.price > 7000";
  	rowDes.headElems.push_back(2);
  } else {
	  queryStr = "select * from Sales as BODY, Sales as HEAD where BODY.gid=HEAD.gid and HEAD.price > 7000 and BODY.price < 5000";
  	rowDes.headElems.push_back(7);
  }

	  if (argc == 2) oper = bt_query_t::bt_nooper;
	  else if (!strcmp(argv[2],"nooper")) oper = bt_query_t::bt_nooper;
	  else if (!strcmp(argv[2],"lt")) oper = bt_query_t::bt_lt;
	  else if (!strcmp(argv[2],"le")) oper = bt_query_t::bt_le;
	  else if (!strcmp(argv[2],"gt")) oper = bt_query_t::bt_gt;
	  else if (!strcmp(argv[2],"ge")) oper = bt_query_t::bt_ge;
	  else if (!strcmp(argv[2],"betw")) oper = bt_query_t::bt_betw;

	  int first = argc-2;
	  int second = argc-1;
	  if (argc == 4) first++;
	  if (isdigit(argv[first][0]))
	     query1 = new bt_query_t(oper, new int(atoi(argv[first])), new int(atoi(argv[second])));
	  else query1 = new bt_query_t(oper, argv[first], argv[second]);
	  //if (!mi1.existIndex()) mi1.createIndex();
	  //mi = new MIndexIterator(mi1, query1);
	switch (oper) {
	case bt_query_t::bt_lt:
		queryStr += " and BODY.price < ";
		queryStr += argv[first];
		break;
	case bt_query_t::bt_le:
		queryStr += " and BODY.price <= ";
		queryStr += argv[first];
		break;
	case bt_query_t::bt_gt:
		queryStr += " and BODY.price > ";
		queryStr += argv[first];
		break;
	case bt_query_t::bt_ge:
		queryStr += " and BODY.price >= ";
		queryStr += argv[first];
		break;
	case bt_query_t::bt_betw:
		queryStr += " and BODY.price >= ";
		queryStr += argv[first];
		queryStr += " and BODY.price <= ";
		queryStr += argv[second];
		break;
	}

	  if (!mi1.existIndex()) mi1.createIndex();
	  mi = new MIndexIterator(mi1, query1);
	int howMany =0;
	for (; mi->current() != mi->end(); (*mi)++) {
		cout << mi->getCurrentGID() << ' ' << mi->getCurrentItem() << endl;
	}
/*
	//queryStr += " order by gid";
cout << queryStr << endl;
	statement = connection->prepareStatement(queryStr.c_str());
  
  //MRResultSetIterator mr(statement,(!strcmp(argv[1],"true")) ? mi : NULL);
  MRResultSetIterator mr(statement, NULL);

  options.setConnection(connection);
  options.setStatement(statement);
  options.setMRIterator(mr);
  options.setSourceRowDescription(rowDes);
  options.setOutTableName(outTableName);
  options.setPartitionOptions(partOpt);
  
  long start = time(NULL);
  try {    
    partitionWithClusters( options );
  if (!strcmp(argv[1],"true")) {
	stmt1 = connection->prepareStatement(queryStr1.c_str());
  MRResultSetIterator mr1(stmt1, NULL);
  options.setStatement(stmt1);
  options.setMRIterator(mr1);
    partitionWithClusters( options );
}
  } catch (odbc::SQLException e) {
    cerr << e.getMessage() << endl;
    throw;
  } catch (std::exception e) {
    cerr << e.what() << endl;
    throw;
  }
  long elapsed = time(NULL)-start;

  cout << "Elapsed time: " << elapsed << endl;
  delete statement;
*/    
  delete connection;
#endif
}


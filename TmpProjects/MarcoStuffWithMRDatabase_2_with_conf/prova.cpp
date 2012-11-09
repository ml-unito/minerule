// CORE.CPP

//using std::cout;
//using std::cin;
//using std::setiosflags;
//using std::ios;
//using std::setprecision;

// Qui definisco il tipo TYPENODE. Tutte le classi sono dichiarate come <String>

#include<stdio.h>
#include<string>
#include<stdlib.h>
#include <iostream>
#include <fstream>



#include "AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h"
#include "Database/MRResultSet.h"
#include "prova.h"

#define MROPTIONFILE "mroptions"

#include <unistd.h>
//#include "mindex.h"
#include "gist.h"
#include "gist_support.h"
#include "gist_btree.h"
#include "gist_cursor.h"


using namespace std;
using namespace minerule;


extern gist_ext_t *str_extension;
extern gist_ext_t *int_extension;

bool createIndex(char * filename) {

  string idxName = filename;
  idxName += ".PriceIdx";

  //CREATE TABLE
  gist *gindex = new gist;

  rc_t status;
  bool typeIsNum = true;
  if (typeIsNum)
    status = gindex->create(idxName.c_str(), int_extension);
  else 
    status = gindex->create(idxName.c_str(), str_extension);
  gindex->flush();
  if (status != RCOK) {
    cout << "Error opening table: " << idxName << endl;
    delete gindex;
    return false;
  }

  int howMany = 0;

  SalesRow row;
  ifstream in(filename);

  while(!in.eof()) {
  IndexElem ie;
	ie.rowPos = in.tellg();
	in >> row;
	if (in.eof()) break;
  int intItem = row.price;
	ie.rowPos = row.item;
	ie.gid = row.gid;

	 if (typeIsNum) {
    if (gindex->insert((void* )&intItem, sizeof(intItem),
        (void *) &ie, sizeof(ie)) != RCOK) {
      std::cerr << "can't insert" << endl;
    }
	 }
	 else {
    //if (gindex->insert((void* )item.c_str(), item.length()+1,
        //(void *) &ie, sizeof(ie)) != RCOK) {
      //std::cerr << "can't insert" << endl;
  //  }
	 }
	 howMany++;
	 if (howMany % 1000 == 0) cout << "." << flush;
  }//while
  cout << "done" << endl;

  gindex->close();
  in.close();

  delete gindex;
  return true;

}//End CreateIndex

/************************************************/

bool fetchInit(char * idxFileName, std::set<IndexElem>& groups, void *query){

  gist *gindex = new gist;

  rc_t status = gindex->open(idxFileName); //, extension);

  if (status != RCOK) {
    delete gindex;
    cout << "Error opening table " << idxFileName << endl;
    return false;
   }

  gist_cursor_t cursor;

  if (gindex->fetch_init(cursor, (const gist_query_t*) query) != RCOK) {
      std::cerr << "can't initialize cursor" << endl;
      return false;
  }
 // groups.erase(groups.begin(),groups.end());

  bool eof;
  char key[gist_p::max_tup_sz];
  smsize_t klen;
  char data[gist_p::max_tup_sz];
  smsize_t dlen;
  

		int howMany = 0;
		do {
      klen = gist_p::max_tup_sz;
      dlen = gist_p::max_tup_sz;
      if (gindex->fetch(cursor, (void *) key, klen, (void *) data, dlen, eof) != RCOK) {
          std::cerr << "Can't fetch from cursor" << endl;
          gindex->close();
	       return false;
      }
		if (!eof)  {
			//groups[*(int*)data] = *(int*) data;
			IndexElem ie((char*)data);
			groups.insert(ie);
//		  	std::cout << *(int*)key << " : " << *(int*)data << endl;
		   howMany++;
		}
		} while (!eof);

  gindex->close();
  
		std::cout << "Size: " << groups.size() << " su " << howMany << endl;
//		current = groups.begin();
  return true;
}

int main(int argc, char *argv[]) {
  //  go();
#if 1
  odbc::Connection* connection;
  odbc::ResultSet* result;
  string outTableName;
  minerule::HeadBodySourceRowDescription rowDes;
  minerule::AlgorithmsOptions options;
  MineruleOptions::PartitionAlgorithms partOpt;
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  mrOpts.readFromFile(MROPTIONFILE);

  connection = mrOpts.getOdbc_db().getConnection();

  options.setSupport(2.0);
  options.setConfidence(75.0);
  partOpt.setRowsPerPartition(100000);
  outTableName="prova";


  odbc::PreparedStatement* statement;
  string queryStr;
  


 // Descrizione righe nella tabella sorgente
 
  rowDes.groupBodyElems.push_back(1);
//  rowDes.clusterBodyElems.push_back(2);
  rowDes.bodyElems.push_back(2);
//  rowDes.clusterHeadElems.push_back(4);
  rowDes.headElems.push_back(7);

  vector<int> groupCols;
  groupCols.insert(groupCols.end(), 1);

  //MIndex mi1("Sales","price",groupCols);

  bt_query_t *query, *query1;
  bt_query_t::bt_oper oper = bt_query_t::bt_nooper;

  if (argc > 5 || argc < 2) {
      std::cerr << "Usage: marcocore {true|false} [[bt_operator] key]" << endl;
      std::cerr << "Usage example: marcocore true" << endl;
      std::cerr << "Usage example: marcocore true nooper" << endl;
      std::cerr << "Usage example: marcocore false lt 1000" << endl;
      std::cerr << "Usage example: marcocore true betw 200 500" << endl;
     return -1;
  }

  Comparison * cf;

  //MIndexIterator * mi;
  if (!strcmp(argv[1],"true")) {
	  queryStr = "select * from Sales as BODY, Sales as HEAD where BODY.gid = ? and HEAD.gid = BODY.gid";
  } else
	  queryStr = "select * from Sales as BODY, Sales as HEAD where BODY.gid=HEAD.gid";
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
	query = new bt_query_t(bt_query_t::bt_nooper, NULL, NULL);
//	  if (!mi1.existIndex()) mi1.createIndex();
//	  mi = new MIndexIterator(mi1, query1);
	switch (oper) {
	case bt_query_t::bt_lt:
		queryStr += " and BODY.price < ";
		queryStr += argv[first];
		cf = new LessThan(argv[first]);
		break;
	case bt_query_t::bt_le:
		queryStr += " and BODY.price <= ";
		queryStr += argv[first];
		cf = new LessOrEqual(argv[first]);
		break;
	case bt_query_t::bt_gt:
		queryStr += " and BODY.price > ";
		queryStr += argv[first];
		cf = new GreaterThan(argv[first]);
		break;
	case bt_query_t::bt_ge:
		queryStr += " and BODY.price >= ";
		queryStr += argv[first];
		cf = new GreaterOrEqual(argv[first]);
		break;
	case bt_query_t::bt_betw:
		queryStr += " and BODY.price >= ";
		queryStr += argv[first];
		queryStr += " and BODY.price <= ";
		queryStr += argv[second];
		cf = new InBetween(argv[first],argv[second]);
		break;
	}

	//statement = connection->prepareStatement(queryStr.c_str());
	statement = connection->prepareStatement("select * from Sales order by price");
  
  //MRResultSetIterator mr(statement,&mi);
//  MRResultSetIterator mr(statement,(!strcmp(argv[1],"true")) ? mi : NULL);

  options.setConnection(connection);
  options.setStatement(statement);
  //options.setMRIterator(mr);
  options.setSourceRowDescription(rowDes);
  options.setOutTableName(outTableName);
  options.setPartitionOptions(partOpt);
 /* 
  long start = time(NULL);
  try {    
    partitionWithClusters( options );
  } catch (odbc::SQLException e) {
    std::cerr << e.getMessage() << endl;
    throw;
  } catch (std::exception e) {
    std::cerr << e.what() << endl;
    throw;
  }
  long elapsed = time(NULL)-start;

  cout << "Elapsed time: " << elapsed << endl;
  result = statement->executeQuery();

  ofstream out("Sales.db");
  while (result->next()) {
	out << result->getString(1);
	out << ' ' << result->getString(2);
	out << ' ' << result->getString(3);
	out << ' ' << result->getString(4);
	out << ' ' << result->getString(5);
	out << endl;
  }
  out.close();
*/
//  createIndex("Sales.db");

  std::set<IndexElem> groups;

  ifstream in("Sales.db");
  SalesRow row;
  int howMany = 0;
  int logId = MRLogGetNewID();

  MRLogPush("Executing Selection");
  if (!strcmp(argv[1],"true")) {
	MRLog(logId) << "Starting using index" << endl;
	fetchInit("Sales.db.PriceIdx",groups,query1);
	for (std::set<IndexElem>::iterator i = groups.begin(); i != groups.end(); i++) {
//		in.seekg((*i).rowPos);
//		in >> row;
		howMany ++;
	}
	MRLog(logId) << "Selected: " << howMany << " rows" << endl;
  } else {
	MRLog(logId) << "Starting without index" << endl;
	fetchInit("Sales.db.gidIdx",groups,query);
	for (std::set<IndexElem>::iterator i = groups.begin(); i != groups.end(); i++) {
		in.seekg((*i).rowPos);
		in >> row;
		if (cf->compare(row.price)) howMany ++;
	}
	//while(!in.eof()) {
	//	in >> row;
	//	if (in.eof()) break;
	//	if (cf->compare(row.price)) howMany ++;
	//}
	MRLog(logId) << "Selected: " << howMany << " rows" << endl;
  }
  MRLogPop();
  in.close();
  delete statement;
  delete connection;
#endif
}


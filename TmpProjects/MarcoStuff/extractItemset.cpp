#include "bodymap.h"
#include "constraints.h"
#include <multimap.h>

#define MROPTIONFILE "mroptions"

int main(int argc, char *argv[]) {
  //  go();
#if 1
  if (argc != 4) {
	std::cerr << "Usage: extractItemset minsupp maxCard minprice" << endl;
	exit(1);
  }
  odbc::Connection* connection;
  string outTableName;
  minerule::HeadBodySourceRowDescription rowDes;
  minerule::AlgorithmsOptions options, options1;
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  //MineruleOptions::MiningAlgorithms::PartitionBase& partOpt = mrOpts.getMiningAlgorithms().getPartitionBase();
  mrOpts.readFromFile(MROPTIONFILE);

  connection = mrOpts.getOdbc_db().getODBCConnection();

  options.setSupport(Converter(argv[1]).toDouble());
  options.setConfidence(96.0);
  //artOpt.setRowsPerPartition(100000);
  outTableName="marcoProva";


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

  rowDes.headElems.push_back(2);

  char buf[1024];
  ifstream qin("query.txt");
  qin.getline(buf,1024);
  queryStr = buf;
  //qin.getline(buf,1024);
  sprintf(buf,"SELECT RAND(%d);",(int) options.getSupport()*1000);
  queryStr1 = buf;
  qin.close();
	//queryStr += " order by gid";
  odbc::ResultSet* result, *result1;
  odbc::ResultSetMetaData* rsmd;
	stmt1 = connection->prepareStatement(queryStr1.c_str());
  	result1 = stmt1->executeQuery();
	
	std::cout << queryStr << endl;
	statement = connection->prepareStatement(queryStr.c_str());
  
  //MRResultSetIterator mr(statement,(!strcmp(argv[1],"true")) ? mi : NULL);
  //MRResultSetIterator mr(statement, NULL);

  options.setConnection(connection);
  options.setStatement(statement);
  //options.setMRIterator(mr);
  options.setSourceRowDescription(rowDes);
  options.setOutTableName(outTableName);
  options.setMiningAlgorithmsOptions(mrOpts.getMiningAlgorithms());
  
  GidList bodyGids;

  float support = Converter(argv[1]).toDouble();
int maxBody = Converter(argv[2]).toLong();
int maxHead = 6;
  result = statement->executeQuery();
  ItemType gid1;

//  stmt1 = connection->prepareStatement(queryStr1.c_str());
//  result1 = stmt1->executeQuery();

  bool withGids = (argc > 3 && !strcmp(argv[3],"false") ? false : true);
  bool found = /*found && */Transaction::findGid(gid1,result,rowDes,true);

  int howManyGroups = 0;
  int totalGroups = 0;
  int howManyRows = 0;
  int howManyPart = 1;
  rsmd = result->getMetaData();
  int nFields = rsmd->getColumnCount();
  MRLogPush("Reading partition ");
  while (!result->isAfterLast()) {
	for (int i=1; i<=nFields; i++) result->getString(i);
	result->next();
  }

  MRLogPop();
  delete statement;
  delete connection;
#endif
}

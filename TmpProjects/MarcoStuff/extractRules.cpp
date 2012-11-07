#include "constraints.h"
#include "bodymap.h"

void go();

#define MROPTIONFILE "mroptions"

int main(int argc, char *argv[]) {
  //  go();
#if 1
  odbc::Connection* connection;
  string outTableName;
  minerule::HeadBodySourceRowDescription rowDes;
  minerule::AlgorithmsOptions options, options1;
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  MineruleOptions::MiningAlgorithms::PartitionBase& partOpt = mrOpts.getMiningAlgorithms().getPartitionBase();
  mrOpts.readFromFile(MROPTIONFILE);

  connection = mrOpts.getOdbc_db().getConnection();

  options.setSupport(Converter(argv[1]).toDouble());
  options.setConfidence(96.0);
  partOpt.setRowsPerPartition(20000);
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
  qin.getline(buf,1024);
  queryStr1 = buf;
  qin.close();
	//queryStr += " order by gid";
  cout << queryStr << endl;
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

MRLogPush("Starting query");
  odbc::ResultSet* result, *result1;

float support = Converter(argv[1]).toDouble();
int maxBody = 5;
int maxHead = 5;
  result = statement->executeQuery();
  ItemType gid1;

  stmt1 = connection->prepareStatement(queryStr1.c_str());
  result1 = stmt1->executeQuery();

  bool withGids = (argc > 2 && !strcmp(argv[2],"false") ? false : true);
  bool found = Transaction::findGid(gid1,result1,rowDes,true);
  found = found && Transaction::findGid(gid1,result,rowDes,true);
  char * resultfile = tmpnam(NULL);
  BodyMap bodyMap(resultfile,1);
  BodyMap headMap(resultfile,1);

  int howManyGroups = 0;
  int totalGroups = 0;
  int howManyRows = 0;
  int howManyPart = 1;
//  rsmd = result->getMetaData();
//  int nFields = rsmd->getColumnCount();
  ofstream out("partition");
  cout << "Reading partition " << howManyPart << " ..." << endl;
  while (!result->isAfterLast()) {
	HeadBodySourceRow hbsr(result, rowDes);
	ItemType gid = hbsr.getGroupBody();
	Transaction t1(rowDes), t2(rowDes);
	t1.loadBody(gid,result,0);
	bool found2 = t2.findGid(gid,result1,rowDes);
	if (found2) {
		t2.loadHead(gid,result1,0);
	}
//		std::cout << "Read transation " << gid << " of size " << t1.size() << " and size " << t2.size() << endl;
//		howManyRows += bodyMap.add(gid,t1,t2);
		howManyRows += bodyMap.add(t1,totalGroups);
		howManyRows += headMap.add(t2,totalGroups);
		howManyGroups++;
//	}
		totalGroups++;

	if (howManyGroups > partOpt.getRowsPerPartition()) {
		std::cout << "Reading partition " << howManyPart << " done." << endl;
		howManyRows = 0;
		if (!withGids) {
  		bodyMap.updateCount();
//  		bodyMap.pruneMap(support*howManyGroups);
  		std::cout << "Total bodies after pruning: " << bodyMap.size() << endl;
		}
		std::cout << "Saving partition " << howManyPart << "..." << flush;
		//bodyMap.saveMap(out,withGids);
		std::cout << "done." << endl;
//		bodyMap.erase(bodyMap.begin(),bodyMap.end());
  howManyGroups = 0;
		howManyPart++;
		std::cout << "Reading partition " << howManyPart << " ..." << endl;
	}

  }
  if (howManyGroups > 0 && !withGids) {
	bodyMap.updateCount();
	//bodyMap.pruneMap(support*howManyGroups);
	bodyMap.pruneMap(support*totalGroups);
  	std::cout << "Total bodies after pruning: " << bodyMap.size() << endl;
	std::cout << "Saving partition " << howManyPart << "..." << flush;
//	bodyMap.saveMap(out,withGids);
	std::cout << "done." << endl;
	//bodyMap.erase(bodyMap.begin(),bodyMap.end());
  }
  out.close();
/*
totalGroups = 99920;
*/
  cout << "Total rows: " << howManyRows << endl;
  cout << "Total groups: " << totalGroups << endl;
  ifstream in("partition");
//  bodyMap.loadMap(in,withGids);
  in.close();
  if (!withGids) {
  delete result;
  delete result1;
  result = statement->executeQuery();
  result1 = stmt1->executeQuery();
  found = Transaction::findGid(gid1,result1,rowDes,true);
  found = found && Transaction::findGid(gid1,result,rowDes,true);
  cout << "Reading again database " << " ..." << endl;
  int n = 0;
  while (!result->isAfterLast()) {
	HeadBodySourceRow hbsr(result,rowDes);
	ItemType gid=hbsr.getGroupBody();
	Transaction t1(rowDes), t2(rowDes);
	t1.loadBody(gid,result,0);
	bool found2 = t2.findGid(gid,result1,rowDes);
	if (found2) {
		t2.loadHead(gid,result1,0);
//		std::cout << "Read transation " << gid << " of size " << t1.size() << " and size " << t2.size() << endl;
		howManyRows += bodyMap.add(n++,t1,t2,true);
	}
  }
  }
  MRLogPop();
  MRLogPush("Starting rule extraction");
  bodyMap.updateCount();
  headMap.updateCount();
  cout << "Total bodies before pruning: " << bodyMap.size() << endl;
  MRLogPop();
  MRLogPush("Starting pruning ...");
  bodyMap.pruneMap(support*totalGroups,true);
  headMap.pruneMap(support*totalGroups,true);
  NewRuleSet rs;
  MRLogPop();
  bodyMap.setTotalGroups(totalGroups);
  MRLogPush("Starting rule extraction ...");
  vector<BodyMap::iterator> v;
  vector<BodyMap::iterator> v1;
  multimap<int, BodyMap::iterator> temp;
//  for (int i=0; i<bodyMap.size(); i++) {
//      BodyMap::iterator j;
//      int min = INT_MAX;
        for (BodyMap::iterator i = bodyMap.begin(); i!=bodyMap.end(); i++)
        	if (!i->second.done) temp.insert(pair<int, BodyMap::iterator>(i->second.size(),i));
//              if (!i->second.done && min > i->second.size() ) { min = i->second.size(); j = i; }
        for (multimap<int, BodyMap::iterator>::iterator i = temp.begin(); i!=temp.end(); i++)
        v.insert(v.end(),i->second);
//      j->second.done = true;
 // }
  NewRule r;
  //int nrules = bodyMap.buildItemset(rs,r,v,maxBody,support*totalGroups,-1);
  for (BodyMap::iterator i = headMap.begin(); i!=headMap.end(); i++)
        v1.insert(v1.end(),i);
  NewRuleSet rs1;
//  for (int i=0; i<rs.size(); i++)
 //       nrules += headMap.addHead(rs1,rs[i],v1,maxHead,support*totalGroups,-1);
  cout << "Total bodies after pruning: " << v.size() << endl;
  bodyMap.openOutputFiles();
  int nrules = bodyMap.buildRules(rs1,r,v,v1,maxBody,maxHead,support*totalGroups,-1);
  bodyMap.closeOutputFiles();
  cout << "After generating rules: " << nrules << endl;
  MRLogPop();

  delete statement;
  delete connection;
#endif
}

// CORE.CPP

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>
#include <iterator>
#include <algorithm>


#include "AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h"
#include "MRResultSet.h"
#include "Database/ItemType.h"
#include "Utils/Converter.h"
#include "Database/Connection.h"

using namespace minerule;

  minerule::AlgorithmsOptions options, options1;

/*
//typedef int ItemType;
class ItemType {
public:
int item; int price;
ItemType () : item(-1), price(0) {}
ItemType (int i, int p) : item(i), price(p) {}
friend istream& operator>>(istream& in, ItemType& i1) {
in >> i1.item;
return in;
}
friend std::ostream& operator<<(ostream& out, const ItemType& i1) {
out << i1.item;
return out;
}
friend bool operator<(const ItemType& i1, const ItemType& i2) {
return i1.item < i2.item;
}
friend bool operator>(const ItemType& i1, int i) {
return i1.item > i;
}
friend bool operator!=(const ItemType& i1, const ItemType& i2) {
return i1.item != i2.item;
}
friend bool operator==(const ItemType& i1, const ItemType& i2) {
return i1.item == i2.item;
}
friend bool operator==(const ItemType& i1, int i) {
return i1.item == i;
}
ItemType& operator=(int value) { item = value; return *this; }
};
*/


class Transaction : public std::set<ItemType> {
  HeadBodySourceRowDescription srd;
public:
  Transaction(HeadBodySourceRowDescription& rowDes) : std::set<ItemType>(), srd(rowDes) {}
  void load(ItemType& gid, odbc::ResultSet *rs) {
    HeadBodySourceRow hbsr(rs,srd);

    while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
      insert(hbsr.getBody());
      rs->next();
      if(!rs->isAfterLast())
	hbsr.init(rs,srd);
    }
  }
  static bool findGid(ItemType& gid, odbc::ResultSet *rs, HeadBodySourceRowDescription& srd, bool init=false) {
    if (init) { rs->next(); return true; }
    
    HeadBodySourceRow hbsr(rs,srd);
    while (!rs->isAfterLast() &&  ItemType(hbsr.getGroupBody()) < gid ) {
      rs->next();
      
      if(!rs->isAfterLast())
	hbsr.init(rs,srd);
    }
    
    return !rs->isAfterLast() && gid == hbsr.getGroupBody();
  }
};


class MapElement : public std::set<ItemType> {
public:
  int count;
  MapElement() : count(0) {}
};
typedef MapElement GidList;


//class BodyMapElement : public std::set<ItemType> {
class BodyMapElement : public MapElement {
public:
  pair<iterator, bool> insert(const value_type& x) { return std::set<ItemType>::insert(x); }
  map<ItemType, MapElement > heads;
  void insert(const ItemType& item, MapElement& gidList, bool secondPass = false);
  bool pruneMap (float threshold);
  bool updateCount ();
  //	void saveMap (ostream& out, bool withGids = true);
  //	void loadMap (istream& in, bool withGids = true);
};

void BodyMapElement::insert(const ItemType& item, MapElement& gidList, bool secondPass) {
  map<ItemType, MapElement>::iterator found = heads.find(item);
  if (found == heads.end()) {
    if (!secondPass) heads[item] = gidList;
  } else found->second.insert(gidList.begin(),gidList.end());
}

bool BodyMapElement::pruneMap (float threshold) {
  //	if (size() < threshold) return false;
  map<ItemType, MapElement> newMap;
  for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
    if (i->second.count >= threshold) {i->second.count = 0; newMap[i->first] = i->second;}
  heads = newMap;
  return heads.size() > 0;
}

bool BodyMapElement::updateCount () {
  //	if (size() < threshold) return false;
  //	map<ItemType, MapElement> newMap;
  for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
    i->second.count += i->second.size();
  //		if (i->second.size() >= threshold) newMap[i->first] = i->second;
  //	heads = newMap;
  return heads.size() > 0;
}

/*
  void BodyMapElement::saveMap (ostream& out, bool withGids) {
  out << ' ' << heads.size();
  for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++) {
  out << ' ' << i->first;
  out << ' ' << i->second.size();
  if (withGids) {
  out  << ' ';
  copy(i->second.begin(), i->second.end(), std::ostream_iterator<ItemType>(out, " "));
  }
  i->second.erase(i->second.begin(), i->second.end());
  }
  out << endl;
  }

  void BodyMapElement::loadMap (istream& in, bool withGids) {
  int n,m; in >> n;
  ItemType item, gid;
  MapElement me;
  for (int i=0; i<n; i++) {
  in >> item;
  in >> m;
  heads[item].count += m;
  if (withGids) {
  in >> m;
  for (int j=0; j<m; j++) { in >> gid; heads[item].insert(gid); }
  } //else { me = heads[item]; }
  }
  }*/

class NewRule {
public:
  vector<ItemType> body, head;
  map<ItemType, BodyMapElement>::iterator lastBody;
  map<ItemType, MapElement>::iterator lastHead;
  int bodySupp;
  double conf;

  GidList gids;
  NewRule (map<ItemType, BodyMapElement>::iterator b, GidList& g) : lastBody(b), gids(g) {
    body.push_back(b->first);
    lastHead = b->second.heads.begin();
  }
  NewRule (NewRule& r, map<ItemType, BodyMapElement>::iterator b, GidList& g) : body(r.body), lastBody(b), gids(g) {
    body.push_back(b->first);
    lastHead = b->second.heads.begin();
  }
  NewRule (map<ItemType, BodyMapElement>::iterator b, map<ItemType, MapElement>::iterator h, GidList& g, int bs) : lastBody(b), lastHead(h), gids(g), bodySupp(bs) {
    body.push_back(b->first);
    head.push_back(h->first);
  }
  NewRule (NewRule& r, map<ItemType, BodyMapElement>::iterator b, GidList& g, int bs) : body(r.body), head(r.head), lastBody(b), lastHead(r.lastHead), gids(g), bodySupp(bs) {
    body.push_back(b->first);
  }
  NewRule (NewRule& r, map<ItemType, MapElement>::iterator h, GidList& g) : body(r.body), head(r.head), lastBody(r.lastBody), lastHead(h), gids(g), bodySupp(r.bodySupp) {
    head.push_back(h->first);
  }
  friend std::ostream& operator<<(ostream& out, NewRule r) {
    vector<ItemType>::iterator i;
    for (i=r.body.begin(); i!=r.body.end(); i++) {
      if (i != r.body.begin()) out << ", ";
      out << *i;
    }
    out << " -> ";
    for (i=r.head.begin(); i!=r.head.end(); i++) {
      if (i != r.head.begin()) out << ", ";
      out << *i;
    }
    out << endl;
    return out;
  }
};

class NewRuleSet : public vector<NewRule> {};

class BodyMap : public map<ItemType, BodyMapElement> {
  vector<GidList> bodySupports;
public:
  int add(ItemType& gid, Transaction& t1, Transaction& t2, bool secondPass = false);
  //	void saveMap(ostream& out, bool withGids = true);
  //	void loadMap(istream& in, bool withGids = true);
  void pruneMap(float threshold);
  void updateCount ();
  //  void extract1Rule (NewRuleSet& rs, float threshold);
  //  void extendBody (NewRuleSet& rs, float threshold, int maxBody);
  //  void extendHead (NewRuleSet& rs, NewRuleSet& rs1, float threshold, int maxHead);
  void createBodies (NewRuleSet& rs, float threshold, int maxBody);
  void createHeads (NewRuleSet& rs, NewRuleSet& rs1, float threshold, int maxHead);
};

int BodyMap::add(ItemType& gid, Transaction& t1, Transaction& t2, bool secondPass) {
  int howMany = 0;
  MapElement me;
  me.insert(gid);
  for (Transaction::iterator i = t1.begin(); i != t1.end(); i++) {
    map<ItemType, BodyMapElement>::iterator found = find(*i);
    if (found == end()) {
      if (secondPass) continue;
      BodyMapElement bme;
      //			bme.insert(gid);
      (*this)[*i] = bme;
      found = find(*i);
    }
    found->second.insert(gid);
    for (Transaction::iterator j = t2.begin(); j != t2.end(); j++)
      if (*i != *j /*&& i->price < j->price*/) {
	howMany++;
	found->second.insert(*j,me,secondPass);
      }
  }
  return howMany;
}

/*void BodyMap::saveMap (ostream& out, bool withGids) {
  for (iterator i = begin(); i != end(); i++) {
  out << i->first;
  i->second.saveMap(out, withGids);
  }
  }

  void BodyMap::loadMap (istream& in, bool withGids) {
  ItemType item;
  while (!in.eof()) {
  in >> item;
  if (in.eof()) break;
  (*this)[item].loadMap(in, withGids);
  }
  }
*/
void BodyMap::pruneMap (float threshold) {
  BodyMap newMap;
  for (iterator i = begin(); i != end(); i++)
    if (i->second.pruneMap(threshold)) newMap[i->first] = i->second;
  (*this) = newMap;
}

void BodyMap::updateCount () {
  for (iterator i = begin(); i != end(); i++)
    i->second.updateCount();
}

#if 0
void BodyMap::extract1Rule (NewRuleSet& rs, float threshold) {
  int n = 0;
  for (iterator i = begin(); i != end(); i++, n++) {
    bodySupports.insert(bodySupports.end(),i->second);
    for (map<ItemType, MapElement>::iterator j = i->second.heads.begin(); j != i->second.heads.end(); j++) {
      //GidList newGidList;
      //set_intersection(i->second.begin(),i->second.end(),
      //		 j->second.begin(), j->second.end(),
      //		 inserter(newGidList,newGidList.begin()));
      //if (newGidList.size() >= threshold) {
      if (j->second.size() >= threshold) {
	//NewRule r(i,j,newGidList);
	NewRule r(i,j,j->second,n);
	rs.insert(rs.end(),r);
      }
    }
  }
}
#endif

void BodyMap::createBodies (NewRuleSet& rs, float threshold, int maxBody) {
  for (iterator i = begin(); i != end(); i++) {
    NewRule r(i,i->second);
    rs.insert(rs.end(),r);
  }
  for (int n = 0; n < rs.size(); n++) {
    NewRuleSet::iterator i = rs.begin()+n;
    NewRule& rc = *i;
    if (rc.body.size() < maxBody) {
      BodyMap::iterator lb = rc.lastBody;
      for (BodyMap::iterator j = ++lb; j != end(); j++) {
	GidList newGidList;
	set_intersection(rc.gids.begin(),rc.gids.end(),
			 j->second.begin(), j->second.end(),
			 inserter(newGidList,newGidList.begin()));
	if (newGidList.size() >= threshold) {
	  NewRule r(rc,j,newGidList);
	  rs.insert(rs.end(),r);
	}
      }
    }
  }
}

#if 0
void BodyMap::extendBody (NewRuleSet& rs, float threshold, int maxBody) {
  for (int n = 0; n < rs.size(); n++) {
    NewRuleSet::iterator i = rs.begin()+n;
    NewRule& rc = *i;
    if (rc.body.size() < maxBody) {
      BodyMap::iterator lb = rc.lastBody;
      for (BodyMap::iterator j = ++lb; j != end(); j++) {
	GidList newGidList,newGidList1;
	set_intersection(bodySupports[rc.bodySupp].begin(),bodySupports[rc.bodySupp].end(),
			 j->second.begin(), j->second.end(),
			 inserter(newGidList,newGidList.begin()));
	if (newGidList.size() >= threshold) {
	  bodySupports.insert(bodySupports.end(),newGidList);
	  int k = bodySupports.size()-1;
	  map<ItemType, MapElement>::iterator found = j->second.heads.find(rc.lastHead->first);
	  if (found != j->second.heads.end()) {
	    //set_intersection(newGidList.begin(),newGidList.end(),
	    set_intersection(rc.gids.begin(),rc.gids.end(),
			     found->second.begin(), found->second.end(),
			     inserter(newGidList1,newGidList1.begin()));
	    if (newGidList1.size() >= threshold) {
	      NewRule r(rc,j,newGidList1,k);
	      r.lastHead = found;
	      rs.insert(rs.end(),r);
	    }
	  }
	}
      }
    }
  }
}


void BodyMap::extendHead (NewRuleSet& rs, NewRuleSet& rs1, float threshold, int maxHead) {
  for (int n = 0; n < rs.size(); n++) {
    //NewRuleSet::iterator i = rs.begin()+n;
    NewRule& rc = rs[n];
    if (rc.head.size() < maxHead) {
      map<ItemType, MapElement>::iterator lh = rc.lastHead;
      map<ItemType, MapElement>::iterator eh = rc.lastBody->second.heads.end();
      for (map<ItemType, MapElement>::iterator j = ++lh; j != eh; j++) {
	GidList newGidList;
	//cout << "Extending head of " << *i << " with " << j->first  << " size " << j->second.size() << endl;
	if (find(rc.body.begin(), rc.body.end(), j->first) == rc.body.end() &&
	    rc.head.find(j->first) == rc.head.end()) {
	  set_intersection(rc.gids.begin(),rc.gids.end(),
			   j->second.begin(), j->second.end(),
			   inserter(newGidList,newGidList.begin()));
	  if (newGidList.size() >= threshold) {
	    NewRule r(rc,j,newGidList);
	    rs1.insert(rs1.end(),r);
	  }
	}
      }
    }
  }
}
#endif

void BodyMap::createHeads (NewRuleSet& rs, NewRuleSet& rs1, float threshold, int maxHead) {
  for (int n = 0; n < rs.size(); n++) {
    //NewRuleSet::iterator i = rs.begin()+n;
    NewRule& rc = rs[n];
    if (rc.head.size() < maxHead) {
      map<ItemType, MapElement>::iterator lh = rc.lastHead;
      map<ItemType, MapElement>::iterator eh = rc.lastBody->second.heads.end();
      double supportBody = rc.gids.size();

      for (map<ItemType, MapElement>::iterator j = lh; j != eh; j++) {
	GidList newGidList;
	//cout << "Extending head of " << *i << " with " << j->first  << " size " << j->second.size() << endl;
	if (std::find(rc.body.begin(), rc.body.end(), j->first) == rc.body.end()/* &&
						      rc.head.find(j->first) == rc.head.end()*/) {
	  set_intersection(rc.gids.begin(),rc.gids.end(),
			   j->second.begin(), j->second.end(),
			   inserter(newGidList,newGidList.begin()));
	  if (newGidList.size() >= threshold) {
	    NewRule r(rc,j,newGidList);
	    r.lastHead++;
	    r.conf = newGidList.size() / supportBody;
	    rs1.insert(rs1.end(),r);
	  }
	}
      }
    }
  }
}

void insertRules( const NewRuleSet& rs, double totGroups ) {
  #warning settare bene c
  Connection connection;
  connection.useODBCConnection(MineruleOptions::getSharedOptions().getOdbc_db().getODBCConnection());
  connection.setOutTableName("prova");
  //  connection.setBodyCardinalities(options.getBodyCardinalities());
  //  connection.setHeadCardinalities(options.getHeadCardinalities());

  NewRuleSet::const_iterator it;
  for(it=rs.begin(); it!=rs.end(); it++) {
    connection.insert( it->body, 
			    it->head, 
			    it->gids.size()/totGroups, 
			    it->conf );
  }
}


//void go();
//void partitionWithoutClusters(  const minerule::AlgorithmsOptions& options );
//void partitionWithClusters( const minerule::AlgorithmsOptions& options	 );

#define MROPTIONFILE "mroptions"



int main(int argc, char *argv[]) {
  //  go();
#if 1
  assert( argc>1 );
  odbc::Connection* connection;
  string outTableName;
  minerule::HeadBodySourceRowDescription rowDes;
  MineruleOptions::PartitionAlgorithms partOpt;
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  mrOpts.readFromFile(MROPTIONFILE);

  connection = mrOpts.getOdbc_db().getODBCConnection();

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

  //  MIndex mi1("Sales","price",groupCols,itemCols);

  //	queryStr = "select * from T20I6D100K as BODY order by gid";
  //	queryStr1 = "select * from T20I6D100K as HEAD order by gid";
  //	queryStr = "select Customer,Item,Price from Purchase as BODY where BODY.price < 500 order by Customer";
  //	queryStr1 = "select Customer,Item,Price from Purchase as HEAD where HEAD.price < 500 order by Customer";
  //	queryStr = "select gid,item,price from SalesOri as BODY where BODY.price > 0 order by gid, item";
  //	queryStr1 = "select gid,item,price from SalesOri as HEAD where HEAD.price > 0 order by gid, item";
  //	queryStr = "select * from Sales as BODY where BODY.price < 500 order by gid";
  //	queryStr1 = "select * from Sales as HEAD where HEAD.price > 7000 order by gid";
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
  options.setPartitionOptions(partOpt);
  
  GidList bodyGids;

  MRLogPush("Starting new algorithm");
  odbc::ResultSet* result, *result1;

  float support = Converter(argv[1]).toDouble();
  int maxBody = 6;
  int maxHead = 6;
  result = statement->executeQuery();
  ItemType gid1;

  stmt1 = connection->prepareStatement(queryStr1.c_str());
  result1 = stmt1->executeQuery();

  bool withGids = (argc > 1 && !strcmp(argv[1],"false") ? false : true);
  bool found = Transaction::findGid(gid1,result1, rowDes, true);
  found = found && Transaction::findGid(gid1,result,rowDes,true);
  BodyMap bodyMap;

  int howManyGroups = 0;
  int totalGroups = 0;
  int howManyRows = 0;
  int howManyPart = 1;
  ofstream out("partition");
  cout << "Reading partition " << howManyPart << " ..." << endl;
  while (!result->isAfterLast()) {
    HeadBodySourceRow hbsr(result, rowDes);
    ItemType gid = hbsr.getGroupBody();
    Transaction t1(rowDes), t2(rowDes);
    t1.load(gid,result);
    bool found2 = t2.findGid(gid,result1,rowDes);
    if (found2) {
      t2.load(gid,result1);
      //		std::cout << "Read transation " << gid << " of size " << t1.size() << " and size " << t2.size() << endl;
    }
    howManyRows += bodyMap.add(gid,t1,t2);
    howManyGroups++;
	
    totalGroups++;

    if (howManyGroups > partOpt.getRowsPerPartition()) {
      cout << "Reading partition " << howManyPart << " done." << endl;
      howManyRows = 0;
      if (!withGids) {
	bodyMap.updateCount();
	//  		bodyMap.pruneMap(support*howManyGroups);
	std::cout << "Total bodies after pruning: " << bodyMap.size() << endl;
      }
      cout << "Saving partition " << howManyPart << "..." << flush;
      //		bodyMap.saveMap(out,withGids);
      cout << "done." << endl;
      //		bodyMap.erase(bodyMap.begin(),bodyMap.end());
      howManyGroups = 0;
      howManyPart++;
      cout << "Reading partition " << howManyPart << " ..." << endl;
    }

  }
  if (howManyGroups > 0 && !withGids) {
    bodyMap.updateCount();
    //bodyMap.pruneMap(support*howManyGroups);
    bodyMap.pruneMap(support*totalGroups);
    cout << "Total bodies after pruning: " << bodyMap.size() << endl;
    cout << "Saving partition " << howManyPart << "..." << flush;
    //	bodyMap.saveMap(out,withGids);
    cout << "done." << endl;
    //bodyMap.erase(bodyMap.begin(),bodyMap.end());
  }
  out.close();
  /*
    totalGroups = 99920;
  */
  cout << "Total groups: " << totalGroups << endl;
  ifstream in("partition");
  //  bodyMap.loadMap(in,withGids);
  in.close();
  if (!withGids) {
    delete result;
    delete result1;
    result = statement->executeQuery();
    result1 = stmt1->executeQuery();
    found = Transaction::findGid(gid1,result1,rowDes);
    found = found && Transaction::findGid(gid1,result,rowDes);
    cout << "Reading again database " << " ..." << endl;
    while (!result->isAfterLast()) {
      HeadBodySourceRow hbsr(result,rowDes);
      ItemType gid=hbsr.getGroupBody();
      Transaction t1(rowDes), t2(rowDes);
      t1.load(gid,result);
      bool found2 = t2.findGid(gid,result1,rowDes);
      if (found2) {
	t2.load(gid,result1);
	//		std::cout << "Read transation " << gid << " of size " << t1.size() << " and size " << t2.size() << endl;
	howManyRows += bodyMap.add(gid,t1,t2,true);
      }
    }
  }
  bodyMap.updateCount();
  cout << "Total bodies before pruning: " << bodyMap.size() << endl;
  bodyMap.pruneMap(support*totalGroups);
  cout << "Total bodies after pruning: " << bodyMap.size() << endl;
  NewRuleSet rs;
  //bodyMap.extract1Rule(rs,support*totalGroups);
  cout << "After extracting 1 to 1 rules: " << rs.size() << endl;
  //bodyMap.extendBody(rs,support*totalGroups,maxBody);
  bodyMap.createBodies(rs,support*totalGroups,maxBody);
  cout << "After extending bodies, rules: " << rs.size() << endl;
  int nrules = 0;
  do {
    NewRuleSet rs1;
    //bodyMap.extendHead(rs,rs1,support*totalGroups,maxHead);
    bodyMap.createHeads(rs,rs1,support*totalGroups,maxHead);
    nrules += rs1.size();
    //  for (int i=0; i<rs1.size(); i++) cout << rs1[i] << endl;
    cout << "Extracted rules: " << rs1.size() << endl;
    insertRules(rs1, totalGroups);

    rs = rs1;
  } while (rs.size() > 0);
  //  for (int i=0; i<rs.size(); i++) cout << rs[i] << endl;
  cout << "After Extendig heads, rules: " << nrules << endl;

  MRLogPop();
  cout << "Press the enter key ..." << endl;
  while (cin.peek() == 0);
  /*
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
    std::cerr << e.getMessage() << endl;
    throw;
    } catch (std::exception e) {
    std::cerr << e.what() << endl;
    throw;
    }
    long elapsed = time(NULL)-start;

    cout << "Elapsed time: " << elapsed << endl;
  */  
  delete statement;
  delete connection;
#endif
}

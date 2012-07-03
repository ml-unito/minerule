#include <iostream>
#include <stdlib.h>
#include <string>
#include<fstream>
#include <unistd.h>
//#include "mindex.h"
#include <odbc++/resultsetmetadata.h>
#include "MRResultSet.h"
#include "Utils/SQLUtils.h"

using namespace std;
using namespace minerule;


gist_ext_t *str_extension= &MR_bt_str_ext;
gist_ext_t *int_extension= &MR_bt_int_ext;

gist *gindex = new gist;


#define MROPTIONFILE "mroptions"
#define MAX_TABLES 10
#define NOT_FOUND (-1)


/****************************************************/

std::string MIndex::concatenate(std::vector<int>& v) {
  std::string group = "";
  char buf[255];
    for (size_t i = 0; i<v.size(); i++) {
		 if (i>0) group += "_";
		 std::sprintf(buf,"%d",v[i]);
		 group += buf;
	 }
	 return group;
}

/****************************************************/

void showFile(const char *table){


  cout <<"GLI ELEMENTI DEL FILE "<< table << " sono: " << endl;

  ifstream fin( table , ios::binary);

  int group,pt;
      while(fin) {
    fin.read( (char*)&group, sizeof(int) );
    if(!fin)
      break;

    cout << group;

    fin.read((char*)&pt,sizeof(int));
    if(!fin)
      break;

    cout << " : " << pt << endl;
  }


  fin.close();
}


//*********************SHOW INDEX*********************

//void MIndex::showIndex(const char *tableName, const char *itemColName, const char *groupColName){
void MIndex::showIndex(const char *tableName, const char *itemColName, std::vector<int>& groupCols){
	MIndex mi(tableName, itemColName, groupCols);
	mi.showIndex();
}

void MIndex::showIndex() {

  bt_query_t *query;

  //OPEN TABLE
  gist *gindex = new gist;

  cout <<"GLI ELEMENTI DI " << priIdx << " SONO:" <<endl;

  rc_t status = gindex->open(priIdx.c_str()); //, extension);

  if (status != RCOK) {
    delete gindex;
    cout << "Error2 opening table." << endl;
    return;
  }

  gist_cursor_t cursor;

  query = new bt_query_t(bt_query_t::bt_nooper, NULL, NULL);

  if (gindex->fetch_init(cursor, query) != RCOK) {
      cerr << "can't initialize cursor" << endl;
      return;
  }

  bool eof;
  char key[gist_p::max_tup_sz];
  smsize_t klen;
  char data[gist_p::max_tup_sz];
  smsize_t dlen;
  int cnt = 0;

  for (;;) {
      klen = gist_p::max_tup_sz;
      dlen = gist_p::max_tup_sz;
      if (gindex->fetch(cursor, (void *) key, klen, (void *) data, dlen, eof) != RCOK) {
          cerr << "can't fetch from cursor" << endl;
			break;
      }
      if (eof) break;

      cout << (char*) key;
      cout <<" : " << (char*) data <<endl;
      cnt++;
  }
  cout << "Retrieved " << cnt << " items" << endl;

  gindex->close();

}


/************************************************/
bool MIndex::createIndex(const char *tableName, const char *itemColName, std::vector<int>& groupCols, std::vector<int>& itemCols) { 
	MIndex mi(tableName, itemColName, groupCols, itemCols);
	return mi.createIndex();
}

bool MIndex::createIndex() {

  odbc::ResultSet* rs;
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();

  try {
    odbc::Connection* connection=
      mrOpts.getOdbc_db().getConnection();

  //MRLog() << "Connessione ok!" << endl;
  odbc::Statement* state = connection->createStatement(
			    odbc::ResultSet::TYPE_SCROLL_INSENSITIVE,
			    odbc::ResultSet::CONCUR_READ_ONLY);
  std::string q("SELECT * FROM ");
  q += name;
  rs = state->executeQuery(q.c_str());

  //MRLog() << "Fine!" << endl;

  } catch (odbc::SQLException& e) {
    MRErr() << e.getMessage() << endl;
    throw;
  }
  //CREATE TABLE
  gist *gindex = new gist;

  odbc::ResultSetMetaData * rsmd = rs->getMetaData();
  int iColNum = rs->findColumn(itemCol);


  rc_t status;
  bool typeIsNum = SQLUtils::isNumericType(
	      (odbc::Types::SQLType) rsmd->getColumnType(iColNum));
  if (typeIsNum)
    status = gindex->create(priIdx.c_str(), int_extension);
  else 
    status = gindex->create(priIdx.c_str(), str_extension);
  gindex->flush();
  if (status != RCOK) {
    cout << "Error opening table: " << priIdx << endl;
    delete gindex;
    return false;
  }

  cout << "Creating index " << priIdx << " as type " << (typeIsNum ? "MR_bt_int" : "MR_bt_str") << endl;

  int howMany = 0;

  while(rs->next()) {
  std::string item, group;
  int intItem;
  size_t i;
	for (i = 0; i<groupCol.size(); i++) {
		 if (i>0) group += ",";
		 group += rs->getString(groupCol[i]);
	}
	group += ":";
	for (i = 0; i<groupCol.size(); i++) {
		 if (i>0) group += ",";
		 group += rs->getString(groupCol[i]);
	}
cerr << group << endl;
	if (typeIsNum) intItem = rs->getInt(iColNum);
	 else item=rs->getString(iColNum);

	 if (typeIsNum) {
    if (gindex->insert((void* )&intItem, sizeof(intItem),
        (void *) group.c_str(), group.length()+1) != RCOK) {
      cerr << "can't insert" << endl;
    }
	 }
	 else {
    if (gindex->insert((void* )item.c_str(), item.length()+1,
        (void *) group.c_str(), group.length()+1) != RCOK) {
      cerr << "can't insert" << endl;
    }
	 }
	 howMany++;
	 if (howMany % 1000 == 0) cout << "." << flush;
  }//while
  cout << "done" << endl;

  gindex->close();

  delete gindex;
  delete rs;
  return true;

}//End CreateIndex


/************************************************/

//void MIndex::removeIndex(const char *tableName, const char *itemColName, const char *groupColName){
void MIndex::removeIndex(const char *tableName, const char *itemColName, std::vector<int>& groupCols){
	MIndex mi(tableName, itemColName, groupCols);
	mi.removeIndex();
}

void MIndex::removeIndex(){

    //DROP TABLE
  if (unlink(priIdx.c_str()))
    cout << "There is no index for table " << name << endl;
  else
    cout << "Index " << priIdx << " dropped." << endl;

}

bool MIndex::existIndex(const char *tableName, const char *itemColName, std::vector<int>& groupCols){
	MIndex mi(tableName, itemColName, groupCols);
	return mi.existIndex();
}
bool MIndex::existIndex() {
  
  gist *gindex = new gist;

  rc_t status = gindex->open(priIdx.c_str()); //, extension);
  gindex->close();

  delete gindex;
  return status == RCOK;

}
/************************************************/

bool MIndex::fetchInit(set<std::string>& groups, void *query){

  gist *gindex = new gist;

  rc_t status = gindex->open(priIdx.c_str()); //, extension);

  if (status != RCOK) {
    delete gindex;
    cout << "Error opening table " << priIdx << endl;
    return false;
   }

  gist_cursor_t cursor;

  if (gindex->fetch_init(cursor, (const gist_query_t*) query) != RCOK) {
      cerr << "can't initialize cursor" << endl;
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
          cerr << "Can't fetch from cursor" << endl;
          gindex->close();
	       return false;
      }
		if (!eof)  {
			//groups[*(int*)data] = *(int*) data;
			std::string group((char*)data);
			groups.insert(group);
//		  	cout << *(int*)key << " : " << *(int*)data << endl;
		   howMany++;
		}
		} while (!eof);

  gindex->close();
  
		cout << "Size: " << groups.size() << " su " << howMany << endl;
//		current = groups.begin();
  return true;
}

/************************************************/
/*
bool MIndex::fetchNext(std::string& rowTable){

//	if (current == groups.end()) return false;
//	rowTable = *current;
	// *rowTable = atoi(current->first.c_str());
	//currentGid = atoi(current->first.c_str());
//	current++;
	return true;

}

bool MIndex::check(std::string& gid) {
//	bool ok = true;
//	std::string currentGid;
//	while (*current < gid && ok) ok = fetchNext(&currentGid);
//	return (ok && gid == *current);
	return false;
}
*/
MIndexIterator::MIndexIterator(MIndex& mi, bt_query_t * query) {
	mi.fetchInit(groups,query);
	cur= groups.begin();
}

void MIndexIterator::intersect(MIndex& mi, bt_query_t * query) {
	set<std::string> miGroups,newGroups;
	mi.fetchInit(miGroups,query);
	insert_iterator<set<std::string> > ii(newGroups, newGroups.begin());
	set_intersection(groups.begin(),groups.end(),miGroups.begin(),miGroups.end(),ii);
	groups = newGroups;
	cur= groups.begin();
}

void MIndexIterator::unify(MIndex& mi, bt_query_t * query) {
	mi.fetchInit(groups,query);
	cur= groups.begin();
}

void MIndexIterator::unify(MIndexIterator& mii) {
	set<std::string> newGroups;
	insert_iterator<set<std::string> > ii(newGroups, newGroups.begin());
	set_intersection(groups.begin(),groups.end(),mii.groups.begin(),mii.groups.end(),ii);
	groups = newGroups;
	cur= groups.begin();
}

#ifdef MAIN
// ********MAIN*******//
int main(int argc, char** argv) {

	 MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
	 mrOpts.readFromFile(MROPTIONFILE);
	 vector<int> groupCols;
	 groupCols.insert(groupCols.end(), 8);

	MIndex MIR("sales_facts","product_key",groupCols);
	MIndex MIR1("sales_facts","store_key",groupCols);
  int rowId=0;

  bt_query_t *query, *query1;
  bt_query_t::bt_oper oper = bt_query_t::bt_nooper;

  if (argc != 2 && argc != 3) {
	  cerr << "Usage: mindex [bt_operator] key" << endl;
	  cerr << "Usage example: mindex lt 33" << endl;
	  return -1;
  }

  if (argc == 2) oper = bt_query_t::bt_nooper;
  else if (!strcmp(argv[1],"lt")) oper = bt_query_t::bt_lt;
  else if (!strcmp(argv[1],"le")) oper = bt_query_t::bt_le;
  else if (!strcmp(argv[1],"gt")) oper = bt_query_t::bt_gt;
  else if (!strcmp(argv[1],"ge")) oper = bt_query_t::bt_ge;
  else if (!strcmp(argv[1],"betw")) oper = bt_query_t::bt_betw;

  query = new bt_query_t(bt_query_t::bt_nooper, NULL, NULL);
  query1 = new bt_query_t(oper, argv[argc-1], NULL);

  if (!MIR.existIndex()) MIR.createIndex();
  if (!MIR1.existIndex()) MIR1.createIndex();

//  MIndex::showIndex("sales_facts","product_key","customer_key");
/*
  MIR.fetchInit(query1);
  MIR1.fetchInit(query1);
//  MIR2.fetchInit(query);
*/
  odbc::ResultSet* rs;
  odbc::Connection* connection= mrOpts.getOdbc_db().getConnection();
  
  odbc::PreparedStatement * pst = connection->prepareStatement("select * from sales_facts where customer_key = ?");

  //MIndexIterator mi(MIR1, query1);
  MIndexIterator mi(MIR, query);
  MIndexIterator mi1(MIR1, query1);
  mi.intersect(MIR1, query1);
  mi.unify(mi1);
  MRResultSetIterator mr(pst,&mi);

  int prev = -1, current;
  while( mr.next()){
	  current = mr.getResultSet()->getInt(8);
	  if (prev != current) {
		  prev = current;
		  cout << "DATA MAIN: " << current <<endl;
	  }
  }

  delete query;
  delete pst;
  delete connection;
  return 0;

}

#endif

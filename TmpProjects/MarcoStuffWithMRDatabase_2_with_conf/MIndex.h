//	mindex.h

#ifndef MINDEX_H
#define MINDEX_H
#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/Database/SourceRowAttribute.hpp"
#include "minerule/Database/SourceRowColumnIds.hpp"
#include "minerule/Database/SourceRowAttributeCollection.hpp"
using namespace minerule;
#include <ansidecl.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/resultset.h>
#if (GCC_VERSION >= 3000)
#include <ext/hash_map>
using namespace __gnu_cxx;
#else
#include <hash_map.h>
#endif
#include <set>
#include <string>
#include "gist.h"
#include "gist_support.h"
#include "gist_btree.h"
#include "gist_cursor.h"

using namespace std;

#define MYSIZE 255
/******************************************************************
  This is the Mining Index class. A Mining Index is a B+-tree that
  indexes all groups having at least one item satisfying a given
  property. The MIndex class provides the abstraction to the use of
  a mining index. It creates the B+-tree if necessary, by using the
  Gist 2.0 package.
  To use the index, first call fetchInit that initializes a map
  structure with the list of all groups satisfying the query, and then
  repeatedly call fetchNext to get the group id. 
  *****************************************************************/
class MIndex {
	friend class MIndexIterator;
	std::string name;
	std::string priIdx;
	std::string secIdx;
	std::string fileIdx;
	std::string itemCol;
	std::vector<int>& groupCol;
	std::vector<int>& itemCols;
   std::string concatenate(std::vector<int>& v);
	public:
	//MIndex(const char *tableName, const char *itemColName, const char *groupColName) :
	MIndex(const char *tableName, const char *itemColName, std::vector<int>& gr) :
	  	name(tableName), itemCol(itemColName), groupCol(gr) {
			priIdx = tableName; priIdx += itemColName;
			priIdx += '_';
			priIdx += concatenate(groupCol);
			
			secIdx = priIdx; fileIdx = priIdx;
		  	priIdx += "IdxPri";
		  	secIdx += "IdxSec";
		  	fileIdx += "FileIndex.bin";
	}
	MIndex(const char *tableName, const char *itemColName, std::vector<int>& gr, std::vector<int>& itCols) : itemCols(itCols) {
		MIndex(tableName, itemColName, gr);
	}
	static void removeIndex(const char *tableName, const char *itemColName, std::vector<int>& gr);
	static void showIndex(const char *tableName, const char *itemColName, std::vector<int>& gr);
	static bool createIndex(const char *tableName, const char *itemColName, std::vector<int>& gr);
	static bool existIndex(const char *tableName, const char *itemColName, std::vector<int>& gr);
	bool createIndex();
	bool existIndex();
	void showIndex();
	void removeIndex();
	//bool check(int gid);
	bool fetchInit(std::set<std::string>& groups, void * query);
	//bool fetchNext(int * data);

};

/******************************************************************
  This is the Mining Index Iterator class. It is used to provide an
  abstraction for a list of Mining Indexes to be used together.
  To use this class, first creates the MIndex objects as needed,
  then add them to the MIndexItaerator object and repeatedly call
  next to get the group ids satisfying all Mining Indexes in the
  list.
  *****************************************************************/

class MIndexIterator {
	std::set<std::string> groups;
	std::set<std::string>::iterator cur;
	public:
	MIndexIterator() { cur = groups.begin(); }
	MIndexIterator(MIndex& mi, bt_query_t * query);
	void intersect(MIndex& mi, bt_query_t * query);
	void unify(MIndex& mi, bt_query_t * query);
	void unify(MIndexIterator& mii);
	//std::string& operator*() { return *cur; }
	std::set<std::string>::iterator operator++(int) { return cur++; }
	std::set<std::string>::iterator operator++() { return ++cur; }
	std::set<std::string>::iterator current() { return cur; }
	std::set<std::string>::iterator begin() { return groups.begin(); }
	std::set<std::string>::iterator end() { return groups.end(); }
	bool next(std::string& value) { value = *cur; cur++; return (cur == groups.end()); }
	void reset() {
	  cur=groups.begin();
	}
	
};
#endif





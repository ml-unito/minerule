#ifndef __BODYMAP_H
#define __BODYMAP_H

#include<limits.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include "Algorithms/AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h"
#include "Database/MRResultSet.h"
#include "Database/ItemType.h"
#include "Utils/Converter.h"
#include "Database/Connection.h"
#include "Utils/Bitstring.h"

namespace minerule {

class AggregateMonoConstraint;
class AggregateAntiMonoConstraint;
class NewRule;


//class Transaction : public std::set<std::pair<ItemType, ItemType> > {
class Transaction : public std::set<ItemType> {
  HeadBodySourceRowDescription srd;
  public:
	Transaction(HeadBodySourceRowDescription& rowDes) : std::set<ItemType>(), srd(rowDes) {}
 std::vector<int> values;
	void loadBody(ItemType& gid, odbc::ResultSet *rs, int nFields) {
		HeadBodySourceRow hbsr(rs,srd);
		while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
			insert(hbsr.getBody());
			//for (int j=3; j<=nFields; j++) values.insert(values.end(),rs->getInt(j));
			rs->next();
			if(!rs->isAfterLast()) hbsr.init(rs,srd);
		}
	}
	void loadHead(ItemType& gid, odbc::ResultSet *rs, int nFields) {
		HeadBodySourceRow hbsr(rs,srd);
		while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
			insert(hbsr.getHead());
			//for (int j=3; j<=nFields; j++) values.insert(values.end(),rs->getInt(j));
			rs->next();
			if(!rs->isAfterLast()) hbsr.init(rs,srd);
		}
	}
	static bool findGid(ItemType& gid, odbc::ResultSet *rs, HeadBodySourceRowDescription& srd, bool init=false) {
		if (init) { rs->next(); return true; }
		HeadBodySourceRow hbsr(rs,srd);
		while (!rs->isAfterLast() && gid > hbsr.getGroupBody()) {
			rs->next();
			if(!rs->isAfterLast()) hbsr.init(rs,srd);
		}
		return !rs->isAfterLast() && gid == hbsr.getGroupBody();
	}
};

//class MapElement : public std::set<ItemType> {
class MapElement : public BitString {
  public:
	int counter;
	void insert(const int x) { set(x,true); }
	MapElement() : counter(0) {}
	void operator=(const BitString& bs) { BitString::operator=(bs); }
};
typedef MapElement GidList;

class MinMax {
  public:
	int min;
	int max;
	MinMax () : min(INT_MAX), max(0) {}
	MinMax (int m1, int m2) : min(m1), max(m2) {}
	int minValue() { return min; }
	int maxValue() { return max; }
};

//class BodyMapElement : public std::set<ItemType> {
class BodyMapElement : public MapElement {
  public:
 std::vector<MinMax> attribute;
	bool done;
	BodyMapElement() { done = false; }
	BodyMapElement(const BodyMapElement& bm) {
		done = bm.done;
		for (size_t i=0; i<bm.attribute.size(); i++) attribute.insert(attribute.end(),bm.attribute[i]);
		BitString::operator=((BitString&)bm);
	}
	//pair<iterator, bool> insert(const value_type& x) { return std::set<ItemType>::insert(x); }
	void insert(const int x) { set(x,true); }
	map<ItemType, MapElement > heads;
	void insert(const ItemType& item, const int gid, bool secondPass = false);
	bool pruneMap(double threshold, bool onlyBody = false);
	bool updateCount ();
//	void saveMap (std::ostream& out, bool withGids = true);
//	void loadMap (istream& in, bool withGids = true);
	void setMinMax (int which, int v);
};

class NewRule {
  public:
 std::vector<ItemType> body, head;
	map<ItemType, BodyMapElement>::iterator lastBody;
	map<ItemType, MapElement>::iterator lastHead;
	bool satisfy;
	GidList gids;
	int bodySupp;
	NewRule () : satisfy(false) {}
	NewRule (map<ItemType, BodyMapElement>::iterator b, GidList& g) : lastBody(b), satisfy(false), gids(g) {
		body.push_back(b->first);
		lastHead = b->second.heads.begin();
	}
	NewRule (NewRule& r, std::map<ItemType, BodyMapElement>::iterator b, GidList& g) : body(r.body), lastBody(b), satisfy(r.satisfy), gids(g) {
		body.push_back(b->first);
		lastHead = b->second.heads.begin();
	}
	NewRule (map<ItemType, BodyMapElement>::iterator b, std::map<ItemType, MapElement>::iterator h, GidList& g, int bs) : lastBody(b), lastHead(h), satisfy(false), gids(g), bodySupp(bs) {
		body.push_back(b->first);
		head.push_back(h->first);
	}
	NewRule (NewRule& r, std::map<ItemType, BodyMapElement>::iterator b, GidList& g, int bs) : body(r.body), head(r.head), lastBody(b), lastHead(r.lastHead), satisfy(r.satisfy), gids(g), bodySupp(bs) {
		body.push_back(b->first);
	}
	NewRule (NewRule& r, std::map<ItemType, BodyMapElement>::iterator b, int bs, GidList& g) : body(r.body), head(r.head), lastBody(b), lastHead(r.lastHead), satisfy(r.satisfy), gids(g), bodySupp(bs) {
		head.push_back(b->first);
	}
	NewRule (NewRule& r, std::map<ItemType, MapElement>::iterator h, GidList& g) : body(r.body), head(r.head), lastBody(r.lastBody), lastHead(h), satisfy(r.satisfy), gids(g), bodySupp(r.bodySupp) {
		head.push_back(h->first);
	}
	friend std::ostream& operator<<(std::ostream& out, NewRule r) {
	 std::vector<ItemType>::iterator i;
		for (i=r.body.begin(); i!=r.body.end(); i++) {
			if (i != r.body.begin()) out << ", ";
			out << *i;
		}
		out << " -> ";
		for (i=r.head.begin(); i!=r.head.end(); i++) {
			if (i != r.head.begin()) out << ", ";
			out << *i;
		}
		//out << std::endl;
		return out;
	}
};

class NewRuleSet : public std::vector<NewRule> {};

class BodyMap : public std::map<ItemType, BodyMapElement> {
	static int nextid;
std::string outfile;
	Connection * connection;
	double totGroups;
	ofstream outR;
	ofstream outHB;
// std::vector<GidList> bodySupports;
 std::vector<int> itemsets;
//	int curBodySupp;
  public:
	int nextID() { return nextid++; }
	BodyMap(std::string f, double totG) : outfile(f), totGroups(totG) {};
	BodyMap(Connection& cc, double totG) : connection(&cc), totGroups(totG) {};
	void openOutputFiles() {
	std::string filename = outfile + ".r";
		outR.open(filename.c_str());
	std::string filename1 = outfile + ".hb";
		outHB.open(filename1.c_str());
	}
	void closeOutputFiles() { outR.close(); outHB.close(); }
	void setTotalGroups(double totG) { totGroups = totG; }
 std::vector<AggregateMonoConstraint *> monoConstr;
 std::vector<AggregateAntiMonoConstraint *> antiMonoConstr;
	bool checkAntiMono (NewRule& rc);
	bool checkMono (NewRule& rc);
	int add(const int gid, Transaction& t1, Transaction& t2, bool secondPass = false);
//	int add(const int gid, Transaction& t1, bool secondPass = false);
	//int add(ItemType& gid, Transaction& t1);
//	int add(const int gid, ItemType& t1, ItemType& t2, bool secondPass);
	int add(Transaction& t1, const int gid);
//	void saveMap(std::ostream& out, bool withGids = true);
//	void loadMap(istream& in, bool withGids = true);
	void pruneMap(double threshold, bool onlyBody = false);
	void updateCount ();
	int buildItemset (NewRuleSet& rs, NewRule& rc, std::vector<BodyMap::iterator>& v, int maxCard, float threshold, int j);
	int generateStartItemSets (NewRuleSet& rs, NewRule& rc, std::vector<BodyMap::iterator>& v, int maxCard, float threshold, int j);
	int buildRules (NewRuleSet& rs2, NewRule rc, BodyMap::iterator p, float threshold, int maxBody, int maxHead);
	int buildHead (NewRuleSet& rs, float threshold, int maxHead, int suppBody, NewRule& rc, std::map<ItemType, MapElement>::iterator j);
	int buildRules (NewRuleSet& rs, NewRule& rc, std::vector<BodyMap::iterator>& vb, std::vector<BodyMap::iterator>& vh, int maxBodyCard, int maxHeadCard, float threshold, int j);
 std::vector<BodyMap::iterator> buildHead (NewRuleSet& rs1, NewRule rc, std::vector<BodyMap::iterator>& v, int maxCard, float threshold, int j);
	void howManyItemsets();
	void saveRules( const NewRuleSet& rs, double bodySupp);
	void saveItemsets( const NewRuleSet& rs, double bodySupp);
	void saveItemset( const NewRule& r, double bodySupp);
};

} // end namespace

#endif

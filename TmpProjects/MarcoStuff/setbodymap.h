#ifndef __BFSWITHGIDSNOCROSS_H__
#define __BFSWITHGIDSNOCROSS_H__

#include "Algorithms/MiningAlgorithmBase.h"
#include "Algorithms/sqlCoreConn.h"

  class Transaction : public set<ItemType> {
    HeadBodySourceRowDescription srd;
  public:
    Transaction(HeadBodySourceRowDescription& rowDes) : set<ItemType>(), srd(rowDes) {}
    void loadBody(ItemType& gid, odbc::ResultSet *rs, int n) {
      HeadBodySourceRow hbsr(rs,srd);

      while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
	insert(hbsr.getBody());
	rs->next();
	if(!rs->isAfterLast())
	  hbsr.init(rs,srd);
      }
    }

    void loadHead(ItemType& gid, odbc::ResultSet *rs, int n) {
      HeadBodySourceRow hbsr(rs,srd);

      while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
	insert(hbsr.getHead());
	rs->next();
	if(!rs->isAfterLast())
	  hbsr.init(rs,srd);
      }
    }

    static bool findGid(ItemType& gid, odbc::ResultSet *rs, HeadBodySourceRowDescription& srd, bool init=false) {
      if (init) { 
	if(!rs->isAfterLast()) {
	  rs->next(); 
	  return true;
	} else
	  return false;
      }      
      
      HeadBodySourceRow hbsr(rs,srd);
      while (!rs->isAfterLast() && gid > hbsr.getGroupBody() ) {
	rs->next();
      
	if(!rs->isAfterLast())
	  hbsr.init(rs,srd);
      }
    
      return !rs->isAfterLast() && gid == hbsr.getGroupBody();
    }
  };


  class MapElement : public set<int> {
  public:
    int counter;
    MapElement() : counter(0) {}
    bool moreThan(double th) { return size() > th; }
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

//class BodyMapElement : public set<ItemType> {
class BodyMapElement : public MapElement {
  public:
//	vector<MinMax> attribute;
	bool done;
	BodyMapElement() : done(false) {}
//	BodyMapElement(const BodyMapElement& bm) {
//		done = bm.done;
//		for (int i=0; i<bm.attribute.size(); i++) attribute.insert(attribute.end(),bm.attribute[i]);
//		BitString::operator=((BitString&)bm);
//	}
    pair<iterator, bool> insert(const value_type& x) { return set<int>::insert(x); }
	map<ItemType, MapElement > heads;
	void insert(const ItemType& item, const int gid, bool secondPass = false);
	bool pruneMap(float threshold, bool onlyBody = false);
	bool updateCount ();
//	void saveMap (ostream& out, bool withGids = true);
//	void loadMap (istream& in, bool withGids = true);
//	void setMinMax (int which, int v);
};

class NewRule {
  public:
	vector<ItemType> body, head;
	map<ItemType, BodyMapElement>::iterator lastBody;
	map<ItemType, MapElement>::iterator lastHead;
	int bodySupp;
	bool satisfy;
	GidList gids;
	NewRule () : satisfy(false) {}
	NewRule (map<ItemType, BodyMapElement>::iterator b, GidList& g) : lastBody(b), satisfy(false), gids(g) {
		body.push_back(b->first);
		lastHead = b->second.heads.begin();
	}
	NewRule (NewRule& r, map<ItemType, BodyMapElement>::iterator b, GidList& g) : body(r.body), lastBody(b), satisfy(r.satisfy), gids(g) {
		body.push_back(b->first);
		lastHead = b->second.heads.begin();
	}
	NewRule (map<ItemType, BodyMapElement>::iterator b, map<ItemType, MapElement>::iterator h, GidList& g, int bs) : lastBody(b), lastHead(h), satisfy(false), gids(g), bodySupp(bs) {
		body.push_back(b->first);
		head.push_back(h->first);
	}
	NewRule (NewRule& r, map<ItemType, BodyMapElement>::iterator b, GidList& g, int bs) : body(r.body), head(r.head), lastBody(b), lastHead(r.lastHead), satisfy(r.satisfy), gids(g), bodySupp(bs) {
		body.push_back(b->first);
	}
	NewRule (NewRule& r, map<ItemType, BodyMapElement>::iterator b, int bs, GidList& g) : body(r.body), head(r.head), lastBody(b), lastHead(r.lastHead), satisfy(r.satisfy), gids(g), bodySupp(bs) {
		head.push_back(b->first);
	}
	NewRule (NewRule& r, map<ItemType, MapElement>::iterator h, GidList& g) : body(r.body), head(r.head), lastBody(r.lastBody), lastHead(h), satisfy(r.satisfy), gids(g), bodySupp(r.bodySupp) {
		head.push_back(h->first);
	}
	friend ostream& operator<<(ostream& out, NewRule r) {
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
		//out << endl;
		return out;
	}
};

class NewRuleSet : public vector<NewRule> {};

class BodyMap : public map<ItemType, BodyMapElement> {
	static int nextid;
	string outfile;
	double totGroups;
	ofstream outR;
	ofstream outHB;
//	vector<GidList> bodySupports;
	vector<int> itemsets;
//	int curBodySupp;
  public:
	int nextID() { return nextid++; }
	BodyMap(string f, double totG) : outfile(f), totGroups(totG) {};
	void openOutputFiles() {
		string filename = outfile + ".r";
		outR.open(filename.c_str());
		string filename1 = outfile + ".hb";
		outHB.open(filename1.c_str());
	}
	void closeOutputFiles() { outR.close(); outHB.close(); }
	void setTotalGroups(double totG) { totGroups = totG; }
	vector<AggregateMonoConstraint *> monoConstr;
	vector<AggregateAntiMonoConstraint *> antiMonoConstr;
	bool checkAntiMono (NewRule& rc);
	bool checkMono (NewRule& rc);
	int add(const int gid, Transaction& t1, Transaction& t2, bool secondPass = false);
//	int add(const int gid, Transaction& t1, bool secondPass = false);
	//int add(ItemType& gid, Transaction& t1);
//	int add(const int gid, ItemType& t1, ItemType& t2, bool secondPass);
	int add(Transaction& t1, const int gid);
//	void saveMap(ostream& out, bool withGids = true);
//	void loadMap(istream& in, bool withGids = true);
	void pruneMap(float threshold, bool onlyBody = false);
	void updateCount ();
	int buildItemset (NewRuleSet& rs, NewRule& rc, vector<BodyMap::iterator>& v, int maxCard, float threshold, int j);
	int generateStartItemSets (NewRuleSet& rs, NewRule& rc, vector<BodyMap::iterator>& v, int maxCard, float threshold, int j);
	int buildRules (NewRuleSet& rs, NewRule& rc, vector<BodyMap::iterator>& vb, vector<BodyMap::iterator>& vh, int maxBodyCard, int maxHeadCard, float threshold, int j);
	vector<BodyMap::iterator> buildHead (NewRuleSet& rs1, NewRule rc, vector<BodyMap::iterator>& v, int maxCard, float threshold, int j);
	void howManyItemsets();
	void saveRules( const NewRuleSet& rs, double bodySupp);
	void saveItemsets( const NewRuleSet& rs, double bodySupp);
	void saveItemset( const NewRule& r, double bodySupp);
};


#endif

#ifndef __BFSWITHGIDSANDCROSS_H__
#define __BFSWITHGIDSANDCROSS_H__

#include "Algorithms/MiningAlgorithmBase.h"
#include "Database/Connection.h"

namespace minerule {

  class BFSWithGidsAndCross : public MiningAlgorithm {

  class Transaction : public std::vector<std::pair<ItemType,ItemType> > {
    HeadBodySourceRowDescription srd;
  public:
    Transaction(HeadBodySourceRowDescription& rowDes) : std::vector<std::pair<ItemType, ItemType> >(), srd(rowDes) {}
    void load(ItemType& gid, odbc::ResultSet *rs) {
      HeadBodySourceRow hbsr(rs,srd);

      while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
	insert(end(),std::pair<ItemType,ItemType>(hbsr.getBody(),hbsr.getHead()));
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
      while (!rs->isAfterLast() &&  gid > hbsr.getGroupBody() ) {
	rs->next();
      
	if(!rs->isAfterLast())
	  hbsr.init(rs,srd);
      }
    
      return !rs->isAfterLast() && gid == hbsr.getGroupBody();
    }
  };


  //class MapElement : public std::set<ItemType> {
  class MapElement : public std::set<int> {
  public:
    int count;
    MapElement() : count(0) {}
  };
  typedef MapElement GidList;


  class BodyMapElement : public MapElement {
  public:
    //pair<iterator, bool> insert(const value_type& x) { return std::set<ItemType>::insert(x); }
    pair<iterator, bool> insert(const value_type& x) { return std::set<int>::insert(x); }
    std::map<ItemType, MapElement > heads;
    void insert(const ItemType& item, MapElement& gidList, bool secondPass = false);
    bool pruneMap (float threshold);
    bool updateCount ();
  };

  class NewRule {
  public:
    std::vector<ItemType> body, head;
    std::map<ItemType, BodyMapElement>::iterator lastBody;
    std::map<ItemType, MapElement>::iterator lastHead;
    double conf;

    GidList gids;
    int bodySupp;

    NewRule (map<ItemType, BodyMapElement>::iterator b, GidList& g) : lastBody(b), gids(g) {
      body.push_back(b->first);
      lastHead = b->second.heads.begin();
    }
    NewRule (NewRule& r, std::map<ItemType, BodyMapElement>::iterator b, GidList& g) : body(r.body), lastBody(b), gids(g) {
      body.push_back(b->first);
      lastHead = b->second.heads.begin();
    }
    NewRule (map<ItemType, BodyMapElement>::iterator b, std::map<ItemType, MapElement>::iterator h, GidList& g, int bs) : lastBody(b), lastHead(h), gids(g), bodySupp(bs) {
      body.push_back(b->first);
      head.push_back(h->first);
    }
    NewRule (NewRule& r, std::map<ItemType, BodyMapElement>::iterator b, GidList& g, int bs) : body(r.body), head(r.head), lastBody(b), lastHead(r.lastHead), gids(g), bodySupp(bs) {
      body.push_back(b->first);
    }
    NewRule (NewRule& r, std::map<ItemType, MapElement>::iterator h, GidList& g) : body(r.body), head(r.head), lastBody(r.lastBody), lastHead(h), gids(g), bodySupp(r.bodySupp) {
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
      out << std::endl;
      return out;
    }
  };

  class NewRuleSet : public std::vector<NewRule> {};

  class BodyMap : public std::map<ItemType, BodyMapElement> {
    sqlCoreConn* coreConn;
    void insertRules( const NewRuleSet& rs, double totGroups );
  public:
    BodyMap(sqlCoreConn& cc) : coreConn(&cc) {};
    //int add(ItemType& gid, Transaction& t1, Transaction& t2, bool secondPass = false);
    //int add(int gid, Transaction& t1, Transaction& t2, bool secondPass = false);
    int add(int gid, Transaction& t1, bool secondPass = false);
    int add(int gid, odbc::ResultSet* rs, HeadBodySourceRow& hbsr, bool secondPass = false);
    //	void saveMap(std::ostream& out, bool withGids = true);
    //	void loadMap(istream& in, bool withGids = true);
    void pruneMap(float threshold);
    void updateCount ();
    void createBodies (NewRuleSet& rs, float threshold, size_t maxBody);
    void createHeads (NewRuleSet& rs, NewRuleSet& rs1, float threshold, size_t maxHead);
    //void generateRules (NewRuleSet& rs, float threshold, int maxBody, int maxHead);
    int generateRules (float support, int totGroups, int maxBody, int maxHead);
    void addHead (NewRuleSet& rs, float threshold, int maxHead, int suppBody, NewRule& rc);

  };
  private:
    AlgorithmsOptions options;
    sqlCoreConn coreConn;
    HeadBodySourceRowDescription rowDes;
    odbc::PreparedStatement* statement;
    odbc::PreparedStatement* stmt1;
	static bool mineruleHasSameBodyHead;


    void insertRules( const NewRuleSet& rs, double totGroups );
    size_t buildAttrStr(const ParsedMinerule::ListType& attr,
			size_t startIndex,
			std::string& attrStr, 
			std::vector<int>& des) const;

    std::string buildQry( const std::string& groupAttrStr,
			  const std::string& attrStr,
			  const std::string& constraints) const;

    void prepareData();
    
  public:
    BFSWithGidsAndCross(const OptimizedMinerule& mr) : 
      MiningAlgorithm(mr), statement(NULL), stmt1(NULL) {}

    virtual ~BFSWithGidsAndCross() {}

    virtual bool needsCrossProductOfSourceTable() const {
      return true;
    }

    virtual bool needsGidSortedSourceTable() const {
      return true;
    }
	
	static bool getMineruleHasSameBodyHead() { 
		return mineruleHasSameBodyHead;
	}

    virtual void execute();

    virtual bool canHandleMinerule() const {
      return 
//	!minerule.getParsedMinerule().hasCrossConditions() &&
	!minerule.getParsedMinerule().requiresClusters() &&
	!minerule.getParsedMinerule().hasDisjuctionsInMC();
    }

  };
}



#endif

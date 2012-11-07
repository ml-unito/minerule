#ifndef __BFSWITHGIDSNOCROSS_H__
#define __BFSWITHGIDSNOCROSS_H__

#include "Algorithms/MiningAlgorithmBase.h"
#include "Algorithms/sqlCoreConn.h"

namespace minerule {

	class BFSWithGidsNoCross : public MiningAlgorithm {

		class Transaction : public std::set<ItemType> {
			HeadBodySourceRowDescription srd;
		public:
			Transaction(HeadBodySourceRowDescription& rowDes) : std::set<ItemType>(), srd(rowDes) {}
			void loadBody(ItemType& gid, odbc::ResultSet *rs) {
				HeadBodySourceRow hbsr(rs,srd);

				while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
					// MRDebug() << "Body:" << hbsr.getBody() << std::endl;
					
					insert(hbsr.getBody());
					rs->next();
					if(!rs->isAfterLast())
						hbsr.init(rs,srd);
				}
			}

			void loadHead(ItemType& gid, odbc::ResultSet *rs) {
				HeadBodySourceRow hbsr(rs,srd);
			
				// MRDebug() << "\n" << std::endl;
				while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
					// MRDebug() << "Head:" << hbsr.getHead() << std::endl;
					// MRDebug() << "rs head:" << rs->getString(2) << std::endl;
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
				map<ItemType, MapElement > heads;
				void insert(const ItemType& item, MapElement& gidList, bool secondPass = false);
				bool pruneMap (float threshold);
				bool updateCount ();
			};

			class NewRule {
			public:
			 std::vector<ItemType> body, head;
				map<ItemType, BodyMapElement>::iterator lastBody;
				map<ItemType, MapElement>::iterator lastHead;
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
				int add(int gid, Transaction& t1, Transaction& t2, bool secondPass = false);
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
			odbc::PreparedStatement* statementBody;
			odbc::PreparedStatement* statementHead;
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
			BFSWithGidsNoCross(const OptimizedMinerule& mr) : 
			MiningAlgorithm(mr), statementBody(NULL), statementHead(NULL) {
				mineruleHasSameBodyHead = mr.getParsedMinerule().hasSameBodyHead();				
			}

			virtual ~BFSWithGidsNoCross() {}

			virtual void execute();
			
			static bool getMineruleHasSameBodyHead() { 
				return mineruleHasSameBodyHead;
			  }

			virtual bool canHandleMinerule() const {
				return 
					!minerule.getParsedMinerule().hasCrossConditions() &&
						!minerule.getParsedMinerule().requiresClusters() &&
							!minerule.getParsedMinerule().hasDisjuctionsInMC();
			}

		};
	}



#endif

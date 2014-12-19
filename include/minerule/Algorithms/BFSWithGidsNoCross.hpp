//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Marco Botta (botta@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef __BFSWITHGIDSNOCROSS_H__
#define __BFSWITHGIDSNOCROSS_H__

#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Database/Connection.hpp"
#include "minerule/Database/SourceTable.hpp"
#include "minerule/Database/Transaction.hpp"

namespace minerule {

	class BFSWithGidsNoCross : public MiningAlgorithm {

		typedef ItemTransaction< std::set<ItemType> > Transaction;

		class MapElement : public std::set<int> {
		public:
			int count;
			MapElement() : count(0) {}
		};

		typedef MapElement GidList;

		class BodyMapElement : public MapElement {
		public:
    //pair<iterator, bool> insert(const value_type& x) { return std::set<ItemType>::insert(x); }
			std::pair<iterator, bool> insert(const value_type& x) { return std::set<int>::insert(x); }
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

			NewRule (std::map<ItemType, BodyMapElement>::iterator b, GidList& g) : lastBody(b), gids(g) {
				body.push_back(b->first);
				lastHead = b->second.heads.begin();
			}
			NewRule (NewRule& r, std::map<ItemType, BodyMapElement>::iterator b, GidList& g) : body(r.body), lastBody(b), gids(g) {
				body.push_back(b->first);
				lastHead = b->second.heads.begin();
			}
			NewRule (std::map<ItemType, BodyMapElement>::iterator b, std::map<ItemType, MapElement>::iterator h, GidList& g, int bs) : lastBody(b), lastHead(h), gids(g), bodySupp(bs) {
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
			Connection* connection;
			Progress* progress;
			void insertRules( const NewRuleSet& rs, double totGroups );
		public:
			BodyMap(Connection& cc, Progress& prog) : connection(&cc), progress(&prog) {};

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
		static bool mineruleHasSameBodyHead;
		SourceTable* sourceTable;

		SourceTable::Iterator bodyIterator;
		SourceTable::Iterator headIterator;

		void insertRules( const NewRuleSet& rs, double totGroups );

		void prepareData();
	public:
		BFSWithGidsNoCross(const OptimizedMinerule& mr) : MiningAlgorithm(mr), sourceTable(NULL) {
			mineruleHasSameBodyHead = mr.getParsedMinerule().hasSameBodyHead();
		}

		virtual ~BFSWithGidsNoCross() {
			if(sourceTable!=NULL) delete sourceTable;
		}

		virtual SourceTableRequirements sourceTableRequirements() const {
			return SourceTableRequirements(SourceTableRequirements::SortedGids);
		};


		virtual void mineRules();

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

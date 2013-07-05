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
// CORE.CPP

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>
#include <iterator>
#include <algorithm>


#include "minerule/Algorithms/AlgorithmsOptions.h"
#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Database/MRResultSet.h"
#include "minerule/Database/ItemType.h"
#include "minerule/Utils/Converter.h"
#include "minerule/Database/Connection.h"
#include "minerule/Algorithms/BFSWithGidsAndCross.h"
#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.h"
#include "minerule/Database/PrepareDataUtils.h"

namespace minerule {

	bool BFSWithGidsAndCross::mineruleHasSameBodyHead = false;	

	void BFSWithGidsAndCross::BodyMapElement::insert(const ItemType& item, MapElement& gidList, bool secondPass) {
		std::map<ItemType, MapElement>::iterator found = heads.find(item);
		if (found == heads.end()) {
			if (!secondPass) heads[item] = gidList;
			} else found->second.insert(gidList.begin(),gidList.end());
		}

		bool BFSWithGidsAndCross::BodyMapElement::pruneMap (float threshold) {
			std::map<ItemType, MapElement> newMap;
			for (std::map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
				if (i->second.count >= threshold) {i->second.count = 0; newMap[i->first] = i->second;}
			heads = newMap;
			return heads.size() > 0;
		}

		bool BFSWithGidsAndCross::BodyMapElement::updateCount () {
			for (std::map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
				i->second.count += i->second.size();
			return heads.size() > 0;
		}


//  int BFSWithGidsAndCross::BodyMap::add(ItemType& gid, Transaction& t1, Transaction& t2, bool secondPass) {
		int BFSWithGidsAndCross::BodyMap::add(int gid, Transaction& t1, bool secondPass) {
			int howMany = 0;
			MapElement me;
			me.insert(gid);
			for (Transaction::iterator i = t1.begin(); i != t1.end(); i++) {
				std::map<ItemType, BodyMapElement>::iterator found = find(i->first);
				if (found == end()) {
					if (secondPass) continue;
					BodyMapElement bme;
					(*this)[i->first] = bme;
					found = find(i->first);
				}
				found->second.insert(gid);
				found->second.insert(i->second,me,secondPass);
				howMany++;
			}
			return howMany;
		}

		int BFSWithGidsAndCross::BodyMap::add(int gid, odbc::ResultSet* rs, SourceRow& hbsr, bool secondPass) {
			int howMany = 0;
			MapElement me;
			me.insert(gid);
			ItemType body = hbsr.getBody();
			std::map<ItemType, BodyMapElement>::iterator found = find(body);
			if (found == end()) {
				if (secondPass) return 0;
				BodyMapElement bme;
				(*this)[body] = bme;
				found = find(body);
			}
			found->second.insert(gid);
			ItemType head = hbsr.getHead();
			found->second.insert(head,me,secondPass);
			howMany++;
			return howMany;
		}

  /*void BodyMap::saveMap (std::ostream& out, bool withGids) {
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
	void BFSWithGidsAndCross::BodyMap::pruneMap (float threshold) {
		BodyMap newMap(*connection);
		for (iterator i = begin(); i != end(); i++)
			if (i->second.pruneMap(threshold)) newMap[i->first] = i->second;
		(*this) = newMap;
	}

	void BFSWithGidsAndCross::BodyMap::updateCount () {
		for (iterator i = begin(); i != end(); i++)
			i->second.updateCount();
	}


	// void BFSWithGidsAndCross::BodyMap::createBodies (NewRuleSet& rs, float threshold, size_t maxBody) {
	// 	for (iterator i = begin(); i != end(); i++) {
	// 		NewRule r(i,i->second);
	// 		rs.insert(rs.end(),r);
	// 	}
	// 	for (unsigned int n = 0; n < rs.size(); n++) {
	// 		NewRuleSet::iterator i = rs.begin()+n;
	// 		NewRule& rc = *i;
	// 		if (rc.body.size() < maxBody) {
	// 			BodyMap::iterator lb = rc.lastBody;
	// 			for (BodyMap::iterator j = ++lb; j != end(); j++) {
	// 				GidList newGidList;
	// 				NewRule& rc2 = *(rs.begin()+n);
	// 				set_intersection(rc2.gids.begin(),rc2.gids.end(),
	// 					j->second.begin(), j->second.end(),
	// 						inserter(newGidList,newGidList.begin()));
	// 				if (newGidList.size() >= threshold) {
	// 					NewRule r(rc2,j,newGidList);
	// 					rs.insert(rs.end(),r);
	// 				}
	// 			}
	// 		}
	// 	}
	// }


	// void BFSWithGidsAndCross::BodyMap::createHeads (NewRuleSet& rs, NewRuleSet& rs1, float threshold, size_t maxHead) {
	// 		for (size_t n = 0; n < rs.size(); n++) {
	// 			NewRule& rc = rs[n];
	// 			if (rc.head.size() < maxHead) {
	// 				map<ItemType, MapElement>::iterator lh = rc.lastHead;
	// 				map<ItemType, MapElement>::iterator eh = rc.headTable.end();
	// 				double supportBody = rc.gids.size();
	// 
	// 				for (map<ItemType, MapElement>::iterator j = lh; j != eh; j++) {
	// 					GidList newGidList;
	// 					if (std::find(rc.body.begin(), rc.body.end(), j->first) == rc.body.end()/* &&
	// 					rc.head.find(j->first) == rc.head.end()*/) {
	// 						set_intersection(rc.gids.begin(),rc.gids.end(),
	// 							j->second.begin(), j->second.end(),
	// 								inserter(newGidList,newGidList.begin()));
	// 						if (newGidList.size() >= threshold) {
	// 							NewRule r(rc,j,newGidList);
	// 							r.lastHead++;
	// 							r.conf = newGidList.size() / supportBody;
	// 							rs1.insert(rs1.end(),r);
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}

	bool BFSWithGidsAndCross::BodyMap::checkHeads (NewRule& r, std::map<ItemType, BodyMapElement>::iterator b, GidList& g, float threshold) {
		bool ok = true;
		for (std::vector<ItemType>::iterator i = r.head.begin(); i != r.head.end() && ok; i++) {
			std::map<ItemType, MapElement >::iterator found = b->second.heads.find(*i);
			if (found == b->second.heads.end()) return false;
			else {
				GidList newGidList;
				set_intersection((*found).second.begin(),(*found).second.end(),
					g.begin(), g.end(),
						inserter(newGidList,newGidList.begin()));
				g = newGidList;
				ok = newGidList.size() >= threshold;
			}
		}
		return ok;
	}

	int BFSWithGidsAndCross::BodyMap::generateRules (float support, int totGroups, int maxBody, int maxHead) {
		NewRuleSet rs1;
		int howManyRules = 0;
		float threshold = support*totGroups;
		for (iterator i = begin(); i != end(); i++) {
			NewRule r(i,i->second);
			rs1.insert(rs1.end(),r);
		}
		for (size_t n = 0; n < rs1.size(); n++) {
			NewRule& rc = *(rs1.begin()+n);
			NewRuleSet rs;
			addHead(rs, threshold, maxHead, rc.gids.size(), rc);
			howManyRules += rs.size();
			insertRules(rs, totGroups);
			if (rc.body.size() < (size_t)maxBody) {
				BodyMap::iterator lb = rc.lastBody;
				for (BodyMap::iterator j = ++lb; j != end(); j++) {
					NewRule& rc2 = *(rs1.begin()+n);
					GidList newGidList;
					set_intersection(rc2.gids.begin(),rc2.gids.end(),
						j->second.begin(), j->second.end(),
							inserter(newGidList,newGidList.begin()));
					if (newGidList.size() >= threshold) {
						NewRule r(rc2,j,newGidList);
						rs1.insert(rs1.end(),r);
					}
				}
			}
		}
		return howManyRules;
	}

	void BFSWithGidsAndCross::BodyMap::addHead (NewRuleSet& rs, float threshold, int maxHead, int suppBody, NewRule& rc) {
		if (rc.head.size() < (size_t)maxHead) {
			std::map<ItemType, MapElement>::iterator lh = rc.lastHead;
			std::map<ItemType, MapElement>::iterator eh = rc.headTable.end();
			for (map<ItemType, MapElement>::iterator j = lh; j != eh; j++) {
				GidList newGidList;
				if (!BFSWithGidsAndCross::getMineruleHasSameBodyHead() || std::find(rc.body.begin(), rc.body.end(), j->first) == rc.body.end()) {
					
					set_intersection(	rc.gids.begin(),rc.gids.end(),
										j->second.begin(), j->second.end(),
										inserter(newGidList,newGidList.begin()));
					
					if (newGidList.size() >= threshold) {
						NewRule r(rc,j,newGidList);
						r.lastHead++;
						r.conf = newGidList.size() / (double)suppBody;
						rs.insert(rs.end(),r);
						addHead(rs,threshold,maxHead,suppBody,r);
					}
				}
			}
		}
	}
	
	
	 std::map<ItemType, BFSWithGidsAndCross::MapElement> BFSWithGidsAndCross::NewRule::hash_intersection( std::map<ItemType, MapElement>& lhs, const std::map<ItemType, MapElement>& rhs ) {
		std::map<ItemType, MapElement> result;
		
		std::map<ItemType, MapElement>::iterator it;		
		for( it=lhs.begin(); it!=lhs.end(); ++it ) {
			std::map<ItemType, MapElement>::const_iterator found = rhs.find(it->first);
			if( found != rhs.end() ) {				
				// GIDS INTERSECTION
				GidList newGidList;
				set_intersection(it->second.begin(),it->second.end(),
					found->second.begin(), found->second.end(),
						inserter(newGidList,newGidList.begin()));				
				result[it->first] = newGidList;
			}
		}
		
		return result;
	}


	void BFSWithGidsAndCross::BodyMap::insertRules( const NewRuleSet& rs, double totGroups ) {
		NewRuleSet::const_iterator it;
		for(it=rs.begin(); it!=rs.end(); it++) {
			connection->insert( it->body, it->head, it->gids.size()/totGroups, it->conf );
		}
	}


	void BFSWithGidsAndCross::prepareData() {
		sourceTable = new SourceTable(*this);
		ruleIterator = sourceTable->newIterator(SourceTable::FullIterator);

		options.setTotGroups(sourceTable->getTotGroups());
	}


	void BFSWithGidsAndCross::mineRules() {
		MRLogPush("Starting BFSWithGidsAndCross mining algorithm...");
    
		MRLog() << "Preparing data sources..." << std::endl;
		prepareData();


		float support = options.getSupport();
		int maxBody = options.getBodyCardinalities().getMax();
		int maxHead = options.getHeadCardinalities().getMax();

		ItemType gid1;
		BodyMap bodyMap(connection);

		int totalGroups = options.getTotGroups();
		int howManyRows = 0;
		int howManyGroups = 0;


		MRLogPush("Reading data...");

		while (!ruleIterator.isAfterLast()) {
			gid1 = ruleIterator->getGroup();
			howManyGroups++;
		
			Transaction t1;
		
			t1.load(gid1,ruleIterator);
			howManyRows += bodyMap.add(howManyGroups,t1);
		}

		MRLog() << "Total groups: " << totalGroups << std::endl;
		MRLogPop();

		MRLogPush("Starting rule extraction...");

		bodyMap.updateCount();
		MRLog() << "Total bodies before pruning: " << bodyMap.size() << std::endl;
		bodyMap.pruneMap(support*totalGroups);
		MRLog() << "Total bodies after pruning: " << bodyMap.size() << std::endl;
		NewRuleSet rs;

		int nrules = bodyMap.generateRules(support,totalGroups,maxBody,maxHead);
		MRLog() << "After extracting rules, rules: " << nrules << std::endl;

		MRLogPop();
		connection.finalize();

		MRLogPop();
	}


} // namespace

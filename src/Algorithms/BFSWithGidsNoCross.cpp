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


#include "minerule/Algorithms/AlgorithmsOptions.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/Database/MRResultSet.hpp"
#include "minerule/Database/ItemType.hpp"
#include "minerule/Utils/Converter.hpp"
#include "minerule/Database/Connection.hpp"
#include "minerule/Algorithms/BFSWithGidsNoCross.hpp"
#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"
#include "minerule/Database/Transaction.hpp"

namespace minerule {
  bool BFSWithGidsNoCross::mineruleHasSameBodyHead = false;	

  void BFSWithGidsNoCross::BodyMapElement::insert(const ItemType& item, MapElement& gidList, bool secondPass) {
    std::map<ItemType, MapElement>::iterator found = heads.find(item);
    if (found == heads.end()) {
      if (!secondPass) heads[item] = gidList;
    } else found->second.insert(gidList.begin(),gidList.end());
  }

  bool BFSWithGidsNoCross::BodyMapElement::pruneMap (float threshold) {
    std::map<ItemType, MapElement> newMap;
    for (std::map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++) {
      if (i->second.count >= threshold) {
		  i->second.count = 0; 
		  newMap[i->first] = i->second;
	  }
	}
    heads = newMap;
    return heads.size() > 0;
  }

  bool BFSWithGidsNoCross::BodyMapElement::updateCount () {
    for (std::map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++) { 
      i->second.count += i->second.size();
  	}
    return heads.size() > 0;
  }


  int BFSWithGidsNoCross::BodyMap::add(int gid, Transaction& t1, Transaction& t2, bool secondPass) {	  	  
	  int howMany = 0;
	  MapElement me;
	  me.insert(gid);
	  for (Transaction::iterator i = t1.begin(); i != t1.end(); i++) {
		  std::map<ItemType, BodyMapElement>::iterator found = find(*i);
		  if (found == end()) {
			  if (secondPass) continue;
			  BodyMapElement bme;
			  (*this)[*i] = bme;
			  found = find(*i);
		  }
		  found->second.insert(gid);
		  for (Transaction::iterator j = t2.begin(); j != t2.end(); j++)
		  if (!BFSWithGidsNoCross::getMineruleHasSameBodyHead() || *i != *j /*&& i->price < j->price*/) {
			  howMany++;
			  found->second.insert(*j,me,secondPass);
		  }
	  }
	  return howMany;
  }

  void BFSWithGidsNoCross::BodyMap::pruneMap (float threshold) {
	  BodyMap newMap(*connection);
	  for (iterator i = begin(); i != end(); i++) {		  
		  if (i->second.pruneMap(threshold)) 
			  newMap[i->first] = i->second;
	  }
	  
	  (*this) = newMap;
  }

  void BFSWithGidsNoCross::BodyMap::updateCount () {
    for (iterator i = begin(); i != end(); i++)
      i->second.updateCount();
  }

  void BFSWithGidsNoCross::BodyMap::createBodies (NewRuleSet& rs, float threshold, size_t maxBody) {
	  for (iterator i = begin(); i != end(); i++) {
		  NewRule r(i,i->second);
		  rs.insert(rs.end(),r);
	  }
	
	  for (unsigned int n = 0; n < rs.size(); n++) {
		  NewRuleSet::iterator i = rs.begin()+n;
		  NewRule& rc = *i;
		  
		  if (rc.body.size() < maxBody) {
			  
			  BodyMap::iterator lb = rc.lastBody;
			  for (BodyMap::iterator j = ++lb; j != end(); j++) {
				  GidList newGidList;
				  NewRule& rc2 = *(rs.begin()+n);
				  
				  set_intersection(rc2.gids.begin(),rc2.gids.end(),
					  j->second.begin(), j->second.end(),inserter(newGidList,newGidList.begin()));
				  
				  if (newGidList.size() >= threshold) {
					  NewRule r(rc2,j,newGidList);
					  rs.insert(rs.end(),r);
				  }
			  }
			  
		  } // if(rc.body.size())
		  
	  } // for
  }


  void BFSWithGidsNoCross::BodyMap::createHeads (NewRuleSet& rs, NewRuleSet& rs1, float threshold, size_t maxHead) {
	  for (size_t n = 0; n < rs.size(); n++) {
		  NewRule& rc = rs[n];
		  if (rc.head.size() < maxHead) {
			  map<ItemType, MapElement>::iterator lh = rc.lastHead;
			  map<ItemType, MapElement>::iterator eh = rc.lastBody->second.heads.end();
			  double supportBody = rc.gids.size();

			  for (map<ItemType, MapElement>::iterator j = lh; j != eh; j++) {
				  GidList newGidList;
				  if (!BFSWithGidsNoCross::getMineruleHasSameBodyHead() || std::find(rc.body.begin(), rc.body.end(), j->first) == rc.body.end()) {
					  
					  set_intersection(rc.gids.begin(),rc.gids.end(),
						  j->second.begin(), j->second.end(), inserter(newGidList,newGidList.begin()));
					  
					  if (newGidList.size() >= threshold) {
						  NewRule r(rc,j,newGidList);
						  r.lastHead++;
						  r.conf = newGidList.size() / supportBody;
						  rs1.insert(rs1.end(),r);
					  }
				  }
			  } // for
		  } // if
	  } // for
  }

  int BFSWithGidsNoCross::BodyMap::generateRules (float support, int totGroups, int maxBody, int maxHead) {
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

  void BFSWithGidsNoCross::BodyMap::addHead (NewRuleSet& rs, float threshold, int maxHead, int suppBody, NewRule& rc) {	  
    if (rc.head.size() < (size_t)maxHead) {
      std::map<ItemType, MapElement>::iterator lh = rc.lastHead;
      std::map<ItemType, MapElement>::iterator eh = rc.lastBody->second.heads.end();
      for (map<ItemType, MapElement>::iterator j = lh; j != eh; j++) {
		GidList newGidList;

		if (!BFSWithGidsNoCross::getMineruleHasSameBodyHead() || 
			 std::find(rc.body.begin(), rc.body.end(), j->first) == rc.body.end()
			 /* && rc.head.find(j->first) == rc.head.end()*/) {
		  set_intersection(rc.gids.begin(),rc.gids.end(),
				   j->second.begin(), j->second.end(),
				   inserter(newGidList,newGidList.begin()));
		  
		  if (newGidList.size() >= threshold) {
		    NewRule r(rc,j,newGidList);
		    r.lastHead++;
		    r.conf = newGidList.size() / (double)suppBody;
		    rs.insert(rs.end(),r);
		    addHead(rs,threshold,maxHead,suppBody,r);						
		  }
		} // if (std::find
      } // for( map...
    } // if (rc.head.size...
  }

  void BFSWithGidsNoCross::BodyMap::insertRules( const NewRuleSet& rs, 
			     double totGroups ) {
    NewRuleSet::const_iterator it;
    for(it=rs.begin(); it!=rs.end(); it++) {
      connection->insert( it->body, it->head, it->gids.size()/totGroups, it->conf );
    }
  }


  void BFSWithGidsNoCross::prepareData() {	  
	  sourceTable = new SourceTable(*this);
	  bodyIterator = sourceTable->newIterator(SourceTable::BodyIterator);
	  headIterator = sourceTable->newIterator(SourceTable::HeadIterator);

	  options.setTotGroups(sourceTable->getTotGroups());
  }


  void BFSWithGidsNoCross::mineRules() {
    MRLogPush("Starting BFSWithGidsNoCross mining algorithm...");
    
    MRLog() << "Preparing data sources..." << std::endl;
    prepareData();

    float support = options.getSupport();
    int maxBody = options.getBodyCardinalities().getMax();
    int maxHead = options.getHeadCardinalities().getMax();

    ItemType gid1;
	
    if(bodyIterator.isAfterLast())
		throw new MineruleException(MR_ERROR_INTERNAL,"Cannot find initial GID for body elements");
    if(headIterator.isAfterLast())
		throw new MineruleException(MR_ERROR_INTERNAL,"Cannot find initial GID for head elements");
	
	MRLogPush("Reading data");
    BodyMap bodyMap(connection);

    int totalGroups = options.getTotGroups();
    int howManyRows = 0;
    int howManyGroups = 0;

    while (!bodyIterator.isAfterLast()) {		
      ItemType gid = bodyIterator->getGroup();
      
	  Transaction t1, t2;
      t1.loadBody(gid,bodyIterator);

      bool found2 = t2.findGid(gid,headIterator);
      if (found2) {
		  t2.loadHead(gid,headIterator);
      }

      howManyRows += bodyMap.add(howManyGroups,t1,t2);
      howManyGroups++;
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

    MRLogPop();

    connection.finalize();
  }


} // namespace

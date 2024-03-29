//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Arianna Gallo (gallo.arianna@gmail.com)
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
// care.cpp


#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"
#include "minerule/Algorithms/ConstrItemSetsExtraction.hpp"

#define MROPTIONFILE "mroptions"

namespace minerule {
  void ConstrItemSetsExtraction::prepareData() {
    MineruleOptions& mrOptions = MineruleOptions::getSharedOptions();

    options.setSupport( minerule.getParsedMinerule().sup );
    options.setConfidence( minerule.getParsedMinerule().conf );

    MinMaxPair bodyCards(minerule.getParsedMinerule().bodyCardinalities);
    bodyCards.applyConstraints(mrOptions.getParsers().getBodyCardinalities());
    options.setBodyCardinalities(bodyCards);

    MinMaxPair headCards(minerule.getParsedMinerule().headCardinalities);
    headCards.applyConstraints(mrOptions.getParsers().getHeadCardinalities());
    options.setHeadCardinalities(headCards);

    sourceTable = new SourceTable(*this);
	  bodyIterator = sourceTable->newIterator(SourceTable::BodyIterator);

    MRLog() << "Building db queries" << std::endl;
    MRLog() << "Executing queries" << std::endl;

    connection.useMRDBConnection(MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection());
    connection.setOutTableName(minerule.getParsedMinerule().tab_result);
    connection.setBodyCardinalities(minerule.getParsedMinerule().bodyCardinalities);
    //connection.setHeadCardinalities(minerule.getParsedMinerule().headCardinalities);
    connection.setHeadCardinalities(MinMaxPair(0,1000));
    connection.createResultTables(SourceRowMetaInfo(connection.getMRDBConnection(), minerule.getParsedMinerule()));
    connection.init();
  }

  void ConstrItemSetsExtraction::mineRules() {
    MRLogPush("Starting ConstrItemSetsExtraction mining algorithm...");

    MRLog() << "Preparing data sources..." << std::endl;
    prepareData();

    float support = options.getSupport();
    int maxBody = options.getBodyCardinalities().getMax();

    ItemType gid1;
    BodyMap bodyMap(connection,1);

    int howManyGroups = 0;
    int totalGroups = sourceTable->getTotGroups();
    int howManyRows = 0;

    while (!bodyIterator.isAfterLast()) {
		ItemType gid = bodyIterator->getGroup();
		Transaction t1, t2;

		t1.loadBody(gid,bodyIterator);
		howManyRows += bodyMap.add(t1,howManyGroups);
		howManyGroups++;
    }
    MRLog() << "Total rows: " << howManyRows << std::endl;
    MRLog() << "Total groups: " << totalGroups << std::endl;
    MRLogPop();

    MRLogPush("Starting itemset extraction...");

    bodyMap.updateCount();
    MRLog() << "Total bodies before pruning: " << bodyMap.size() << std::endl;
    bodyMap.pruneMap(support*totalGroups,true);
    bodyMap.setTotalGroups(totalGroups);
    std::vector<BodyMap::iterator> v;
    std::multimap<int, BodyMap::iterator> temp;
    for (BodyMap::iterator i = bodyMap.begin(); i!=bodyMap.end(); i++)
       	if (!i->second.done) temp.insert(std::pair<int, BodyMap::iterator>(i->second.count(),i));
    for (std::multimap<int, BodyMap::iterator>::iterator i = temp.begin(); i!=temp.end(); i++)
        v.insert(v.end(),i->second);
    MRLog() << "Total bodies after pruning: " << v.size() << std::endl;

    NewRule r;
    NewRuleSet rs;
    int nrules = bodyMap.generateStartItemSets(rs,r,v,maxBody,support*totalGroups,-1);
    MRLog() << "After extracting itemsets: " << nrules << std::endl;
    MRLogPop();

    connection.finalize();
  }

} //end namespace

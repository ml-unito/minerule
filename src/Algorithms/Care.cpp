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

// care.cpp

#include "minerule/Algorithms/Care.hpp"

#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"

#define MROPTIONFILE "mroptions"

namespace minerule {

  // Preprocessing functions

	size_t CARE::buildAttrStr(const ParsedMinerule::AttrVector& attr,
		size_t startIndex,
			std::string& attrStr,
	std::vector<int>& des) const {
		ParsedMinerule::AttrVector::const_iterator it = attr.begin();
		for( ; it!=attr.end(); it++ ) {
			if(it!=attr.begin()) {
				attrStr+=",";
			}

			attrStr+=*it;
			des.push_back(++startIndex);
		}

		return startIndex;
	}

	std::string CARE::buildQry( const std::string& groupAttrStr,
		const std::string& attrStr,
	const std::string& constraints) const {

		return std::string("SELECT "+groupAttrStr+","+attrStr+" "
			"FROM "+minerule.getParsedMinerule().tab_source+" "+
				(constraints.size()>0 ?
					"WHERE "+constraints+" " :
		"")
			+"ORDER BY "+groupAttrStr+","+attrStr);
	}

	void CARE::prepareData() {
		sourceTable = new SourceTable(*this);
//		options.setTotGroups(sourceTable->getTotGroups());

		bodyIterator = sourceTable->newIterator(SourceTable::BodyIterator);
		headIterator = sourceTable->newIterator(SourceTable::HeadIterator);
	}

	void CARE::mineRules() {
		MRLogPush("Starting CARE mining algorithm...");

		MRLog() << "Preparing data sources..." << std::endl;
		prepareData();

		double support = options.getSupport();
		int maxBody = options.getBodyCardinalities().getMax();
		int maxHead = options.getHeadCardinalities().getMax();

		ItemType gid1;

		BodyMap bodyMap(connection,1);
		BodyMap headMap(connection,1);

		int howManyGroups = 0;
		int totalGroups = sourceTable->getTotGroups();
		int howManyRows = 0;

		while (!bodyIterator.isAfterLast()) {
			ItemType gid = bodyIterator->getGroup();
			Transaction t1, t2;

			t1.loadBody(gid,bodyIterator);
			bool found2 = t2.findGid(gid,headIterator);
			if (found2) {
				t2.loadHead(gid,headIterator);
			}
			howManyRows += bodyMap.add(t1,howManyGroups);
			howManyRows += headMap.add(t2,howManyGroups);
			howManyGroups++;
		}

		MRLog() << "Total rows: " << howManyRows << std::endl;
		MRLog() << "Total groups: " << totalGroups << std::endl;
		MRLogPop();

		MRLogPush("Starting rule extraction...");

		bodyMap.updateCount();
		headMap.updateCount();
		MRLog() << "Total bodies before pruning: " << bodyMap.size() << std::endl;
  //  MRLogPop();
//    MRLogPush("Starting pruning ...");
		bodyMap.pruneMap(support*totalGroups,true);
		headMap.pruneMap(support*totalGroups,true);
//    MRLogPop();
		bodyMap.setTotalGroups(totalGroups);
//  MRLogPush("Starting rule extraction ...");
		std::vector<BodyMap::iterator> v;
		std::vector<BodyMap::iterator> v1;
		std::multimap<int, BodyMap::iterator> temp;
		for (BodyMap::iterator i = bodyMap.begin(); i!=bodyMap.end(); i++)
			if (!i->second.done) temp.insert(std::pair<int, BodyMap::iterator>(i->second.count(),i));
		for (std::multimap<int, BodyMap::iterator>::iterator i = temp.begin(); i!=temp.end(); i++)
			v.insert(v.end(),i->second);
		for (BodyMap::iterator i = headMap.begin(); i!=headMap.end(); i++)
			v1.insert(v1.end(),i);
		MRLog() << "Total bodies after pruning: " << v.size() << std::endl;

		NewRule r;
		NewRuleSet rs1;
    //bodyMap.openOutputFiles();
		int nrules = bodyMap.buildRules(rs1,r,v,v1,maxBody,maxHead,support*totalGroups,-1);
    //bodyMap.closeOutputFiles();
		MRLog() << "After extracting rules: " << nrules << std::endl;
		MRLogPop();
		connection.finalize();
	}

} //end namespace

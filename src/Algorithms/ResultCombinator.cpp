//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
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
#include "minerule/Algorithms/ResultCombinator.hpp"
#include "minerule/Result/QueryResult.hpp"

namespace minerule {

	void 
	ResultCombinator::execute() {
		MRLogPusher log1("ResultCombinator incremental algorithm starting...");

		if( minerule==NULL )
			throw MineruleException(MR_ERROR_INTERNAL, 
				"ResultCombinator::execute called, but the "
				"optimized minerule is NULL");

		const std::vector<ParsedMinerule>& minerules=
			minerule->getOptimizationInfo().minerulesToCombine;
		const GAQueryCombinator::QueryOrList& formula = 
			minerule->getOptimizationInfo().combinationFormula;

		QueryResult::ResultSet<QueryResult::SortBodyHead> result;

		GAQueryCombinator::QueryOrList::const_iterator OrIt;
		for( OrIt=formula.begin();
		OrIt!=formula.end();
		OrIt++) {
			MRLogPusher log2("Analyzing disjunct");
			if(OrIt!=formula.begin()) {
				MRLog()<<"Analyzing next conjunct"<<std::endl;
			}

			GAQueryCombinator::QueryAndList::const_iterator AndIt =
				OrIt->begin();

			QueryResult::ResultSet<QueryResult::SortBodyHead> curConj;
      
			MRLogPush("Loading first query from disk");
			if( AndIt!=OrIt->end() ) {
				curConj.load( minerules[*AndIt].tab_result );
				AndIt++;
			}
			MRLogPop();

			MRLog() << "Current conjunct current result set size:" 
				<< curConj.size() << std::endl;

			MRLog() << "Intersecting with other queries:" << std::endl;
			for( ;  AndIt!=OrIt->end();  AndIt++) {
				MRLogPusher log3("Analyzing conjunct");

				MRLogPush("Loading query from disk.");
				QueryResult::ResultSet<QueryResult::SortBodyHead>  q;
				q.load( minerules[*AndIt].tab_result );
				MRLogPop();

				MRLogPush("Intersecting...");
				curConj.inplace_intersect(q);
				MRLogPop();

				MRLog() << "Current conjunct current result set size:" 
					<< curConj.size() << std::endl;
			}

			MRLogPush("Merging...");
			result.inplace_union(curConj);
			MRLogPop();

			MRLog() << "Current result set size:"  << result.size() << std::endl;
		}
    
		result.save(minerule->getParsedMinerule());
	}


} // namespace

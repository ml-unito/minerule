#include "Algorithms/ResultCombinator.h"
#include "Result/QueryResult.h"

namespace minerule {

	void 
	ResultCombinator::execute() throw(MineruleException,odbc::SQLException) {
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

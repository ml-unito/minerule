#include "Matcher.h"
#include "RuleGidsMatcher.h"
#include "GidRulesMatcher.h"
#include "Utils/MineruleErrors.h"

namespace mrmatch {
	Matcher* Matcher::newMatcher(MatchKind kind) {
		switch(kind) {
			case RuleGids: return new RuleGidsMatcher();
			case GidRules: return new GidRulesMatcher();
			default: throw MineruleException( MR_ERROR_INTERNAL, "Unknown or unsupported matcher kind");
		}
	}

	void Matcher::matchWithCrossProduct(SourceTable& st) {
		SourceTable::Iterator it = st.newIterator(SourceTable::FullIterator);
		
		while(!it.isAfterLast()) {
			ItemType gid = it->getGroup();
			
			RuleTransaction<RulesMatcher::RuleSetType> transaction;
			transaction.load(gid, it);
			
			matchRuleTransaction(gid, transaction);			
		}		
	}
		
	void Matcher::matchWithoutCrossProduct(SourceTable& st) {
		SourceTable::Iterator bodyIt = st.newIterator(SourceTable::BodyIterator);
		SourceTable::Iterator headIt = st.newIterator(SourceTable::HeadIterator);
				
		while(!bodyIt.isAfterLast()) {
			ItemType gid = bodyIt->getGroup();
			
			ItemTransaction<RulesMatcher::ItemSetType> bodies;
			ItemTransaction<RulesMatcher::ItemSetType> heads;
			
			bodies.loadBody(gid, bodyIt); 			// this advances the body iterator
						
			if( !TransactionBase<RulesMatcher::ItemSetType>::findGid(gid, headIt) ) {// positioning the head iterator  
				break;								// no more heads to load
			}

			heads.loadHead(gid, headIt);			// loading the heads

			matchItemTransaction(gid, bodies, heads);			
		} // while
		
	} // matchWithoutCrossProduct		

}
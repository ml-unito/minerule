#include "RuleGidsMatcher.h"
#include "Database/Transaction.h"
#include "Result/RulesMatcher.h"
#include "Result/RuleFormatter.h"

namespace mrmatch {
	
	// create a new empty rule and returns it (so that it can be intialized)
	Rule& RuleGidsMatcher::addRule() {
		_ruleGidsVector.push_back( RuleGids() );
		return _ruleGidsVector.back().first;
	}
	
	void RuleGidsMatcher::matchWithCrossProduct(SourceTable& st) {
		SourceTable::Iterator it = st.newIterator(SourceTable::FullIterator);
		
		while(!it.isAfterLast()) {
			ItemType gid = it->getGroup();
			
			RuleTransaction<RulesMatcher::RuleSetType> transaction;
			transaction.load(gid, it);
			
			for(RuleGidsVector::iterator ruleIt=_ruleGidsVector.begin(); ruleIt!=_ruleGidsVector.end(); ++ruleIt) {
				if( RulesMatcher::match(ruleIt->first, transaction) ) {
					ruleIt->second.push_back( gid );
				}
			}
		}		
	}
		
	void RuleGidsMatcher::matchWithoutCrossProduct(SourceTable& st) {
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
			
			// populating results
			for(RuleGidsVector::iterator ruleIt = _ruleGidsVector.begin(); ruleIt!=_ruleGidsVector.end(); ++ruleIt) {				
				if( RulesMatcher::match( ruleIt->first, bodies, heads ) ) {
					ruleIt->second.push_back( gid );
				}
			}
			
		} // while
		
	} // matchWithoutCrossProduct
	
	
	std::string RuleGidsMatcher::formatGids( const Gids& gids ) const {
		std::stringstream str;
		for(std::vector<ItemType>::const_iterator it = gids.begin(); it!=gids.end(); ++it) {
			str << *it << " ";
		}
		
		return StringUtils::toBold(StringUtils::toGreen(str.str()));
	}
	
	void RuleGidsMatcher::printMatches( ) const {
		SimpleRuleFormatter sf;
		sf.setFieldWidths( SimpleRuleFormatter::FieldWidths(1,1,1,1) );
		for(RuleGidsVector::const_iterator it=_ruleGidsVector.begin(); it!=_ruleGidsVector.end(); ++it) {
			MRLog() 	<< StringUtils::toBold("Rule: ") << sf.formatRule(it->first) << " "
						<< StringUtils::toBold("\tGids: ") << formatGids(it->second) << std::endl;
		}
	}
	
	

}
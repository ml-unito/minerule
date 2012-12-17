#include "RuleGidsMatcher.h"
#include "minerule/Database/Transaction.h"
#include "minerule/Result/RulesMatcher.h"
#include "minerule/Result/RuleFormatter.h"

namespace mrmatch {
	
	// create a new empty rule and returns it (so that it can be intialized)
	Rule& RuleGidsMatcher::addRule() {
		_ruleGidsVector.push_back( RuleGids() );
		return _ruleGidsVector.back().first;
	}
	
	void RuleGidsMatcher::matchItemTransaction(const ItemType& gid, const ItemTransaction<RulesMatcher::ItemSetType>& bodies,const ItemTransaction<RulesMatcher::ItemSetType>& heads) {
			for(RuleGidsVector::iterator ruleIt = _ruleGidsVector.begin(); ruleIt!=_ruleGidsVector.end(); ++ruleIt) {				
				if( RulesMatcher::match( ruleIt->first, bodies, heads ) ) {
					ruleIt->second.push_back( gid );
				}
			}		
	}
	
	void RuleGidsMatcher::matchRuleTransaction(const minerule::ItemType& gid, const RuleTransaction<RulesMatcher::RuleSetType>& transaction) {
			for(RuleGidsVector::iterator ruleIt=_ruleGidsVector.begin(); ruleIt!=_ruleGidsVector.end(); ++ruleIt) {
				if( RulesMatcher::match(ruleIt->first, transaction) ) {
					ruleIt->second.push_back( gid );
				}
			}		
	}
		
	
	
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
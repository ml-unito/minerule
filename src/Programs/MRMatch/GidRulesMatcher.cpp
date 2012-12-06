#include "GidRulesMatcher.h"
#include "Result/RuleFormatter.h"

namespace mrmatch {

	Rule& GidRulesMatcher::addRule() {
		rules.push_back(Rule());
		return rules.back();
	}

	void GidRulesMatcher::matchItemTransaction(const minerule::ItemType& gid, const ItemTransaction<RulesMatcher::ItemSetType>& bodies,const ItemTransaction<RulesMatcher::ItemSetType>& heads) {
		_gidRulesVector.push_back( GidRules() );
		GidRules& gidRules = _gidRulesVector.back();
		gidRules.first = gid;

		for(RuleVector::iterator ruleIt = rules.begin(); ruleIt!=rules.end(); ++ruleIt) {				
			if( RulesMatcher::match( *ruleIt, bodies, heads ) ) {
				gidRules.second.push_back(& (*ruleIt) );
			}
		}		
	}
	
	void GidRulesMatcher::matchRuleTransaction(const minerule::ItemType& gid, const RuleTransaction<RulesMatcher::RuleSetType>& transaction) {
		_gidRulesVector.push_back( GidRules() );
		GidRules& gidRules = _gidRulesVector.back();
		gidRules.first = gid;

		for(RuleVector::iterator ruleIt = rules.begin(); ruleIt!=rules.end(); ++ruleIt) {				
			if( RulesMatcher::match( *ruleIt, transaction ) ) {
				gidRules.second.push_back(& (*ruleIt) );
			}
		}				
	}
	
	void GidRulesMatcher::printMatches() const {
		SimpleRuleFormatter sf;
		sf.setFieldWidths( SimpleRuleFormatter::FieldWidths(1,1,1,1) );
		
		for( GidRulesVector::const_iterator it= _gidRulesVector.begin(); it!=_gidRulesVector.end(); ++it ) {
				MRLog() << StringUtils::toBold("Gid: ") << it->first << std::endl;
				const RulePtrVector& matchedRules = it->second;
				
				for( RulePtrVector::const_iterator rIt=matchedRules.begin(); rIt!=matchedRules.end(); ++rIt) {
					MRLog() << StringUtils::toBold("\tRule: ") << sf.formatRule( **rIt ) << std::endl;
				}			
		}
	}

}
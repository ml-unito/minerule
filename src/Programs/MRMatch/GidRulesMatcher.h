#ifndef GIDRULESMATCHER_H_E8FYP5QZ
#define GIDRULESMATCHER_H_E8FYP5QZ

#include "Matcher.h"

namespace mrmatch {
	
	class GidRulesMatcher : public Matcher {
	private:
		typedef std::vector< Rule > RuleVector;
		typedef std::vector< Rule* > RulePtrVector;
		typedef std::pair< ItemType, RulePtrVector > GidRules;
		typedef std::vector< GidRules > GidRulesVector;
		
		RuleVector rules;								// stores all known rules
		GidRulesVector _gidRulesVector;	// stores gid -> rules association
		

	protected:
		virtual void matchItemTransaction(const minerule::ItemType& gid, const ItemTransaction<RulesMatcher::ItemSetType>& bodies,const ItemTransaction<RulesMatcher::ItemSetType>& heads);
		virtual void matchRuleTransaction(const minerule::ItemType& gid, const RuleTransaction<RulesMatcher::RuleSetType>& transaction);
		
	public:
		GidRulesMatcher() {};
		virtual ~GidRulesMatcher() {};
		
		// create a new empty rule and returns it (so that it can be intialized)
		virtual Rule& addRule();
		
		// outputs the results
		virtual void printMatches() const;		
	};

}

#endif /* end of include guard: GIDRULESMATCHER_H_E8FYP5QZ */

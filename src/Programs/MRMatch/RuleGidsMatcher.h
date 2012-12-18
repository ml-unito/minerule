#ifndef RULEGIDMATCHER_H_X9STRBC4
#define RULEGIDMATCHER_H_X9STRBC4

#include "Matcher.h"

namespace mrmatch {

	class RuleGidsMatcher : public Matcher {
	private:
		typedef std::vector<minerule::ItemType> Gids;
		typedef std::pair< minerule::Rule, Gids > RuleGids;
		typedef std::vector< RuleGids > RuleGidsVector;
	
		RuleGidsVector _ruleGidsVector;
		
	protected:
		virtual void matchItemTransaction(const minerule::ItemType& gid, const minerule::ItemTransaction<minerule::RulesMatcher::ItemSetType>& bodies,const minerule::ItemTransaction<minerule::RulesMatcher::ItemSetType>& heads);
		virtual void matchRuleTransaction(const minerule::ItemType& gid, const minerule::RuleTransaction<minerule::RulesMatcher::RuleSetType>& transaction);

		std::string formatGids( const Gids& gids ) const;	
		
	public:
		RuleGidsMatcher() {}
		virtual ~RuleGidsMatcher() {}

	// create a new empty rule and returns it (so that it can be intialized)
		virtual minerule::Rule& addRule();
		
	// outputs the results
		virtual void printMatches() const;
	
	};

}

#endif /* end of include guard: RULEGIDMATCHER_H_X9STRBC4 */

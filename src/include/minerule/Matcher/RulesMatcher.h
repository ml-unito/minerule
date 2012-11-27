#ifndef RULESMATCHER_H_2OI7RAD0
#define RULESMATCHER_H_2OI7RAD0

namespace minerule {
	// Matches rules against a source table.
	// Two possible usages:
	//    - find all rules that match a given source group
	//	  - find all groups that match a given set of rules
	//		in this latter case: return for each rule, the set of groups that match
	class RulesMatcher {
	public:
		typedef QueryResult::ResultSet<QueryResult::FastSorter> RulesSet;
		
		RulesMatcher(RulesSet& resultSet) : _resultSet(resultSet) {};
		virtual ~RulesMatcher () {};
	
		// Check which rules in _resultSet match the given group, and update the rules
		// structure to account for this match.
		// The method populates "matchingRules" with the subset of rules that match
		// the group. 
		// Returns true if at least one rule matches.
		bool match(const SourceGroup& group, RulesSet& matchingRules) const;

		// Check which rules in _resultSet match the given group, and update the rules
		// structure to account for this match.
		// Returns true if at least one rule matches.
		bool match(const SourceGroup& group) const;
	private:
	/* data */
		const & _resultSet;
	};

}

#endif /* end of include guard: RULESMATCHER_H_2OI7RAD0 */

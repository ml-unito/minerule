#ifndef GIDRULEMATCHER_H_265LZIR0
#define GIDRULEMATCHER_H_265LZIR0

#include "Matcher.h"

namespace mrmatch {

	class GidRuleMatcher : public Matcher {
	public:
		GidRuleMatcher() {};
		virtual ~GidRuleMatcher() {};


	// create a new empty rule and returns it (so that it can be intialized)
		virtual Rule& addRule();
	
	// Match the given source table with the current set of rules
		virtual void match(SourceTable& st);
	
	// outputs the results
		virtual void printMatches() const;

	private:
	/* data */
		typedef std::vector< std::pair< minerule::ItemType, std::vector<minerule::Rule> > > GidRulesVector;
	
	};

}

#endif /* end of include guard: GIDRULEMATCHER_H_265LZIR0 */

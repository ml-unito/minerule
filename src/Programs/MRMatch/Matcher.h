#ifndef MATCHERS_H_Q12443YU
#define MATCHERS_H_Q12443YU

#include "Result/Rule.h"
#include "Database/SourceTable.h"

#include "mrmatch.h"

namespace mrmatch {

// Base class for all Matchers
	class Matcher {
	public:
	// creates a new matcher and returns it
		static Matcher* newMatcher(MatchKind kind);

	// create a new empty rule and returns it (so that it can be intialized)
		virtual minerule::Rule& addRule() = 0;
	
	// Match the given source table with the current set of rules
		virtual void match(SourceTable& st) {
			st.usesCrossProduct() ? matchWithCrossProduct(st) : matchWithoutCrossProduct(st);
		}
	
	// outputs the results
		virtual void printMatches() const = 0;
	
	protected:
	
		virtual void matchWithCrossProduct(SourceTable& st) = 0;
		virtual void matchWithoutCrossProduct(SourceTable& st) = 0;
	};

}

#endif /* end of include guard: MATCHERS_H_Q12443YU */

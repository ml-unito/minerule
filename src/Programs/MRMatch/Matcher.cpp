#include "Matcher.h"
#include "RuleGidsMatcher.h"
#include "Utils/MineruleErrors.h"

namespace mrmatch {
	Matcher* Matcher::newMatcher(MatchKind kind) {
		switch(kind) {
			case RuleGids: return new RuleGidsMatcher();
			default: throw MineruleException( MR_ERROR_INTERNAL, "Unknown or unsupported matcher kind");
		}
	}
		

}
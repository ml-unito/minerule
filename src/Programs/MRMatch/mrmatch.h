#ifndef MRMATCH_H_MP1T5YIM
#define MRMATCH_H_MP1T5YIM

#include <vector>
#include "minerule/Result/Rule.h"

namespace mrmatch {
	typedef enum { SUCCESS=0, MRMATCH_OPTION_PARSING_ERROR  } ExitCodes;
	
	// A matcher spec is the bit and of a match kind and a matchoutput.
	// the values of the two types have been designed so that one can
	// combine a matchkind and a matchoutput using the or bit-operator
	// and retrieve the components by anding the result with the appropriate 
	// matchmask.
	// Eg. GidRules | OutOnDb --> 10
	//     10 & MatchKindMask --> 2 == GidRules
	//     10 & MatchOutputMask --> 8 ==- OutOnDb
	// Alternatively, one can check if a particular flag is set by anding the
	// result with one of the MatchKind/MatchOutput.
	// Eg. GidRules | OutOnDb --> 10
	//     10 & GidRules --> true
	//     10 & RuleGids --> false
	//     10 & OutOnConsole --> false
	//     10 & OutOnDb --> true
	typedef enum { None = 0, RuleGids = 1, GidRules = 2, MatchKindMask = 3 } MatchKind;
	typedef enum { OutOnConsole = 4, OutOnDB = 8, MatchOutputMask = 12 } MatchOutput;
	typedef unsigned int MatcherSpecs;	
}


#endif /* end of include guard: MRMATCH_H_MP1T5YIM */

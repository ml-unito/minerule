#ifndef MRMATCH_H_MP1T5YIM
#define MRMATCH_H_MP1T5YIM

#include <vector>
#include "Result/Rule.h"

namespace mrmatch {
	typedef enum { SUCCESS=0, MRMATCH_OPTION_PARSING_ERROR } ExitCodes;
	
	typedef std::vector< std::pair< minerule::Rule, std::vector<minerule::ItemType> > > RuleGidsVector;
}


#endif /* end of include guard: MRMATCH_H_MP1T5YIM */

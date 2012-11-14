#ifndef MRCATALOGUE_H_89D0JOQH
#define MRCATALOGUE_H_89D0JOQH

#include "Options.h"
#include "Utils/MineruleOptions.h"

namespace mrc { // minerule catalogue
  
	// --------------------------------------------------------------------------------
	// Functions 
	// --------------------------------------------------------------------------------
	void parseOptions(int argc, char** argv, minerule::MineruleOptions& mopt, Options& popt);
	int  performCommands(Options& options);
}

#endif /* end of include guard: MRCATALOGUE_H_89D0JOQH */

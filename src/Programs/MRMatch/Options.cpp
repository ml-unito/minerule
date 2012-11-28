#include "Options.h"

#include "Utils/MineruleOptions.h"

using namespace minerule;

namespace mrmatch {

bool Options::initMineruleOptions() const {
	bool ok=false;
	MineruleOptions& options = MineruleOptions::getSharedOptions();
	
	if( !_mrOptionsFileName.empty() ) {
		options.readFromFile(_mrOptionsFileName);
		ok=true;
	}
	
	std::vector<std::string>::const_iterator it;
	for(it=_mrOptionsInline.begin(); it!=_mrOptionsInline.end(); ++it) {
		options.readFromString(*it);
		ok=true;
	}
	
	return ok;
}

}
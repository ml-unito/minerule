#include "mrmatch.h"
#include "Options.h"

#include "Utils/MineruleOptions.h"
#include "Utils/FileUtils.h"

using namespace minerule;

namespace mrmatch {

	bool Options::initMineruleOptions() const {
		bool ok=false;
		MineruleOptions& options = MineruleOptions::getSharedOptions();
	
		if( !_mrOptionsFileName.empty() ) {
			options.readFromFile(_mrOptionsFileName);
			ok=true;
		} else if( FileUtils::fileExists("options.txt") ) {
			options.readFromFile("options.txt");
			ok=true;
		}
	
		std::vector<std::string>::const_iterator it;
		for(it=_mrOptionsInline.begin(); it!=_mrOptionsInline.end(); ++it) {
			options.readFromString(*it);
			ok=true;
		}
	
		return ok;
	}
	
	std::string Options::queryName() const {
		if( _queryNumber != 0 && !_queryName.empty() ) {
			std::cout << StringUtils::toBold("Error:") << "It appears that you specified both a query name and a query number" << std::endl;
			exit(MRMATCH_OPTION_PARSING_ERROR);
		}
		
		if( _queryNumber != 0 )
			return OptimizerCatalogue::getMRQueryName(_queryNumber);		
		
		if( !_queryName.empty() )
			return _queryName;
		
		std::cout << StringUtils::toBold("Error:") << "It appears that you specified neither a query name nor a query number" << std::endl;
		exit(MRMATCH_OPTION_PARSING_ERROR);
	}

}
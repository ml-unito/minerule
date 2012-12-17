#include "minerule/Utils/MineruleOptions.h"

#include "ErrorCodes.h"
#include "MRCUtils.h"
#include "Options.h"

using namespace minerule;

int
main(int argc, char** argv) {
	if( argc < 2 ) {
		mrc::printHelp(argc, argv);
		exit(0);
	}

	int resultVal;
	try {
	  
		MineruleOptions& mr = MineruleOptions::getSharedOptions();
		mrc::Options options;
		mrc::parseOptions(argc, argv, mr, options);

		resultVal=mrc::performCommands(options);
	} catch (mrc::Exception& e) {
		resultVal = e.getResultID();
		if(resultVal!=mrc::NOTHING_TO_DO)
			std::cout << e.what() << std::endl;
	} catch (std::exception& e) {
		std::cout << "An error occurred, the error msg is:" << std::endl;
		std::cout << e.what() << std::endl;
		resultVal=mrc::ERROR_EXCEPTION_THROWN;
	}

	return resultVal;
}

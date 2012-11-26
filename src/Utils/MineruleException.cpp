#include "Utils/MineruleException.h"
#include "Utils/StringUtils.h"

namespace minerule {
	
	#undef MineruleException

	MineruleException::MineruleException(std::string file, int line,  size_t errCode, std::string msg) throw() {
		errorCode=errCode;
		std::stringstream ss;

		ss << "\n\t" << StringUtils::toBold("class:") << "\tMineruleException " << std::endl
			<< StringUtils::toBold("\tsource:\t") << file << ":" << line << std::endl
			<< StringUtils::toBold("\tcode:\t")<< errorCode  
			<< " - " << me_error_name((MineruleErrors) errorCode) << std::endl
			<< StringUtils::toBold("\tmessage:");
	
		std::vector<std::string>* chunks = StringUtils::splitToLength(msg, 70);
		ss << StringUtils::join(*chunks, "\n\t\t");
	
		message = ss.str();
		
		delete chunks;
	}
 
}
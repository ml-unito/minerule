#include "Utils/MineruleException.h"
#include "Utils/StringUtils.h"

namespace minerule {
	
	#undef MineruleException

	MineruleException::MineruleException(std::string file, int line,  size_t errCode, std::string msg) throw() {
		errorCode=errCode;
		std::stringstream ss;

		ss << "\n\tclass:\tMineruleException " << std::endl
			<< "\tsource:\t" << file << ":" << line << std::endl
			<< "\tcode:\t" << errorCode  
			<< " - " << me_error_name((MineruleErrors) errorCode) << std::endl
			<< "\tmessage:";
	
		std::vector<std::string>* chunks = StringUtils::split_to_length(msg, 70);
		ss << StringUtils::join(*chunks, "\n\t\t");
	
		message = ss.str();
		
		delete chunks;
	}
 
}
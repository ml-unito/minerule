#include "minerule/Utils/MineruleException.h"
#include "minerule/Utils/StringUtils.h"

namespace minerule {
	
	#undef MineruleException
	
	MineruleException::MineruleException(std::string sourceFile, int sourceLine,  size_t errCode, std::string msg) 
			throw() : file(sourceFile), line(sourceLine), errorCode(errCode), message(msg) { 
				formatMessage();			
	}
	
	void MineruleException::formatMessage() {		
		std::stringstream ss;

		ss << "\n\t" << StringUtils::toBold("class:") << "\tMineruleException " << std::endl
			<< StringUtils::toBold("\tsource:\t") << file << ":" << line << std::endl
			<< StringUtils::toBold("\tcode:\t")<< errorCode  
			<< " - " << me_error_name((MineruleErrors) errorCode) << std::endl
			<< StringUtils::toBold("\tmessage:") << StringUtils::toBoldRed("[");
	
		// std::vector<std::string>* chunks = StringUtils::splitToLength(message, 70);
		// ss << StringUtils::join(*chunks, "\n");
		ss << message;
		ss << StringUtils::toBoldRed("]");
	
		// delete chunks;	

		formattedMessage = ss.str();		
	}
	
	const char* MineruleException::what() const throw() {
		return formattedMessage.c_str();
	} 
	
	const char* MineruleException::unformattedMessage() const throw() {
     return message.c_str();
	}
	
}
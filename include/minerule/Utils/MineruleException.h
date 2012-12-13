#ifndef __MINERULE_EXCEPTION_H__
#define __MINERULE_EXCEPTION_H__

#include <exception>
#include <sstream>
#include <string>
#include "Utils/MineruleErrors.h"

namespace minerule {

class MineruleException : public std::exception {
  std::string message;
	std::string formattedMessage;
	
	std::string file;
	int line;
  size_t errorCode;
	
	void formatMessage();
 public:
  MineruleException(std::string sourceFile, int sourceLine,  size_t errCode, std::string msg) throw(); 

  virtual ~MineruleException() throw() {    
  }

  virtual const char* what() const throw();
	virtual const char* unformattedMessage() const throw();

  virtual size_t getErrorCode() const {
    return errorCode;
  }
};

}

#define MineruleException(a,b) MineruleException( __FILE__, __LINE__, a,b )

#endif

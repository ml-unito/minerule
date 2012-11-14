#ifndef EXCEPTION_H_U3CYE3H3
#define EXCEPTION_H_U3CYE3H3

#include "ErrorCodes.h"
#include <exception>
#include <string>

namespace mrc {

	class Exception : public std::exception {
		  mrc::Results result;
		  std::string errmsg;
	public:
		  Exception(mrc::Results res,
			  const std::string& msg) :  result(res),errmsg(msg) { }

		  virtual ~Exception() throw() {};

		  virtual const char* what() const throw () {
			  return errmsg.c_str();
		  }

		  virtual mrc::Results getResultID() const {
			  return result;
		  }
	};

}
#endif /* end of include guard: EXCEPTION_H_U3CYE3H3 */

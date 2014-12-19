//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef __MINERULE_EXCEPTION_H__
#define __MINERULE_EXCEPTION_H__

#include <exception>
#include <sstream>
#include <string>
#include "minerule/Utils/MineruleErrors.hpp"

#ifndef _NOEXCEPT
#define _NOEXCEPT throw()
#endif

namespace minerule {

class MineruleException : public std::exception {
  std::string message;
	std::string formattedMessage;

	std::string file;
	int line;
  size_t errorCode;

	void formatMessage() _NOEXCEPT;
 public:
  MineruleException(std::string sourceFile, int sourceLine,  size_t errCode, std::string msg);

  virtual ~MineruleException() _NOEXCEPT {
  }

  virtual const char* what() const _NOEXCEPT ;
	virtual const char* unformattedMessage() const _NOEXCEPT ;

  virtual size_t getErrorCode() const _NOEXCEPT {
    return errorCode;
  }
};

}

#define MineruleException(a,b) MineruleException( __FILE__, __LINE__, a,b )

#endif

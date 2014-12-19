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
#ifndef EXCEPTION_H_U3CYE3H3
#define EXCEPTION_H_U3CYE3H3

#include "ErrorCodes.hpp"
#include <exception>
#include <string>

#ifndef _NOEXCEPT
#define _NOEXCEPT throw()
#endif

namespace mrc {

	class Exception : public std::exception {
		  mrc::Results result;
		  std::string errmsg;
	public:
		  Exception(mrc::Results res,
			  const std::string& msg) :  result(res),errmsg(msg) { }

		  virtual ~Exception() _NOEXCEPT {};

		  virtual const char* what() const _NOEXCEPT {
			  return errmsg.c_str();
		  }

		  virtual mrc::Results getResultID() const {
			  return result;
		  }
	};

}
#endif /* end of include guard: EXCEPTION_H_U3CYE3H3 */

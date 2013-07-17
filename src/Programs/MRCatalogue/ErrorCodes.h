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
#ifndef ERRORCODES_H_307OH17
#define ERRORCODES_H_307OH17

namespace mrc {

	typedef enum {
	  SUCCESS=0,
	  NOTHING_TO_DO,
	  QUERY_NAME_FOUND,
	  QUERY_NAME_NOT_FOUND,
	  CATALOGUE_NOT_INSTALLED,
	  ERROR_OPTION_PARSING,
	  ERROR_CANNOT_OPEN_OPTION_FILE,
	  ERROR_EXCEPTION_THROWN,		  
	} Results;

}
#endif /* end of include guard: ERRORCODES_H_307OH17 */

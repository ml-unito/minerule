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
# - Try to find libodbc++
# Once done, this will define
#
#  LIBODBC++_FOUND - system has LIBODBC++
#  LIBODBC++_INCLUDE_DIRS - the LIBODBC++ include directories
#  LIBODBC++_LIBRARIES - link these to use LIBODBC++

include(LibFindMacros)

find_path(LIBODBC++_INCLUDE_DIR NAMES odbc++/drivermanager.h)

# Finally the library itself
find_library(LIBODBC++_LIBRARY NAMES odbc++ PATHS /usr/local/lib)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(LIBODBC++_PROCESS_INCLUDES LIBODBC++_INCLUDE_DIR)
set(LIBODBC++_PROCESS_LIBS LIBODBC++_LIBRARY)
libfind_process(LIBODBC++)
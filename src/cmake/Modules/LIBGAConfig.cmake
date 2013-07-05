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
# - Try to find libga
# Once done, this will define
#
#  LIBGA_FOUND - system has LIBGA
#  LIBGA_INCLUDE_DIRS - the LIBGA include directories
#  LIBGA_LIBRARIES - link these to use LIBGA

include(LibFindMacros)

find_path(LIBGA_INCLUDE_DIR NAMES ga/ga.h)

# Finally the library itself
find_library(LIBGA_LIBRARY NAMES ga)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(LIBGA_PROCESS_INCLUDES LIBGA_INCLUDE_DIR)
set(LIBGA_PROCESS_LIBS LIBGA_LIBRARY)
libfind_process(LIBGA)
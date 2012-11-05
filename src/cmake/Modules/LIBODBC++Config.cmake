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
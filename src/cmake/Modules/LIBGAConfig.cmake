# - Try to find libga
# Once done, this will define
#
#  LIBGA_FOUND - system has LIBGA
#  LIBGA_INCLUDE_DIRS - the LIBGA include directories
#  LIBGA_LIBRARIES - link these to use LIBGA

include(LibFindMacros)

find_path(LIBGA_INCLUDE_DIR
  NAMES ga/ga.h
  PATHS /usr/local/include/
)

# Finally the library itself
find_library(LIBGA_LIBRARY
  NAMES ga
  PATHS /usr/local/lib
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(LIBGA_PROCESS_INCLUDES LIBGA_INCLUDE_DIR)
set(LIBGA_PROCESS_LIBS LIBGA_LIBRARY)
libfind_process(LIBGA)
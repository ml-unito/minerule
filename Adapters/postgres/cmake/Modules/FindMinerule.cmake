include(LibFindMacros)

find_path(Minerule_INCLUDE_DIR NAMES minerule/Algorithms/Algorithms.hpp)
find_library(Minerule_LIBRARY NAMES minerule)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Minerule_PROCESS_INCLUDES Minerule_INCLUDE_DIR)
set(Minerule_PROCESS_LIBS Minerule_LIBRARY)

MESSAGE(${Minerule_PROCESS_INCLUDES})
MESSAGE(${Minerule_PROCESS_LIBS})

libfind_process(Minerule)

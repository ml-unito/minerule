project(minerule)
cmake_minimum_required(VERSION 2.8)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
LIST(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

# Dependencies
find_package(LIBODBC++ REQUIRED)
include_directories(${LIBODBC++_INCLUDE_DIRS})

find_package(LIBGA REQUIRED)
include_directories(${LIBGA_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include/minerule ${CMAKE_SOURCE_DIR}/../ExtLibs/libgist/include)
add_definitions(-DMR_VERSION=\"2.0a\")

add_subdirectory(Algorithms)
add_subdirectory(Database)
add_subdirectory(Optimizer)
add_subdirectory(PredicateUtils)
add_subdirectory(Utils)
add_subdirectory(Parsers)

LIST(APPEND LIBS mralgorithms mrdatabase mroptimizer mrpredicateutils mrutils mrparsers)
LIST(APPEND LIBS ${LIBGA_LIBRARIES})
LIST(APPEND LIBS ${LIBODBC++_LIBRARIES})

add_executable(minerule Programs/Minerule/minerule.cpp)
target_link_libraries(minerule ${LIBS})

add_executable(mrcatalogue 
	Programs/MRCatalogue/mrcatalogue.cpp 
	Programs/MRCatalogue/MRCUtils.cpp
	Programs/MRCatalogue/Printer.cpp)
	
target_link_libraries(mrcatalogue 
	mrpredicateutils 
	mroptimizer
	mrdatabase 
	mrutils 
	mrparsers 
	${LIBODBC++_LIBRARIES})

add_executable(mrprintrules 
	Programs/MRPrintRules/PrintRules.cpp 	
	Programs/MRPrintRules/RuleFormatter.cpp)
	
target_link_libraries(mrprintrules 
	mroptimizer 
	mrdatabase 
	mrpredicateutils 
	mrutils 
	mrparsers 
	${LIBODBC++_LIBRARIES})

add_executable(mrdefaultoptions 
	Programs/MRDefaultOptions/MRDefaultOptions.cpp)
	
target_link_libraries(mrdefaultoptions 
	mrutils 
	${LIBODBC++_LIBRARIES})



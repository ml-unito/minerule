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
add_subdirectory(Result)

LIST(APPEND LIBS mralgorithms mrdatabase mroptimizer mrpredicateutils mrutils mrparsers mrresult)
LIST(APPEND LIBS ${LIBGA_LIBRARIES})
LIST(APPEND LIBS ${LIBODBC++_LIBRARIES})

add_executable(mr Programs/Minerule/minerule.cpp)
target_link_libraries(mr mrutils ${LIBODBC++_LIBRARIES})

add_executable(mr-query Programs/MRQuery/mrquery.cpp)
target_link_libraries(mr-query  ${LIBS})

add_executable(mr-catalogue 
	Programs/MRCatalogue/mrcatalogue.cpp 
	Programs/MRCatalogue/MRCUtils.cpp
	Programs/MRCatalogue/Printer.cpp
	Programs/MRCatalogue/Options.cpp)
	
target_link_libraries(mr-catalogue 
	mrpredicateutils 
	mroptimizer
	mrdatabase 
	mrutils
	mrresult
	mrparsers 
	${LIBODBC++_LIBRARIES})

add_executable(mr-print 
	Programs/MRPrintRules/PrintRules.cpp 	
	Programs/MRPrintRules/RuleFormatter.cpp)
	
target_link_libraries(mr-print 
	mroptimizer 
	mrdatabase 
	mrpredicateutils 
	mrutils
	mrresult 
	mrparsers 
	${LIBODBC++_LIBRARIES})

add_executable(mr-defaults 
	Programs/MRDefaultOptions/MRDefaultOptions.cpp)
	
target_link_libraries(mr-defaults 
	mrutils 
	${LIBODBC++_LIBRARIES})


add_executable(mr-match 
	Programs/MRMatch/mrmatch.cpp
	Programs/MRMatch/options.cpp)
	
target_link_libraries(mr-match 
	mroptimizer
	mrresult
	mrutils
	${LIBODBC++_LIBRARIES})



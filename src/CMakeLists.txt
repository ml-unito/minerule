project(minerule)
cmake_minimum_required(VERSION 2.8)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
LIST(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

# Dependencies
find_package(LIBODBC++ REQUIRED)
include_directories(${LIBODBC++_INCLUDE_DIRS})

find_package(LIBGA REQUIRED)
include_directories(${LIBGA_INCLUDE_DIRS})



include_directories(${CMAKE_SOURCE_DIR}/include/minerule ${CMAKE_SOURCE_DIR}/../ExtLibs/libgist/include ${CMAKE_SOURCE_DIR}/../ExtLibs/galib245)
add_definitions(-DMR_VERSION=\"2.0a\")

add_subdirectory(Algorithms)
add_subdirectory(MRDatabase)
add_subdirectory(Optimizer)
add_subdirectory(PredicateUtils)
add_subdirectory(Utils)
add_subdirectory(Parsers)

LIST(APPEND LIBS mralgorithms mrdatabase mroptimizer mrpredicateutils mrutils mrparsers)
LIST(APPEND LIBS ${LIBGA_LIBRARIES})
LIST(APPEND LIBS ${LIBODBC++_LIBRARIES})

add_executable(minerule Programs/minerule.cpp)
target_link_libraries(minerule ${LIBS})

add_executable(mrcatalogue Programs/mrcatalogue.cpp)
target_link_libraries(mrdatabase mroptimizer mrpredicateutils mrutils mrparsers ${LIBODBC++_LIBRARIES})

add_executable(mrprintrules Programs/PrintRules.cpp Programs/RuleFormatter.cpp)
target_link_libraries(mrdatabase mroptimizer mrpredicateutils mrutils mrparsers ${LIBODBC++_LIBRARIES})

add_executable(mrdefaultoptions Programs/MRDefaultOptions.cpp)
target_link_libraries(mrdefaultoptions mrutils ${LIBODBC++_LIBRARIES})



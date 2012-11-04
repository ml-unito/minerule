project(minerule)

cmake_minimum_required(VERSION 2.8)


file(GLOB algorithms_src src/Algorithms/*.cpp)

add_library(mralgorithms ${algorithms_src})

include_directories(${CMAKE_SOURCE_DIR}/include/minerule ${CMAKE_SOURCE_DIR}/ExtLibs/libgist/include ${CMAKE_SOURCE_DIR}/ExtLibs/galib245)

add_definitions(-DMR_VERSION=\"2.0a\")

add_executable(minerule Programs/minerule.cpp)



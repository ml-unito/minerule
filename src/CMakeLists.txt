#   Minerule - a sql-like language for datamining
#   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
project(minerule)
cmake_minimum_required(VERSION 2.8)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
LIST(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

execute_process(COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/Utils)
execute_process(COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/Parsers)

# Dependencies

find_package(LIBGA REQUIRED)
include_directories(${LIBGA_INCLUDE_DIRS})

find_package(BISON)
find_package(FLEX)

include_directories(${CMAKE_SOURCE_DIR}/../include)
add_definitions(-DMR_VERSION=\"2.0a\")


flex_target(OPTLEXER
	${CMAKE_CURRENT_SOURCE_DIR}/Utils/OptionParser.lpp
	${CMAKE_CURRENT_BINARY_DIR}/Utils/OptionParser_lexer.cpp
	COMPILE_FLAGS "-POP")

bison_target(OPTPARSER
	${CMAKE_CURRENT_SOURCE_DIR}/Utils/OptionParser.ypp
	${CMAKE_CURRENT_BINARY_DIR}/Utils/OptionParser_parser.cpp
	COMPILE_FLAGS "-pOP")

add_flex_bison_dependency(OPTLEXER OPTPARSER)

flex_target(MRLEXER
	${CMAKE_CURRENT_SOURCE_DIR}/Parsers/MineruleParser.lpp
	${CMAKE_CURRENT_BINARY_DIR}/Parsers/MineruleParser_lexer.cpp
	COMPILE_FLAGS "-P mr -i")

bison_target(MRPARSER
	${CMAKE_CURRENT_SOURCE_DIR}/Parsers/MineruleParser.ypp
	${CMAKE_CURRENT_BINARY_DIR}/Parsers/MineruleParser_parser.cpp
	COMPILE_FLAGS "-p mr")

# include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(utils_src
	Utils/AlgorithmTypes.cpp
	Utils/Bitstring.cpp
	Utils/Constraints.cpp
	Utils/MineruleErrors.cpp
	Utils/MineruleOptions.cpp
	Utils/MRLogger.cpp
	${BISON_OPTPARSER_OUTPUTS}
	${FLEX_OPTLEXER_OUTPUTS}
	Utils/FileUtils.cpp
	Utils/OptionParserLib.cpp
	Utils/MineruleLogs.cpp
	Utils/SQLUtils.cpp
	Utils/StringUtils.cpp
	Utils/MineruleException.cpp
	Utils/Progress.cpp)

set(parsers_src
	${BISON_MRPARSER_OUTPUTS}
	${FLEX_MRLEXER_OUTPUTS}
	Parsers/ParsedMinerule.cpp
	Parsers/ParsedPredicate.cpp
	Parsers/ParserLibrary.cpp
	Parsers/PredicateBase.cpp
	Parsers/SupportMeasure.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB algorithms_src Algorithms/*.cpp)
file(GLOB mrdatabase_src Database/*.cpp)
file(GLOB optimizer_src Optimizer/*.cpp)
file(GLOB installers_src Optimizer/Installers/*.cpp)
file(GLOB predicateutils_src PredicateUtils/*.cpp)
file(GLOB result_src Result/*.cpp)

add_library(minerule
	${utils_src}
	${algorithms_src}
	${mrdatabase_src}
	${optimizer_src}
	${installers_src}
	${predicateutils_src}
	${parsers_src}
	${result_src} )


LIST(APPEND LIBS minerule)
LIST(APPEND LIBS ${LIBGA_LIBRARIES})
LIST(APPEND LIBS ${CMAKE_DL_LIBS})

add_executable(mr Programs/Minerule/minerule.cpp)
target_link_libraries(mr ${LIBS})

add_executable(mr-query Programs/MRQuery/mrquery.cpp)
target_link_libraries(mr-query  ${LIBS})

add_executable(mr-catalogue
	Programs/MRCatalogue/mrcatalogue.cpp
	Programs/MRCatalogue/MRCUtils.cpp
	Programs/MRCatalogue/Printer.cpp
	Programs/MRCatalogue/Options.cpp)

target_link_libraries(mr-catalogue 	${LIBS})

add_executable(mr-print
	Programs/MRPrintRules/PrintRules.cpp
	Programs/MRPrintRules/Options.cpp)

target_link_libraries(mr-print 	${LIBS})

add_executable(mr-defaults
	Programs/MRDefaultOptions/MRDefaultOptions.cpp)

target_link_libraries(mr-defaults ${LIBS})

add_executable(mr-match
	Programs/MRMatch/mrmatch.cpp
	Programs/MRMatch/Options.cpp
	Programs/MRMatch/Matcher.cpp
	Programs/MRMatch/RuleGidsMatcher.cpp
	Programs/MRMatch/GidRulesMatcher.cpp
	Programs/MRMatch/RuleGidsDBMatcher.cpp)

target_link_libraries(mr-match ${LIBS})


INSTALL(TARGETS mr mr-query mr-catalogue mr-print mr-defaults mr-match minerule
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib)

INSTALL(DIRECTORY ../include/minerule DESTINATION include)

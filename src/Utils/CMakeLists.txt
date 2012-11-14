find_package(BISON)
find_package(FLEX)

flex_target(OPTLEXER 
	${CMAKE_CURRENT_SOURCE_DIR}/OptionParser.lpp 
	${CMAKE_CURRENT_BINARY_DIR}/OptionParser_lexer.cpp 
	COMPILE_FLAGS "-POP")
	
bison_target(OPTPARSER 
	${CMAKE_CURRENT_SOURCE_DIR}/OptionParser.ypp 
	${CMAKE_CURRENT_BINARY_DIR}/OptionParser_parser.cpp
	COMPILE_FLAGS "-pOP")

add_flex_bison_dependency(OPTLEXER OPTPARSER)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(utils_src 
	AlgorithmTypes.cpp 
	Bitstring.cpp 
	Constraints.cpp
	MineruleErrors.cpp
	MineruleOptions.cpp
	MRLogger.cpp
	${BISON_OPTPARSER_OUTPUTS}
	${FLEX_OPTLEXER_OUTPUTS}
	OptionParserLib.cpp
	MineruleLogs.cpp
	SQLUtils.cpp
	StringUtils.cpp
	MineruleException.cpp
	FileUtils.cpp)
	

add_library( mrutils ${utils_src})
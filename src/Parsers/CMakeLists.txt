find_package(BISON)
find_package(FLEX)

flex_target(MRLEXER 
	${CMAKE_CURRENT_SOURCE_DIR}/MineruleParser.lpp 
	${CMAKE_CURRENT_BINARY_DIR}/MineruleParser_lexer.cpp 
	COMPILE_FLAGS "-P mr -i")
	
bison_target(MRPARSER 
	${CMAKE_CURRENT_SOURCE_DIR}/MineruleParser.ypp 
	${CMAKE_CURRENT_BINARY_DIR}/MineruleParser_parser.cpp
	COMPILE_FLAGS "-p mr")

add_flex_bison_dependency(MRLEXER MRPARSER)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(parsers_src
	${BISON_MRPARSER_OUTPUTS}
	${FLEX_MRLEXER_OUTPUTS}
	ParsedMinerule.cpp
	ParsedPredicate.cpp
	ParserLibrary.cpp
	PredicateBase.cpp
	SupportMeasure.cpp)
	

add_library( mrparsers ${parsers_src})
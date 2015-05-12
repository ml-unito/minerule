#ifndef __PARSER_LIBRARY_H__
#define __PARSER_LIBRARY_H__

#include "ParsedMinerule.hpp"

namespace minerule {

void parseMinerule(std::string minerule_text, ParsedMinerule& output);
ParsedMinerule& getParserOutputObj();

}

#endif

#ifndef __PARSER_LIBRARY_H__
#define __PARSER_LIBRARY_H__

#include "ParsedMinerule.h"

namespace minerule {

void pars_minerule(std::string minerule_text, ParsedMinerule& output);
ParsedMinerule& getParserOutputObj();

}

#endif

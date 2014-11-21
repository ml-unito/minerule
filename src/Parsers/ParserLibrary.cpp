//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "minerule/Parsers/ParserLibrary.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include <iostream>

extern int mrparse();
extern const FILE* mrout;

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE  mr_scan_string(const char*);
extern void mr_delete_buffer(YY_BUFFER_STATE);


namespace minerule {
  
    extern void init_mrparser();
    ParsedMinerule * outputObj;
  
void parseMinerule(std::string minerule_text, ParsedMinerule& output) {
    outputObj = &output;

    const FILE* logfile = 
      minerule::MineruleOptions::getSharedOptions().getParsers().getLogFILE();

    mrout=logfile;
    YY_BUFFER_STATE buf = mr_scan_string(minerule_text.c_str());

    init_mrparser();

    try {
       mrparse();
    } catch (MineruleException& m) {
     std::string error;
      error = std::string(m.unformattedMessage())+ " Original minerule was:"+minerule_text;
      throw MineruleException(MR_ERROR_MINERULETEXT_PARSING, error);
    }

    mr_delete_buffer(buf);
    
    outputObj = NULL;
  }

  ParsedMinerule& getParserOutputObj() {
    assert(outputObj!=NULL);
    return *outputObj;
  }
}

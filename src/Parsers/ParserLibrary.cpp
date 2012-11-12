#include "Parsers/ParserLibrary.h"
#include <Utils/MineruleOptions.h>
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
      error = std::string(m.what())+ " Original minerule was:"+minerule_text;
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

#ifndef __OPTION_PARSER_LIB_H__
#define __OPTION_PARSER_LIB_H__

#include "MineruleOptions.h"


namespace minerule {
  void initializeOptionsFromFile(MineruleOptions& mr, FILE* file);
  void initializeOptionsFromString(MineruleOptions& mr, string);

  /* the following functions are used by the option parser */
  void pushOptionClassIntoContext(const string& oclass) throw(MineruleException);
  void popOptionClassFromContext();
  void setOption(const string& name, const string& value);
} // namespace

#endif

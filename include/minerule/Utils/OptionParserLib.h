#ifndef __OPTION_PARSER_LIB_H__
#define __OPTION_PARSER_LIB_H__

#include "MineruleOptions.h"


namespace minerule {
  void initializeOptionsFromFile(MineruleOptions& mr, FILE* file);
  void initializeOptionsFromString(MineruleOptions& mr, std::string);

  /* the following functions are used by the option parser */
  void pushOptionClassIntoContext(const std::string& oclass) throw(MineruleException);
  void popOptionClassFromContext();
  void setOption(const std::string& name, const std::string& value);
} // namespace

#endif

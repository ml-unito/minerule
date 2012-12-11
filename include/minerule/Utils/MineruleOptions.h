#ifndef __MINERULE_OPTIONS_H__
#define __MINERULE_OPTIONS_H__

#include <string>
#include <string.h>
#include <odbc++/connection.h>
#include <odbc++/drivermanager.h>
#include <map>
#include <iostream>

#include "Utils/MineruleLogs.h"


#include "Utils/MineruleException.h"
#include "Utils/MinMaxPair.h"
#include "Utils/Converter.h"
#include "Optimizer/OptimizerCatalogue.h"
#include "Utils/MRLogger.h"
#include "Utils/AlgorithmTypes.h"
#include "Utils/StringUtils.h"

namespace minerule {

  class OptionBase {
  public:
    virtual void setOption(const std::string& name,const std::string& value) 
      throw(MineruleException)=0;

    virtual OptionBase& subclassForName(const std::string& subclassName)  
      throw(MineruleException) {
      throw MineruleException(MR_ERROR_OPTION_PARSING, 
			      className()+" does not support sub class named:"+
			      subclassName);
    }

    virtual std::string className() const=0;

  };

// Options subclasses
#include "Utils/MineruleOptions_implementations/root.h"

}// end namespace minerule

#endif

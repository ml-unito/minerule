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

#ifndef __MINERULE_OPTIONS_H__
#define __MINERULE_OPTIONS_H__

#include <string>
#include <string.h>
#include <odbc++/connection.h>
#include <odbc++/drivermanager.h>
#include <map>
#include <iostream>

#include "minerule/Utils/MineruleLogs.h"


#include "minerule/Utils/MineruleException.h"
#include "minerule/Utils/MinMaxPair.h"
#include "minerule/Utils/Converter.h"
#include "minerule/Optimizer/OptimizerCatalogue.h"
#include "minerule/Utils/MRLogger.h"
#include "minerule/Utils/AlgorithmTypes.h"
#include "minerule/Utils/StringUtils.h"

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
#include "minerule/Utils/MineruleOptions_implementations/root.h"

}// end namespace minerule

#endif

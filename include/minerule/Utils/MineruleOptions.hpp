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
#include "minerule/mrdb/Connection.hpp"
#include <map>
#include <iostream>

#include "minerule/Utils/MineruleLogs.hpp"


#include "minerule/Utils/MineruleException.hpp"
#include "minerule/Utils/MinMaxPair.hpp"
#include "minerule/Utils/Converter.hpp"
#include "minerule/Optimizer/OptimizerCatalogue.hpp"
#include "minerule/Utils/MRLogger.hpp"
#include "minerule/Utils/AlgorithmTypes.hpp"
#include "minerule/Utils/StringUtils.hpp"

namespace minerule {

  class OptionBase {
  public:
    virtual ~OptionBase() {}

    virtual void setOption(const std::string& name,const std::string& value)
      =0;

    virtual OptionBase& subclassForName(const std::string& subclassName)
      {
      throw MineruleException(MR_ERROR_OPTION_PARSING,
			      className()+" does not support sub class named:"+
			      subclassName);
    }

    virtual std::string className() const=0;

  };

// Options subclasses
#include "minerule/Utils/MineruleOptions_implementations/root.hpp"

}// end namespace minerule

#endif

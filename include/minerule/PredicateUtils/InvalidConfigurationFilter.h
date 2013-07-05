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
#ifndef __INVALID_CONFIGURATION_FILTER_H__
#define __INVALID_CONFIGURATION_FILTER_H__

#include <string>

#include "minerule/PredicateUtils/VarSet.h"
#include "minerule/PredicateUtils/Interval.h"

namespace minerule {
  
  class InvalidConfigurationFilter {
    const std::string& tab_source;
    list_AND_node* preds;
    IntervalChecker ic;
  public:
    InvalidConfigurationFilter( const std::string& t,
				list_AND_node* l ) :
      tab_source(t),
      preds(l),
      ic(t) {}

    /** returns true if the vset configuration must be 
     *		filtered out
     */
    bool operator()(const VarSet& vset) {
      return ic.impossibleVariableSetting( vset, preds );
    }
  };
};



#endif

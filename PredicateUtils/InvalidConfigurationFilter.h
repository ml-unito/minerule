#ifndef __INVALID_CONFIGURATION_FILTER_H__
#define __INVALID_CONFIGURATION_FILTER_H__

#include <string>

#include "PredicateUtils/VarSet.h"
#include "PredicateUtils/Interval.h"

namespace minerule {
  
  class InvalidConfigurationFilter {
    const string& tab_source;
    list_AND_node* preds;
    IntervalChecker ic;
  public:
    InvalidConfigurationFilter( const std::string& t,
				list_AND_node* l ) :
      tab_source(t),
      preds(l),
      ic(t) {}

    /** returns true if the vset configuration must be 
     *filtered out
     */
    bool operator()(const VarSet& vset) {
      return ic.impossibleVariableSetting( vset, preds );
    }
  };
};



#endif

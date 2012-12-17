#ifndef __INCREMENTAL_ALGORITHM_H__
#define __INCREMENTAL_ALGORITHM_H__

#include "minerule/Optimizer/OptimizedMinerule.h"

namespace minerule {

class IncrementalAlgorithm {
 protected:  
  const OptimizedMinerule* minerule;
 public:
  IncrementalAlgorithm(const OptimizedMinerule& mr) :
    minerule(&mr) {}

  virtual ~IncrementalAlgorithm() {}

  virtual void execute() throw(MineruleException,odbc::SQLException)=0;

  static IncrementalAlgorithm* 
  newIncrementalAlgorithm(const OptimizedMinerule& mr);
};


} //namespace

#endif

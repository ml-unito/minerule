#ifndef __MININGALGORITHM_BASE_H__
#define __MININGALGORITHM_BASE_H__

#include "AlgorithmsOptions.h"
#include "Optimizer/OptimizedMinerule.h"
#include "Utils/AlgorithmTypes.h"

namespace minerule {

/**
 * Base class from which each new mining algorithm will be derived
 */

class MiningAlgorithm {
 protected:
  const OptimizedMinerule& minerule;
 public:
  MiningAlgorithm( const OptimizedMinerule& mr ) : minerule(mr) {}
  virtual ~MiningAlgorithm() {}
  
  virtual void execute() {
    throw MineruleException( MR_ERROR_INTERNAL, "This method should never be executed!");
  }

  virtual bool needsGidSortedSourceTable() const {
    return false;
  }

  virtual bool needsCrossProductOfSourceTable() const {
    return false;
  }

  virtual bool canHandleMinerule() const {
    return false;
  }
  
  virtual const OptimizedMinerule& optimizedMinerule() const { return optimizedMinerule; }

  // Instantiate the algorithm specified by t
  static MiningAlgorithm* algorithmForType(AlgorithmTypes t, const OptimizedMinerule&) 
    throw(MineruleException);
};



}
#endif

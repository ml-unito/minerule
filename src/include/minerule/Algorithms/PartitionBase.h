#ifndef __PARTITIONBASE_H__
#define __PARTITIONBASE_H__

#include "MiningAlgorithmBase.h"

namespace minerule {

class PartitionBase : public MiningAlgorithm {
 protected:
  AlgorithmsOptions options;
  void prepareData();
 public:
  PartitionBase( const OptimizedMinerule& mr) : MiningAlgorithm(mr) {}
  virtual ~PartitionBase() {}

  virtual bool needsGidSortedSourceTable() const {
    return true;
  }

  virtual bool canHandleMinerule() const {
    return 
      minerule.getParsedMinerule().mc == NULL &&
      !minerule.getParsedMinerule().hasCrossConditions() &&
      !minerule.getParsedMinerule().requiresClusters() &&
      !minerule.getParsedMinerule().hasDisjuctionsInMC();
  }
  
  virtual void execute();
};

}

#endif

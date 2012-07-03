#ifndef __PARTITIONWITHCLUSTERS_H__
#define __PARTITIONWITHCLUSTERS_H__

#include "PartitionBase.h"

namespace minerule {

class PartitionWithClusters : public PartitionBase {
 public:
  PartitionWithClusters( const OptimizedMinerule& mr ) : PartitionBase(mr) {}
  virtual ~PartitionWithClusters() {}

  virtual bool needsCrossProductOfSourceTable() const {
    return true;
  }

  virtual bool canHandleMinerule() const {
    return true;
  }

  virtual void execute();
};

}
#endif

#ifndef __FPGROWTH_H__
#define __FPGROWTH_H__

#include "PartitionBase.h"

namespace minerule {

class FPGrowth : public PartitionBase {
 public:
  FPGrowth( const OptimizedMinerule& mr ) : PartitionBase(mr) {}
  virtual ~FPGrowth() {}

  virtual void execute();
  virtual bool needsGidSortedSourceTable() const {
    return true;
  }
};

}

#endif

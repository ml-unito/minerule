#include "MiningAlgorithmBase.h"
#include "PartitionWithClusters.h"
#include "BFSWithGidsNoCross.h"
#include "BFSWithGidsAndCross.h"
#include "Care.h"
#include "ConstrItemSetsExtraction.h"
#include "FPGrowth.h"
#include "PartitionBase.h"


MiningAlgorithm*
MiningAlgorithm::algorithmForType(AlgorithmTypes t, const OptimizedMinerule& mr) throw(MineruleException) {
  switch(t) {
  case ATNone:
    return new MiningAlgorithm(mr);
  case ATPartitionWithClusters:
    return new PartitionWithClusters(mr);
  case ATPartitionBase:
    return new PartitionBase(mr);
  case ATFPGrowth:
    return new FPGrowth(mr);
  case ATBFSWithGidsNoCross:
    return new BFSWithGidsNoCross(mr);
  case ATBFSWithGidsAndCross:
    return new BFSWithGidsAndCross(mr);
  case ATCare:
    return new CARE(mr);
  case ATConstrainedItemsets:
    return new ConstrItemSetsExtraction(mr);
  default: throw MineruleException( MR_ERROR_INTERNAL, "Requested unknown mining algorithm type");
  }
}

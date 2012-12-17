#include "minerule/Algorithms/MiningAlgorithmBase.h"
#include "minerule/Algorithms/BFSWithGidsNoCross.h"
#include "minerule/Algorithms/BFSWithGidsAndCross.h"
#include "minerule/Algorithms/Care.h"
#include "minerule/Algorithms/ConstrItemSetsExtraction.h"


MiningAlgorithmBase*
MiningAlgorithmBase::algorithmForType(AlgorithmTypes t, const OptimizedMinerule& mr) throw(MineruleException) {
  switch(t) {
  case ATNone:
    return new MiningAlgorithm(mr);
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

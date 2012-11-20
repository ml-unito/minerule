#include "Algorithms/MiningAlgorithmBase.h"
#include "Algorithms/BFSWithGidsNoCross.h"
#include "Algorithms/BFSWithGidsAndCross.h"
#include "Algorithms/Care.h"
#include "Algorithms/ConstrItemSetsExtraction.h"


MiningAlgorithm*
MiningAlgorithm::algorithmForType(AlgorithmTypes t, const OptimizedMinerule& mr) throw(MineruleException) {
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

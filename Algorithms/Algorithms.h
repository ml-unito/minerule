#ifndef __MINERULE_ALGORITHMS_H__
#define __MINERULE_ALGORITHMS_H__

#include "MRDatabase/sourcerow.h"
#include "MRDatabase/itemtype.h"

namespace minerule {
  // in different point of the algorithms the ItemSetType
  // is referred as HeadBodyType, we should clean this fact
  // sometime in the future, by replacing HeadBodyType with
  // ItemSetType everywhere.
  typedef ItemSetType HeadBodyType;
}

#include "Algorithms/AlgorithmsOptions.h"
#include "Algorithms/MiningAlgorithmBase.h"
#include "Optimizer/OptimizedMinerule.h"
#include "Utils/AlgorithmTypes.h"

namespace minerule {


class Algorithms {
 public:
  // returns a new MiningAlgorithm choosing among the available ones
  // using some criteria. One constraint it respects is that it returns
  // an algorithm which is able to handle clustering whenever the minerule
  // requires it
  static MiningAlgorithm* 
    newAlgorithm(const OptimizedMinerule& mr);

  // It execute the provided minerule and stores the results in the database
  static void executeMinerule(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException);

  static void executeExtractionAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException);
  static bool executeIncrementalAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException);

  static MiningAlgorithm*
    getBestRulesMiningAlgorithm(const OptimizedMinerule& mr);

  static MiningAlgorithm*
    getBestItemsetsMiningAlgorithm(const OptimizedMinerule& mr);

  static MiningAlgorithm*
    getBestSequencesMiningAlgorithm( const OptimizedMinerule& mr );

};

}


#endif

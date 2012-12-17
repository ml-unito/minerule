#ifndef __MINERULE_ALGORITHMS_H__
#define __MINERULE_ALGORITHMS_H__

#include "minerule/Database/SourceRow.h"
#include "minerule/Database/ItemType.h"
#include "minerule/Algorithms/AlgorithmsOptions.h"
#include "minerule/Algorithms/MiningAlgorithmBase.h"
#include "minerule/Optimizer/OptimizedMinerule.h"
#include "minerule/Utils/AlgorithmTypes.h"
#include <exception>

namespace minerule {

class Algorithms {
 public:
  // returns a new MiningAlgorithm choosing among the available ones
  // using some criteria. One constraint it respects is that it returns
  // an algorithm which is able to handle clustering whenever the minerule
  // requires it
  static MiningAlgorithmBase* newAlgorithm(const OptimizedMinerule& mr);

  // It execute the provided minerule and stores the results in the database
  static void executeMinerule(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException,std::exception);

  static void executeExtractionAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException,std::exception);
  
  static bool executeIncrementalAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException,std::exception);

  static MiningAlgorithmBase* getBestRulesMiningAlgorithm(const OptimizedMinerule& mr);

  static MiningAlgorithmBase* getBestItemsetsMiningAlgorithm(const OptimizedMinerule& mr);

  static MiningAlgorithmBase* getBestSequencesMiningAlgorithm( const OptimizedMinerule& mr );
  
private:
	static void checkAndHandleHomonymMinerules(OptimizedMinerule& mr) throw(MineruleException, odbc::SQLException, std::exception);
	static void showDebugInfo(const std::string& msg, OptimizedMinerule& mr);
};

}


#endif

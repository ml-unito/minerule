#ifndef __MINERULE_ALGORITHMS_H__
#define __MINERULE_ALGORITHMS_H__

#include "Database/SourceRow.h"
#include "Database/ItemType.h"
#include "Algorithms/AlgorithmsOptions.h"
#include "Algorithms/MiningAlgorithmBase.h"
#include "Optimizer/OptimizedMinerule.h"
#include "Utils/AlgorithmTypes.h"
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
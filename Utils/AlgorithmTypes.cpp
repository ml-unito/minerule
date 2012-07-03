#include "Utils/AlgorithmTypes.h"


namespace minerule {
  static const std::string taskNames[] = { "MineRules",
					   "MineItemsets",
					   "MineSequences" };
  


  static const std::string algoNames[] = {"None", 
					"PartitionWithClusters",
					"PartitionBase",
					"FPGrowth",
					"BFSWithGidsNoCross",
					"BFSWithGidsAndCross",
					"Care",
					"ConstrainedItemsets"};


  const std::string& miningTaskToString(MiningTasks mt) {
    return taskNames[mt];
  }


  MiningTasks stringToMiningTask(const std::string& s) {
    MiningTasks task;
    for(task=MTMineRules; task<MTEnd; task=static_cast<MiningTasks>(task+1) ) {
      if( taskNames[task]==s )
	return task;
    } 


    throw MineruleException( MR_ERROR_INTERNAL, "Cannot convert "+s+" to a valid MiningTask" );
  }

  std::string buildStringWithListOfMiningTasks() {
    std::string l;
    MiningTasks task;

    for(task=MTMineRules; task<MTEnd; task=static_cast<MiningTasks>(task+1)) {
      if(task!=MTMineRules)
	l+=",";
      l+=taskNames[task];
    }

    return l;
  }

  const std::string& stringWithListOfMiningTasks() {
    static std::string l = buildStringWithListOfMiningTasks();

    return l;
  }




  const std::string& algorithmTypeToString(AlgorithmTypes t) {
    return algoNames[t];
  }

  AlgorithmTypes stringToAlgorithmType(const std::string& s) throw(MineruleException) {
    AlgorithmTypes algoType;
    for( algoType=ATNone; algoType<ATEnd; algoType=static_cast<AlgorithmTypes>(algoType+1) )
      if(algoNames[algoType]==s)
	return algoType;

    throw MineruleException( MR_ERROR_INTERNAL, "Cannot convert "+s+" to a valid AlgorithmType" );
  }

  std::string buildStringWithListOfAlgorithmTypes() {
    std::string l;
    AlgorithmTypes algoType;

    for(algoType=ATNone; algoType<ATEnd; algoType=static_cast<AlgorithmTypes>(algoType+1)) {
      if(algoType!=ATNone)
	l+=",";
      l+=algoNames[algoType];
    }

    return l;
  }

  const std::string& stringWithListOfAlgorithmTypes() {
    static std::string l = buildStringWithListOfAlgorithmTypes();

    return l;
  }
};

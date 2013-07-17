//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "minerule/Utils/AlgorithmTypes.h"


namespace minerule {
  static const std::string taskNames[] = { 
	  				   "MineRules",
					   "MineItemsets",
					   "MineSequences" };
  


  static const std::string algoNames[] = {
	  				"None", 
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
    for( algoType=ATNone; algoType<ATEnd; algoType=static_cast<AlgorithmTypes>(algoType+1) ) {
      if(algoNames[algoType]==s) 
		  return algoType;
  	}

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

  std::string stringWithListOfAlgorithmTypes() {
    return buildStringWithListOfAlgorithmTypes();
  }
};

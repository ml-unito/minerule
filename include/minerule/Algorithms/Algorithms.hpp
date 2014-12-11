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
#ifndef __MINERULE_ALGORITHMS_H__
#define __MINERULE_ALGORITHMS_H__

#include "minerule/Database/SourceRow.hpp"
#include "minerule/Database/ItemType.hpp"
#include "minerule/Algorithms/AlgorithmsOptions.hpp"
#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Optimizer/OptimizedMinerule.hpp"
#include "minerule/Utils/AlgorithmTypes.hpp"
#include <exception>

namespace minerule {

class Algorithms {
 public:
  /// @return a new MiningAlgorithm choosen among the available ones
  /// using some criteria. One constraint it respects is that it returns
  /// an algorithm which is able to handle clustering whenever the minerule
  /// requires it
  static MiningAlgorithmBase* newAlgorithm(const OptimizedMinerule& mr);

  /// It executes the provided minerule and stores the results in the database. It exploits the optimization
  /// info to select the best option.
  static void executeMinerule(OptimizedMinerule& mr) ;

  /// It executes a rule extraction algorithm.
  static void executeExtractionAlgorithm(OptimizedMinerule& mr) ;

  /// It execute an incremental algorithm. It returns false if an algorithm with the required properties has not yet
  /// been implemented.
  static bool executeIncrementalAlgorithm(OptimizedMinerule& mr) ;

  /// @return an instance of the best rule mining algorithm available for the given mine rule.
  static MiningAlgorithmBase* getBestRulesMiningAlgorithm(const OptimizedMinerule& mr);

  /// @return an instance of the best itemset mining algorithm available for the given mine rule.
  static MiningAlgorithmBase* getBestItemsetsMiningAlgorithm(const OptimizedMinerule& mr);

  /// @return an instance of the best sequence mining algorithm available for the given mine rule.
  static MiningAlgorithmBase* getBestSequencesMiningAlgorithm( const OptimizedMinerule& mr );

private:
	static void checkAndHandleHomonymMinerules(OptimizedMinerule& mr) ;
	static void showDebugInfo(const std::string& msg, OptimizedMinerule& mr);
};

}


#endif

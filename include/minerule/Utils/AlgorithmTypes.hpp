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
#ifndef __ALGORITHMTYPES_H__
#define __ALGORITHMTYPES_H__

/**
 * Here the enumerated type AlgorithmTypes is defined along with few utils to 
 * work with it. Even though AlgorithmTypes should be located in the directory
 * Algorithms, this is not so for a very good reason: MineruleOptions depends on
 * it. If it were in Algorithms then anyone needing libUtils (i.e., everybody) would
 * be forced to link Algorithms (which is not always the case). Two alternatives are
 * possible (better said: ``I can see two alternatives''):
 *  1) to duplicate the information and then convert the types back and forth in order
 *     to keep the variables refferring to them updated.
 *  2) to move this header and the associated implementation file into Utils/
 */

#include <string>
#include "minerule/Utils/MineruleException.hpp"

namespace minerule {

  typedef enum {
    MTMineRules=0,
    MTMineItemsets,
    MTMineSequences,
    MTEnd
  } MiningTasks;


  typedef enum {
    ATNone=0, // dummy algorithm type
    ATBFSWithGidsNoCross,
    ATBFSWithGidsAndCross,
    ATCare,
    ATConstrainedItemsets,
    ATEnd // dummy algorithm type do not add algorithms
          // below this element
  } AlgorithmTypes;

  const std::string& miningTaskToString(MiningTasks mt);
  const std::string& stringWithListOfMiningTasks();

  const std::string& algorithmTypeToString(AlgorithmTypes t);

  AlgorithmTypes stringToAlgorithmType(const std::string& s) ;

  std::string stringWithListOfAlgorithmTypes();
}

#endif

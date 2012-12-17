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
#include "minerule/Utils/MineruleException.h"

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

  AlgorithmTypes stringToAlgorithmType(const std::string& s) throw(MineruleException);

  std::string stringWithListOfAlgorithmTypes();
}

#endif

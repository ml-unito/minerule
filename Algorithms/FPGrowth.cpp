#include "FPGrowth.h"
#include "AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h"


void newFPGrowth(const minerule::AlgorithmsOptions& algoOptions);
void origFPGrowth(const minerule::AlgorithmsOptions& algoOptions);

namespace minerule {

void FPGrowth::execute() {
  options.setMiningAlgorithmsOptions(MineruleOptions::getSharedOptions().getMiningAlgorithms());

  if( options.getMiningAlgorithmsOptions().getRulesMiningAlgorithms().getFPGrowth().getAlgoType() ==
      MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::FPGrowth::Original) {
    MRLog() << "This is FPGrowth Mining algorithm - Original version..." 
	    << endl;
    origFPGrowth(options);
  } else {
    MRLog() << "This is FPGrowth Mining algorithm - Anticipated pruning version..." 
	    << endl;

    prepareData();
    newFPGrowth(options);
  }
};

}


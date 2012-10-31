#include "Algorithms.h"
#include "MiningAlgorithmBase.h"
#include "Parsers/ParsedMinerule.h"
#include "PartitionWithClusters.h"
#include "PartitionBase.h"
#include "FPGrowth.h"
#include "Utils/MineruleOptions.h"
#include "IDIncrementalAlgorithm.h"
#include "ConstrTree.h"
#include "DestrTree.h"
#include "BFSWithGidsNoCross.h"
#include "BFSWithGidsAndCross.h"
#include "ConstrItemSetsExtraction.h"
#include "FSMiner.h"
#include "CCSMiner.h"

namespace minerule {

MiningAlgorithm*
Algorithms::getBestRulesMiningAlgorithm(const OptimizedMinerule& mr) {
  MRDebugPusher pusher("Choosing the best algorithm for the given MR");

  AlgorithmTypes userChoiceOfAT = 
    MineruleOptions::getSharedOptions().getMiningAlgorithms().getRulesMiningAlgorithms().getPreferredAlgorithm();

  MiningAlgorithm* userChoice= MiningAlgorithm::algorithmForType(userChoiceOfAT, mr);
  if( userChoice->canHandleMinerule() ) {
    MRDebug() << "Selected the algorithm given by the user preference" << endl;
    return userChoice;
  }
  else {
    MRDebug() << "User preference cannot be fulfilled" << endl;
    delete userChoice;
    userChoice=NULL;
  }

  // we failed to satisfy user preference, it is up to us
  // to find the best algorithm.

  if( BFSWithGidsNoCross(mr).canHandleMinerule() ) {
    MRDebug() << "Selected BFSWithGidsNoCross" << endl;
    return new BFSWithGidsNoCross(mr);
  }

  MRDebug() << "BFSWithGidsNoCross cannot handle it." << endl;

  if( BFSWithGidsAndCross(mr).canHandleMinerule() ) {
    MRDebug() << "Selected BFSWithGidsAndCross" << endl;
    return new BFSWithGidsAndCross(mr);
  }

  MRDebug() << "BFSWithGidsAndCross cannot handle it." << endl;


  if( PartitionWithClusters(mr).canHandleMinerule() ) {
    MRDebug() << "Selected PartitionWithClusters" << endl;
    return new PartitionWithClusters(mr);
  }
  
  MRDebug() << "Panic! No known algorithm can handle it." << endl;
  throw MineruleException( MR_ERROR_INTERNAL, "No known algorithm can handle the given minerule!" );
}

MiningAlgorithm*
Algorithms::getBestItemsetsMiningAlgorithm(const OptimizedMinerule& mr) {
  MRDebugPusher pusher("Choosing the best algorithm for the given MR");

  AlgorithmTypes userChoiceOfAT = 
    MineruleOptions::getSharedOptions().getMiningAlgorithms().getRulesMiningAlgorithms().getPreferredAlgorithm();

  MiningAlgorithm* userChoice= MiningAlgorithm::algorithmForType(userChoiceOfAT, mr);
  if( userChoice->canHandleMinerule() ) {
    MRDebug() << "Selected the algorithm given by the user preference" << endl;
    return userChoice;
  }
  else {
    MRDebug() << "User preference cannot be fulfilled" << endl;
    delete userChoice;
    userChoice=NULL;
  }

  // we failed to satisfy user preference, it is up to us
  // to find the best algorithm.

  if( ConstrItemSetsExtraction(mr).canHandleMinerule() ) {
    MRDebug() << "Selected ConstrItemSetsExtraction" << endl;
    return new ConstrItemSetsExtraction(mr);
  }
  
  MRDebug() << "Panic! No known algorithm can handle it." << endl;
  throw MineruleException( MR_ERROR_INTERNAL, "No known algorithm can handle the given minerule!" );
}


  /*MiningAlgorithm*
Algorithms::getBestSequencesMiningAlgorithm( const OptimizedMinerule& mr ) {
  AlgorithmsOptions opts;

  opts.setSupport( mr.getParsedMinerule().sup );
  opts.setBodyCardinalities( mr.getParsedMinerule().bodyCardinalities );

  FSMiner* miner = new FSMiner(mr,opts );
  if(miner->canHandleMinerule())
    return miner;
  else {
    delete miner;
    throw MineruleException( MR_ERROR_INTERNAL, "Cannot handle specified mine request." );
  }
  }*/

MiningAlgorithm*
Algorithms::getBestSequencesMiningAlgorithm( const OptimizedMinerule& mr ) {
  AlgorithmsOptions opts;

  opts.setSupport( mr.getParsedMinerule().sup );
  opts.setBodyCardinalities( mr.getParsedMinerule().bodyCardinalities );

  CCSMiner* miner = new CCSMiner(mr,opts);
  if(miner->canHandleMinerule())
    return miner;
  else {
    delete miner;
    throw MineruleException( MR_ERROR_INTERNAL, "Cannot handle specified mine request." );
  }
}

/**
 * This procedure crate a new algorithm and returns its reference.
 * As more algorithms become available it will choose among them
 * using some (hopefully) good heuristic... In modifying it please
 * remember that any future criteria must ensure that the chosen
 * algorithm is able to deal with HeadBodySourceRowDescription that
 * will be passed to it.
 * Remember that the informations stored in HeadBodySourceRowDescription
 * depends upon mr.mineruleRequiresClusters(). 
 */
MiningAlgorithm* 
Algorithms::newAlgorithm(const OptimizedMinerule& mr) {
  switch( mr.getParsedMinerule().miningTask ) {
  case MTMineRules:
    return getBestRulesMiningAlgorithm(mr);
  case MTMineItemsets:
    return getBestItemsetsMiningAlgorithm(mr);
  case MTMineSequences:
    return getBestSequencesMiningAlgorithm(mr);
  default:
    throw MineruleException( MR_ERROR_INTERNAL, "Cannot handle "+miningTaskToString(mr.getParsedMinerule().miningTask)+" mining task");
  }
}

bool
Algorithms::executeIncrementalAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException){
 
  IncrementalAlgorithm* incrAlgo = IncrementalAlgorithm::newIncrementalAlgorithm(mr);
  
  if( incrAlgo!=NULL ) {
    incrAlgo->execute();
    
    // thrashing the thrashable
    delete incrAlgo;
    return true;
  } else {
    MRLog() << "The needed incremental algorithms has not been integrated" << endl
	    << " to the system yet." << endl;
    return false;
  }
}



void
Algorithms::executeExtractionAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException) {
  MiningAlgorithm* algo;

  algo =  Algorithms::newAlgorithm(mr);

  algo->execute();
  delete algo;
}

void
Algorithms::executeMinerule(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException) {
   if( OptimizerCatalogue::existsMinerule(mr.getParsedMinerule().tab_result) ) {
      if( MineruleOptions::getSharedOptions().getSafety().getOverwriteHomonymMinerules() ) {
	MRLog() << "The optimizer Catalogue reports that a minerule " << endl;
	MRLog() << "having the same name as the one you gave already" << endl;
	MRLog() << "exists. I'm now going to delete the previous result as" << endl;
	MRLog() << "it has been specified in the option settings." << endl;
	
	OptimizerCatalogue::deleteMinerule(mr.getParsedMinerule().tab_result);
	
      } else
	throw MineruleException(MR_ERROR_MINERULE_ALREADY_EXISTS,
				"The Optimizer Catalogue reports that a minerule "
				"having the same name as the one you gave already"
				"exists. Please change the name of your current "
				"minerule or set the option "
				"safety::overwriteHomonymMinerules "
				"to true in your configuration file");
    }
  

  MRDebugPush("Unoptimized Minerule info");
  MRDebug() << "Optimized Minerule:[" << mr.getParsedMinerule().getText()<< "]" << endl;
  MRDebug() << "Body attribute list size:" << mr.getParsedMinerule().ba.size() << endl;
  MRDebug() << "Head attribute list size:" << mr.getParsedMinerule().ha.size() << endl;
  MRDebug() << "Rule attribute list size:" << mr.getParsedMinerule().ra.size() << endl;
  MRDebugPop();

  mr.optimize(); // internally it will check if the optimization option
                 // is set.

  MRDebugPush("Optimized Minerule info");
  MRDebug() << "Optimized Minerule:[" << mr.getParsedMinerule().getText()<< "]" << endl;
  MRDebug() << "Body attribute list size:" << mr.getParsedMinerule().ba.size() << endl;
  MRDebug() << "Head attribute list size:" << mr.getParsedMinerule().ha.size() << endl;
  MRDebug() << "Rule attribute list size:" << mr.getParsedMinerule().ra.size() << endl;
  MRDebugPop();


  string unsupportedRelation = "";
  OptimizerCatalogue::MineruleResultInfo result(mr.getParsedMinerule());

  switch(mr.getOptimizationInfo().relationship) {
  case OptimizedMinerule::Equivalence:
    {
      CatalogueInfo catInfo;
      OptimizerCatalogue::getMRQueryInfo( mr.getOptimizationInfo().minerule.tab_result, catInfo );
      result.resultset = catInfo.resName;
    
      MRDebug() << "Inclusion found with respect minerule:" << mr.getOptimizationInfo().minerule.getText() << endl;
      MRDebug() << "Current Minerule:" << result.getText() << endl;
      OptimizerCatalogue::addMineruleResult(result);
    }
    break;
  case OptimizedMinerule::Dominance:
    unsupportedRelation = "Dominance";
  case OptimizedMinerule::Combination:
    if(unsupportedRelation=="") {
      unsupportedRelation="Combination";
      
      if(executeIncrementalAlgorithm(mr)) {
	 
	OptimizerCatalogue::addMineruleResult(result);
	break;
      } else
	MRLog() << "The support for the found dominance relationship is not yet implemented"
		<< " switching back to the non-incremental mining algorithm;" << endl;
    }
  case OptimizedMinerule::Inclusion:
    if( unsupportedRelation=="" )
      unsupportedRelation = "Inclusion";
  case OptimizedMinerule::None:
    if( unsupportedRelation!="" ) {
      MRWarn()
	<< "The optimizer found that there exists a minerule in the catalogue" << endl
	<< "which is in `" <<unsupportedRelation<<"' relationship with the" << endl
	<< "current one. Unfortunately such kind of relationship is still" << endl
	<< "not supported and hence I will switch to the default algorithm" << endl;
    }
    executeExtractionAlgorithm(mr);
    if( mr.getParsedMinerule().miningTask==MTMineRules ||
	mr.getParsedMinerule().miningTask==MTMineItemsets)
      OptimizerCatalogue::addMineruleResult(result);
    break;
  default:
    throw MineruleException(MR_ERROR_INTERNAL, 
			    "Unexpected Relationship! This is a BUG! Please report it!");
  }
}


} // namespace minerule

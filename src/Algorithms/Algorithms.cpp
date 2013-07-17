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
#include "minerule/Algorithms/Algorithms.h"
#include "minerule/Algorithms/MiningAlgorithmBase.h"
#include "minerule/Parsers/ParsedMinerule.h"
#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Algorithms/IDIncrementalAlgorithm.h"
#include "minerule/Algorithms/ConstrTree.h"
#include "minerule/Algorithms/DestrTree.h"
#include "minerule/Algorithms/BFSWithGidsNoCross.h"
#include "minerule/Algorithms/BFSWithGidsAndCross.h"
#include "minerule/Algorithms/ConstrItemSetsExtraction.h"
#include "minerule/Algorithms/FSMiner.h"
#include "minerule/Algorithms/CCSMiner.h"
#include "minerule/Algorithms/STSMinerWithBitVector.h"

namespace minerule {

	MiningAlgorithmBase* Algorithms::getBestRulesMiningAlgorithm(const OptimizedMinerule& mr) {
		MRDebugPusher pusher("Choosing the best algorithm for the given MR");

		AlgorithmTypes userChoiceOfAT = MineruleOptions::getSharedOptions().getMiningAlgorithms().getRulesMiningAlgorithms().getPreferredAlgorithm();

		MiningAlgorithmBase* userChoice= MiningAlgorithm::algorithmForType(userChoiceOfAT, mr);
		if( userChoice->canHandleMinerule() ) {
			MRDebug() << "Selected the algorithm given by the user preference" << std::endl;
			return userChoice;
		}
		else {
			MRDebug() << "User preference cannot be fulfilled" << std::endl;
			delete userChoice;
			userChoice=NULL;
		}

  // we failed to satisfy user preference, it is up to us
  // to find the best algorithm.

		if( BFSWithGidsNoCross(mr).canHandleMinerule() ) {
			MRDebug() << "Selected BFSWithGidsNoCross" << std::endl;
			return new BFSWithGidsNoCross(mr);
		}

		MRDebug() << "BFSWithGidsNoCross cannot handle it." << std::endl;

		if( BFSWithGidsAndCross(mr).canHandleMinerule() ) {
			MRDebug() << "Selected BFSWithGidsAndCross" << std::endl;
			return new BFSWithGidsAndCross(mr);
		}

		MRDebug() << "BFSWithGidsAndCross cannot handle it." << std::endl;

  
		MRDebug() << "Panic! No known algorithm can handle it." << std::endl;
		throw MineruleException( MR_ERROR_INTERNAL, "No known algorithm can handle the given minerule!" );
	}
	
	

	MiningAlgorithmBase*
	Algorithms::getBestItemsetsMiningAlgorithm(const OptimizedMinerule& mr) {
		MRDebugPusher pusher("Choosing the best algorithm for the given MR");

		AlgorithmTypes userChoiceOfAT = 
			MineruleOptions::getSharedOptions().getMiningAlgorithms().getRulesMiningAlgorithms().getPreferredAlgorithm();

		MiningAlgorithmBase* userChoice= MiningAlgorithm::algorithmForType(userChoiceOfAT, mr);
		if( userChoice->canHandleMinerule() ) {
			MRDebug() << "Selected the algorithm given by the user preference" << std::endl;
			return userChoice;
		}
		else {
			MRDebug() << "User preference cannot be fulfilled" << std::endl;
			delete userChoice;
			userChoice=NULL;
		}

  // we failed to satisfy user preference, it is up to us
  // to find the best algorithm.

		if( ConstrItemSetsExtraction(mr).canHandleMinerule() ) {
			MRDebug() << "Selected ConstrItemSetsExtraction" << std::endl;
			return new ConstrItemSetsExtraction(mr);
		}
  
		MRDebug() << "Panic! No known algorithm can handle it." << std::endl;
		throw MineruleException( MR_ERROR_INTERNAL, "No known algorithm can handle the given minerule!" );
	}



	MiningAlgorithmBase*
	Algorithms::getBestSequencesMiningAlgorithm( const OptimizedMinerule& mr ) {
		AlgorithmsOptions opts;

		opts.setSupport( mr.getParsedMinerule().sup );
		opts.setBodyCardinalities( mr.getParsedMinerule().bodyCardinalities );

		//CCSMiner* miner = new CCSMiner(mr,opts);
		STSMinerWithBitVector* miner= new STSMinerWithBitVector(mr,opts);
		if(miner->canHandleMinerule())
			return miner;
		else {
			delete miner;
			throw MineruleException( MR_ERROR_INTERNAL, "Cannot handle specified mine request." );
		}
	}

// 
// 	This procedure crate a new algorithm and returns its reference.
// 	As more algorithms become available it will choose among them
// 	using some (hopefully) good heuristic... In modifying it please
// 	remember that any future criteria must ensure that the chosen
// 	algorithm is able to deal with SourceRowColumnIds that
// 	will be passed to it.
// 	Remember that the informations stored in SourceRowColumnIds
// 	depends upon mr.mineruleRequiresClusters(). 
//  
	MiningAlgorithmBase* Algorithms::newAlgorithm(const OptimizedMinerule& mr) {
		switch( mr.getParsedMinerule().miningTask ) {
			case MTMineRules: 		return getBestRulesMiningAlgorithm(mr);
			case MTMineItemsets:  	return getBestItemsetsMiningAlgorithm(mr);
			case MTMineSequences: 	return getBestSequencesMiningAlgorithm(mr);
			default: throw MineruleException( MR_ERROR_INTERNAL, "Cannot handle "+miningTaskToString(mr.getParsedMinerule().miningTask)+" mining task");
		}
	}

	bool Algorithms::executeIncrementalAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException, std::exception){
 
		IncrementalAlgorithm* incrAlgo = IncrementalAlgorithm::newIncrementalAlgorithm(mr);
  
		if( incrAlgo!=NULL ) {
			incrAlgo->execute();

			delete incrAlgo;
			return true;
		} else {
			MRLog() << "The needed incremental algorithms has not been integrated" << std::endl
					<< " to the system yet." << std::endl;
			return false;
		}
	}



	void Algorithms::executeExtractionAlgorithm(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException, std::exception) {
		MiningAlgorithmBase* algo =  Algorithms::newAlgorithm(mr);
		algo->execute();
		delete algo;
	}

	void Algorithms::checkAndHandleHomonymMinerules(OptimizedMinerule& mr) throw(MineruleException, odbc::SQLException, std::exception) {
		if( OptimizerCatalogue::existsMinerule(mr.getParsedMinerule().tab_result) ) {
			if( MineruleOptions::getSharedOptions().getSafety().getOverwriteHomonymMinerules() ) {
				MRLog() << "The optimizer Catalogue reports that a minerule " << std::endl;
				MRLog() << "having the same name as the one you gave already" << std::endl;
				MRLog() << "exists. I'm now going to delete the previous result as" << std::endl;
				MRLog() << "it has been specified in the option settings." << std::endl;
	
				OptimizerCatalogue::deleteMinerule(mr.getParsedMinerule().tab_result);
	
			} else {
				throw MineruleException(MR_ERROR_MINERULE_ALREADY_EXISTS,
					"The Optimizer Catalogue reports that a minerule "
					"having the same name as the one you gave already"
					"exists. Please change the name of your current "
					"minerule or set the option "
					"safety::overwriteHomonymMinerules "
					"to true in your configuration file");
			}
		}		
	}
	
	void Algorithms::showDebugInfo(const std::string& msg, OptimizedMinerule& mr) {
		MRDebugPush("Unoptimized Minerule info");
		MRDebug("Optimized Minerule:[" + mr.getParsedMinerule().getText() + "]");
		MRDebug() << "Body attribute list size:" << mr.getParsedMinerule().ba.size() << std::endl;
		MRDebug() << "Head attribute list size:" << mr.getParsedMinerule().ha.size() << std::endl;
		MRDebug() << "Rule attribute list size:" << mr.getParsedMinerule().ra.size() << std::endl;
		MRDebugPop();
	}

	void Algorithms::executeMinerule(OptimizedMinerule& mr) throw(MineruleException,odbc::SQLException, std::exception) {
		checkAndHandleHomonymMinerules(mr);

		showDebugInfo("Unoptimized Minerule info", mr);
		mr.optimize(); // internally it will check if the optimization option is set.
		showDebugInfo("Optimized Minerule info", mr);


		std::string unsupportedRelation = "";
		OptimizerCatalogue::MineruleResultInfo result(mr.getParsedMinerule());

		switch(mr.getOptimizationInfo().relationship) {

			// EQUIVALENCE
			case OptimizedMinerule::Equivalence:
				{
					MRLog("Using equivalence relationship.");
					
					CatalogueInfo catInfo;
					OptimizerCatalogue::getMRQueryInfo( mr.getOptimizationInfo().minerule.tab_result, catInfo );
					result.resultset = catInfo.resName;
    
					MRDebug() << "Inclusion found with respect minerule:" << mr.getOptimizationInfo().minerule.getText() << std::endl;
					MRDebug() << "Current Minerule:" << result.getText() << std::endl;
					OptimizerCatalogue::addMineruleResult(result);
				}
				break;

			// DOMINANCE
			case OptimizedMinerule::Dominance:
				unsupportedRelation = "Dominance";
				MRLog("Using dominance relationship.");
			
			// COMBINATION
			case OptimizedMinerule::Combination:
				if(unsupportedRelation=="") {
					MRLog("Using combination relationship.");
					
					unsupportedRelation="Combination";
      			}
				
				if(executeIncrementalAlgorithm(mr)) {	 
					OptimizerCatalogue::addMineruleResult(result);
					break;
				} else {
					MRLog() << "The support for the found dominance relationship is not yet implemented"
						<< " switching back to the non-incremental mining algorithm;" << std::endl;
				}
			
			// INCLUSION
			case OptimizedMinerule::Inclusion:
				if( unsupportedRelation=="" ) {
					unsupportedRelation = "Inclusion";
					MRLog("Using inclusion relationship.");					
				}
				
			// NO RELATION FOUND
			case OptimizedMinerule::None:
				if( unsupportedRelation!="" ) {
					MRWarn() << "The optimizer found that there exists a minerule in the catalogue" << std::endl
						<< "which is in `" << unsupportedRelation <<"' relationship with the" << std::endl
						<< "current one. Unfortunately such kind of relationship is still" << std::endl
						<< "not supported and hence I will switch to the default algorithm" << std::endl;
				}
				
				executeExtractionAlgorithm(mr);
				
				if( mr.getParsedMinerule().miningTask==MTMineRules || mr.getParsedMinerule().miningTask==MTMineItemsets )
						OptimizerCatalogue::addMineruleResult(result);
				break;
				
			// ERROR
			default:
				throw MineruleException(MR_ERROR_INTERNAL, "Unexpected Relationship! This is a BUG! Please report it!");
		}
	}


} // namespace minerule

#include "Algorithms/IncrementalAlgorithm.h"
#include "Algorithms/IDIncrementalAlgorithm.h"
#include "Algorithms/ResultCombinator.h"
#include "Algorithms/ConstrTree.h"
#include "Algorithms/DestrTree.h"

namespace minerule {

	IncrementalAlgorithm* 
	IncrementalAlgorithm::newIncrementalAlgorithm(const OptimizedMinerule& mr) {
    // if mr has only ItemDependent constraints
		MRLog() << "Checking if the current minerule is item dependent..." << std::endl;
    
		if( mr.getParsedMinerule().hasIDConstraints() ) {
			MRLog() << "The minerule is item dependent!" << std::endl;
			if( mr.getOptimizationInfo().relationship==OptimizedMinerule::Combination ) 
				return new ResultCombinator(mr);
			else 
				return new IDIncrementalAlgorithm(mr);
		}
    
		MRLog() << "The minerule is NOT item dependent!" << std::endl;
    
		if( mr.getParsedMinerule().mc!=NULL && mr.getParsedMinerule().mc->next==NULL ) {
      
			IncrementalAlgorithm* incrAlgo = NULL;
      
			switch( MineruleOptions::getSharedOptions().getOptimizations().getIncrementalAlgorithm() ) {
				case MineruleOptions::Optimizations::ConstructiveAlgo:
					incrAlgo = new ConstrTree(mr);
					break;
				case MineruleOptions::Optimizations::DestructiveAlgo:
					incrAlgo = new DestrTree(mr);
					break;
				case MineruleOptions::Optimizations::AutochooseIncrAlgo:
					incrAlgo = new ConstrTree(mr);
					break;
			}
      
			return incrAlgo;
		} else {
			MRLog() << "The needed incremental algorithms has not been integrated" << std::endl
				<< " to the system yet." << std::endl;
			return NULL;
		}
	}
	
} // namespace

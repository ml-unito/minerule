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
#include "minerule/Algorithms/IncrementalAlgorithm.hpp"
#include "minerule/Algorithms/IDIncrementalAlgorithm.hpp"
#include "minerule/Algorithms/ResultCombinator.hpp"
#include "minerule/Algorithms/ConstrTree.hpp"
#include "minerule/Algorithms/DestrTree.hpp"

namespace minerule {

	IncrementalAlgorithm* 
	IncrementalAlgorithm::newIncrementalAlgorithm(const OptimizedMinerule& mr) {
    // if mr has only ItemDependent constraints
		MRLog() << "Checking if the current minerule is item dependent..." << std::endl;
    
		if( mr.hasIDConstraints() ) {
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

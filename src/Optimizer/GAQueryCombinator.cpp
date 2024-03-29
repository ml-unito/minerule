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
#include "minerule/Optimizer/GAQueryCombinator.hpp"
#include "minerule/PredicateUtils/PredicateUtils.hpp"
#include <iterator>
#include <cmath>
#include <algorithm>

#ifndef bit_vector
	#define bit_vector std::vector<bool>
#endif

namespace minerule {

	const int HAM_LOSS   = 10;
	const int DISJ_LOSS  = 1;
	const int QUERY_LOSS = 1;

	void GAQueryCombinator::buildAssociatedPredicate(const GAGenome& g, Predicate& p,
				size_t& numUsedDisjuncts, size_t& numUsedQueries, size_t& numAssertedBits) const {
		size_t last_used_disj = 1<<10;

		const GA1DBinaryStringGenome& genome = dynamic_cast<const GA1DBinaryStringGenome&>(g);
		bit_vector usedQueries(mr_candidates.size());

		for(size_t i=0; i<maxDisjuncts; i++) {
			Predicate innerPred;

			for(size_t j=0; j<mr_candidates.size(); j++) {
				assert(i*mr_candidates.size()+j<size_t(genome.length()));

				int QjAsserted = genome.gene(i*mr_candidates.size()+j);

				if(QjAsserted) {
					numAssertedBits++;
					usedQueries[j]=1;
					if(last_used_disj!=i) {
						last_used_disj=i;
						numUsedDisjuncts++;
					}

					if(innerPred.empty()) {
						innerPred |= mr_candidates[j];
					} else {
						innerPred &= mr_candidates[j];
					}
				}
			}
      
			p |= innerPred;
		}

		for(size_t i=0; i<usedQueries.size(); i++) {
			numUsedQueries+=usedQueries[i];
		}

	}



	float GAQueryCombinator::evaluator(GAGenome& g) {
		GAQueryCombinator* qc = (GAQueryCombinator*) g.userData();
		assert(qc!=NULL);
		
		if((clock()-qc->startingTime)/CLOCKS_PER_SEC > qc->timeOut )
			throw TimeOutException();

		size_t numUsedDisjuncts=0;
		size_t numUsedQueries=0;
		size_t numAssertedBits=0;

		Predicate p2;
		qc->buildAssociatedPredicate(g,p2, numUsedDisjuncts, numUsedQueries, numAssertedBits);

		if(numUsedDisjuncts > qc->maxDisjuncts ||	numUsedQueries > qc->maxQueries) 
				return 0;

		Predicate& p1=qc->mr_target;

    // The following two lines assign unique variables id
    // to elements in p1,p2. It do so setting the variable id
    // for each element of the union (through ci) to the position
    // in which the predicate appear in the union itself. Note
    // that this work only because two equivalent predicates are
    // indeed two references to the same object (look at SimplePredicate
    // class to understand why this is true).
		size_t counter=0;
		list_AND_node* allPreds=NULL;

		CountingIterator ci(counter,allPreds);
		std::set<SimplePredicate*, PtrSimplePredComp>& sp1 = p1.getPredicateList();
		std::set<SimplePredicate*, PtrSimplePredComp>& sp2 = p2.getPredicateList();

		std::set_union( sp1.begin(), sp1.end(), 
			sp2.begin(), sp2.end(),	ci );


		p1.setNumVariables(counter);
		p2.setNumVariables(counter);

		InvalidConfigurationFilter filter( qc->tab_source, allPreds );

		ExpressionNFCoder e1(filter);
		ExpressionNFCoder e2(filter);
  
		float ham_distance=EncodedNF::computeHammingDistance(e1.encode(p1),e2.encode(p2));

		CountingIterator::delete_list_AND_node(allPreds);

		if( ham_distance==0 )
			return HUGE_VAL;

		return ( (1 << qc->maxDistinctPredicates)* HAM_LOSS 
			+ qc->mr_candidates.size()*qc->maxDisjuncts - ham_distance*HAM_LOSS - numAssertedBits);

	}


	GABoolean
	GAQueryCombinator::TerminationCriterion(GAGeneticAlgorithm & ga) {  
		if(ga.statistics().current( GAStatistics::Maximum )==HUGE_VAL)
			return gaTrue;
		else
			return (ga.generation() < ga.nGenerations() ? gaFalse : gaTrue);
	}



	void GAQueryCombinator::evolve() {
    // create a genome
		GA1DBinaryStringGenome genome(mr_candidates.size()*maxDisjuncts, GAQueryCombinator::evaluator, this); 
    // create the genetic algorithm
		GASimpleGA ga(genome);
		ga.populationSize(10);
		ga.nGenerations(100);
		ga.pCrossover(0.8);
		ga.pMutation(0.05);
		ga.terminator(TerminationCriterion);
    // do the evolution

		startingTime = clock();
		ga.evolve();

		buildResult(ga);
	}


	void GAQueryCombinator::buildResult(const GAGeneticAlgorithm& ga) {
		result.clear();

		if(ga.statistics().current(GAStatistics::Maximum)!=HUGE_VAL)
			return;

		size_t conjSize = mr_candidates.size();

		const GA1DBinaryStringGenome& solution = dynamic_cast<const GA1DBinaryStringGenome&>(ga.population().best());

		for(size_t i=0; i<maxDisjuncts; i++) {
			QueryAndList currAnd;

			for(size_t j=0; j<conjSize; j++) {
				if( solution.gene(i*conjSize+j) )
					currAnd.push_back(j);
			}

			if(!currAnd.empty())
				result.push_back(currAnd);
		}
	}

	const GAQueryCombinator::QueryOrList& GAQueryCombinator::getResult() const {
		return result;
	}
    
}







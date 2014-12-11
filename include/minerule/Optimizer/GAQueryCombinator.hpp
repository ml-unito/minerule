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
#include <iostream>
#include <ga/GA1DBinStrGenome.h>
#include <ga/GASimpleGA.h>
#include "minerule/PredicateUtils/Predicate.hpp"

 /**
   * This module provides an algorithm that searches for a combination
   * of queries which yelds a result equivalent to a fixed one. The
   * search algorithm is a genetic one having the following
   * characteristics: GENOMA: bistd::string of n*m bits. It is interpreted
   * as a boolean formula having m disjunctions, each disjunct has n
   * conjuncts. If a disjunct does not have any bit set to one, then
   * it is empty and is not considered.  For instance, let us consider
   * the n=4 queries: Q1, Q2, Q3, Q4 and let us assume that m=2. We
   * need 4*8 bits in order to represent this genoma.  A possible
   * configuration is 0101 1100 corresponding to the formula: Q2 and
   * Q4 or Q1 and Q2. FITTING FUNCTION: The fitting function is the
   * editing distance between the truth table of the formula
   * represented in the genoma and the truth table of the target
   * query.  PENALIZATIONS: A number of penalizations can be proposed,
   * one that seems plausible is one which penalizes: the use of
   * disjunctions, and the use of many different queries.  For instance, let us
   * denote with k<=m the number of disjunctions in the formula and
   * with t<=n the number of different queries exploited.  A possible
   * fitting function may be: -P0*ham - P1*k - P2*t where ham is the
   * Hamming distance, and P0,P1,P2 are the penalities associated to
   * ham,k, and t respectively.
   */


namespace minerule {
  class TimeOutException : public std::exception { 
  };

  
  /**
   * This class implements the GA logic. It requires a vector V of
   * predicates to be specified along with a target predicate Q. The
   * predicates in the vector are and'ed and or'ed until a formula equivalent
   * to predicate Q is found. The result R is returned as a vector of
   * int vectors. R represents a disjunctive formula, i.e. R[0] represent
   * the first conjunct, R[1] represent the second conjunct and so on. Thus
   * R[0] or R[1] or ... or R[R.size()-1] represent the result of the
   * algorithm.
   * 
   * Each int vector V represent a conjunction, element j being set to
   * the index of the predicate that is used in the conjunction. The indexes
   * refer to the predicates in mr_candidates (C in the following),
   * and hence the resulting predicate can be written as:
   *    C(R[0][0]) and C(R[0][1]) ... and C(R[0][R[0].size()-1]) or
   *    C(R[1][0]) and C(R[1][1]) ... and C(R[1][R[1].size()-1]) or
   *    ...
   *    C(R[R.size()-1][0]) and C(R[R.size()-1][1]) ... and 
   *       C(R[R.size()-1][R[R.size()-1].size()-1])
   */ 

  class GAQueryCombinator {
    float  startingTime;

  public:
    /**
     * And of queries. 
     */
    typedef std::vector<size_t> QueryAndList;
    /**
     * Or of queries. It is the type used to return the result to the user.
     */
    typedef std::vector<QueryAndList> QueryOrList;
  private:
    /**
     * Target minerule.
     */
    Predicate& mr_target;
    /**
     * Set of query candidates (the ones that should be combined).
     */
    std::vector<Predicate>& mr_candidates;
    
    /**
     * The result of the GA algorithm will be stored in this vector
     * and returned to the user thru the getResult function.
     */
    QueryOrList result;

    /**
     * Thisstd::string is needed by the PredicateFilter class that is used
     * in order to compute the Hamming distance.
     */
    std::string tab_source;

    /**
     * Number of seconds before a time-out exception is thrown. I.e., the
     * amount of time the search is allowed to run
     */
    float  timeOut;

    /**
     * Max number of disjuncts. It is the number of disjuncts it is considered
     * during the search. Notice that increasing this number has a strong impact
     * on the dimension of the search space.
     */ 
    size_t maxDisjuncts;

    /**
     * Max number of queries. Max number of distinct queries the user allows to
     * be combined in the result. Formulae with a larger number of queries are
     * penalized in the evaluation function.
     */
    size_t maxQueries;

    /**
     * Max distinct predicates. Max number of distinct predicates that the user
     * allows. This afflict the response time: the time spent in assessing each
     * formula grows exponentially fast with the number of predicates.
     */
    size_t maxDistinctPredicates;

    /**
     * Returns in p a combination of the queries in mr_candidates.
     * How to combine the queries is decided by means of g.
     * @param g the GA1DBinaryStringGenome used to decide how to 
     *          combine queries.
     * @param p (output) the resulting predicate.
     * @param numUsedDisjuncts (output) the number of non-empty disjunct
     *                         found in g
     * @param numUsedQueries (output) the number of distinct queries found
     *                       in g
     */

    void
      buildAssociatedPredicate(const GAGenome& g, 
					    Predicate& p,
					    size_t& numUsedDisjuncts,
					    size_t& numUsedQueries,
					    size_t& numAssertedBits) const;
    
    /**
     * It builds the QueryOrList representation of the solution
     * expressed found by ga.
     */
    void buildResult(const GAGeneticAlgorithm& ga);
  public:
    /**
     * GAQueryCombinator constructor. Notice that a reference to
     * _mr_target and one to _mr_candidates are stored into the class. Hence,
     * the user must be sure to not delete those object before this class
     * has been destroyed.
     *
     * The constructor sets the following default values for the search
     * parameters:
     *   timeOut=4
     *   maxDisjuncts=3
     *   maxQueries=5
     *   maxDistinctPredicates=10
     */

    GAQueryCombinator(Predicate& _mr_target, std::vector<Predicate>& _mr_candidates)
      : mr_target(_mr_target), 
        mr_candidates(_mr_candidates),
        timeOut(4.0),
        maxDisjuncts(3),
        maxQueries(5),
        maxDistinctPredicates(10) {}	 

    virtual ~GAQueryCombinator() {}
	
   /**
    * This is the fitness function. 
    * @param g the GA1DBinaryStringGenome object representing the genome
    *          that need to be evaluated
    * @return the score of this genome (in the current implementation the
    *         best score is zero, being all the others negative numbers.
    */
   static float evaluator(GAGenome& g);
	
    
    /**
     * The termination criterion for the GA.
     */
    static GABoolean
      TerminationCriterion(GAGeneticAlgorithm & ga);

    /**
     * The main algorithm. It starts the genetic algorithm and fills
     * the result object.
     */
    void evolve() ;

    /**
     * Returns the result of the GA algorithm.
     */
    const QueryOrList& getResult() const;

    //
    // -- Parameters
    //

    float getTimeOutThreshold() const { return timeOut; }
    size_t getMaxDisjuncts() const {return maxDisjuncts;}
    size_t getMaxQueries() const {return maxQueries;}
    size_t getMaxDistinctPredicates() const {return maxDistinctPredicates;}

    void setMaxDisjuncts(size_t m) { maxDisjuncts=m; }
    void setMaxQueries(size_t m) { maxQueries=m; }
    void setMaxDistinctPredicates(size_t m) { maxDistinctPredicates=m; }
    void setTimeOutThreshold(float to) { timeOut=to; }
  };


}




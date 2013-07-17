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
#ifndef __MROPTIMIZER_H__
#define __MROPTIMIZER_H__

#include "minerule/Parsers/ParsedMinerule.h"
#include "minerule/Database/SourceRow.h"
#include "minerule/Optimizer/GAQueryCombinator.h"


namespace minerule {

  class OptimizedMinerule  {
  public:
    typedef enum  {
      None,            // No relationship can be found
      Dominance,       // There exists a query which dominate the current one
      Inclusion,       // There exists a query which include the current one
      Equivalence,     // There exists a query equivalent to the current one
      Combination      // There exists a set of queries that can be combined
                       // into one equivalent to the current one
    } MineruleRelationship;

    class OptimizationInfo {
    public:
      MineruleRelationship relationship;
      ParsedMinerule minerule;

      // used only in case relationship==Combination, it contains the set of queries
      // which need to be combined in order to form the equivalent one
      std::vector<ParsedMinerule> minerulesToCombine; 
      // Specifies how the queries have to be combined
      GAQueryCombinator::QueryOrList combinationFormula;
    };

  private:
    ParsedMinerule minerule;
    OptimizationInfo optInfo;
    bool sortSourceTableForGids;

  protected:
    void 
      dropTableIfExists(odbc::Connection*, const std::string& tname) const;

    // ??? with respect to what the relationship should be???
    MineruleRelationship getRelationshipType();

    static void attributesInPredicate( const list_OR_node* l, std::set<std::string>& result );
    static bool predicateAttributesAreIncluded( const ParsedMinerule& mr1, const ParsedMinerule& mr2 );
    void checkForCombinedQueries(  );

  public:
    OptimizedMinerule(const std::string& minerule_text) {
      minerule.init(minerule_text);
      optInfo.relationship = None;
    }

    void optimize();
  
    const ParsedMinerule& getParsedMinerule() const {
      return minerule;
    }

    const OptimizationInfo&
      getOptimizationInfo() const {
      return optInfo;
    }

    OptimizationInfo&
      getOptimizationInfo() {
      return optInfo;
    }
		
		bool hasIDConstraints() const throw(MineruleException);

    static bool isACandidateQuery( const ParsedMinerule& candidate, const ParsedMinerule& target );
	static bool firstMineruleIncludesSecondMinerule(const ParsedMinerule&, const ParsedMinerule&);
	static bool firstMineruleDominatesSecondMinerule(const ParsedMinerule&, const ParsedMinerule&);

    // Returns None || Dominance || Inclusion || Equivalence
    // the relation is to be intended as: "the first RELATION the second"
    // i.e. if Dominance is returned it means: "the first dominates the second".
    static MineruleRelationship getMineruleRelationship(const ParsedMinerule&, const ParsedMinerule&);
  };

} // namespace 
#endif

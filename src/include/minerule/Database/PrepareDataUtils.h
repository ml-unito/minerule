#ifndef __PREPAREDATAUTILS_H__
#define __PREPAREDATAUTILS_H__

#include <string>
#include "Parsers/ParsedMinerule.h"
#include "Database/SourceRowColumnIds.h"
#include "Optimizer/OptimizedMinerule.h"
#include "Algorithms/MiningAlgorithmBase.h"

namespace minerule {

class PrepareDataUtils {
 private:
  const OptimizedMinerule& mr;
  const MiningAlgorithm& miningAlgo;
 public:
  PrepareDataUtils( const OptimizedMinerule& m,
		    const MiningAlgorithm& a) : mr(m), miningAlgo(a) {}

   std::string
    buildAndList(const list_AND_node* l) const;

    std::string
      buildConditionFilter(const list_OR_node*) const;

   std::string
      createSourceTable() const;

   static std::string
      buildAttrListDescription(const ParsedMinerule::AttrVector& attrs,
			       const std::string& alias="", 
			       bool addColAlias=false);

   static std::string
      buildAttrListAlias(const ParsedMinerule::AttrVector& attrs,
			       const std::string& alias="", 
			       bool addColAlias=false);

   std::string
      buildAttrListEquiJoin(const std::string& alias1,
			    const std::string& alias2) const;

   void
      buildSimpleSourceTableQuery(string& queryText, SourceRowColumnIds& rowDes) const;
   
   void
     buildExtendedSourceTableQuery(string& queryText, SourceRowColumnIds& rowDes) const;


   void
    buildSourceTableQuery(string& queryText, SourceRowColumnIds& rowDes) const;
  

   static void
     dropTableIfExists(odbc::Connection* conn, const std::string& tname);

    /** 
     * Query the DBMS in order to get the total number of groups
     * contained in the source table (BEFORE filtering it).
     * It returns that value if everything is ok, it throws a
     * MineruleException(MR_DATABASEERROR) if it cannot access to
     * that number after the query. It may
     * also throw an SQLException in case something goes wrong 
     * during the query evaluation.
     */
   static size_t evaluateTotGroups(const ParsedMinerule& pmr) throw(MineruleException, odbc::SQLException);

   size_t evaluateTotGroups() const throw(MineruleException, odbc::SQLException) {
     return evaluateTotGroups(mr.getParsedMinerule());
   }
};

} // namespace

#endif

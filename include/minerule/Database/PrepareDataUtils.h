#ifndef __PREPAREDATAUTILS_H__
#define __PREPAREDATAUTILS_H__

#include <string>
#include "minerule/Parsers/ParsedMinerule.h"
#include "minerule/Database/SourceRowColumnIds.h"
#include "minerule/Optimizer/OptimizedMinerule.h"
#include "minerule/Algorithms/MiningAlgorithmBase.h"

namespace minerule {

	class PrepareDataUtils {
	private:
		const ParsedMinerule& mr;
		SourceTableRequirements sourceTableRequirements;
		
		std::string buildAndList(const list_AND_node* l) const;

		std::string buildConditionFilter(const list_OR_node*) const;

		std::string createSourceTable() const;

		static std::string buildAttrListAlias(const ParsedMinerule::AttrVector& attrs, const std::string& alias="",bool addColAlias=false);

		std::string	buildAttrListEquiJoin(const std::string& alias1, const std::string& alias2) const;		
	public:
		PrepareDataUtils( const ParsedMinerule& m, const SourceTableRequirements& requirements) : mr(m), sourceTableRequirements(requirements) {}


		std::string buildBodyTableQuery(SourceRowColumnIds& rowDes, const std::string& condition) const;
		std::string buildHeadTableQuery(SourceRowColumnIds& rowDes, const std::string& condition) const;
		std::string buildExtendedSourceTableQuery(SourceRowColumnIds& rowDes) const;

		std::string buildSourceTableQuery(SourceRowColumnIds& rowDes) const;
		
		static std::string buildAttrListDescription(const ParsedMinerule::AttrVector& attrs, const std::string& alias="", bool addColAlias=false);

		static void	dropTableIfExists(odbc::Connection* conn, const std::string& tname);

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
			return evaluateTotGroups(mr);
		}
	};

} // namespace

#endif

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
#ifndef __PREPAREDATAUTILS_H__
#define __PREPAREDATAUTILS_H__

#include <string>
#include "minerule/Parsers/ParsedMinerule.hpp"
#include "minerule/Database/SourceRowColumnIds.hpp"
#include "minerule/Optimizer/OptimizedMinerule.hpp"
#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/mrdb/Connection.hpp"

namespace minerule {

class PrepareDataUtils {
private:
  const ParsedMinerule &mr;
  SourceTableRequirements sourceTableRequirements;

  std::string buildAndList(const list_AND_node *l) const;

  std::string buildConditionFilter(const list_OR_node *) const;

  std::string createSourceTable() const;

  static std::string buildAttrListAlias(const ParsedMinerule::AttrVector &attrs,
                                        const std::string &alias = "",
                                        bool addColAlias = false);

  std::string buildAttrListEquiJoin(const std::string &alias1,
                                    const std::string &alias2) const;

public:
  PrepareDataUtils(const ParsedMinerule &m,
                   const SourceTableRequirements &requirements)
      : mr(m), sourceTableRequirements(requirements) {}

  std::string buildBodyTableQuery(SourceRowColumnIds &rowDes,
                                  const std::string &condition) const;
  std::string buildHeadTableQuery(SourceRowColumnIds &rowDes,
                                  const std::string &condition) const;
  std::string buildExtendedSourceTableQuery(SourceRowColumnIds &rowDes) const;

  std::string buildSourceTableQuery(SourceRowColumnIds &rowDes) const;

  static std::string
  buildAttrListDescription(const ParsedMinerule::AttrVector &attrs,
                           const std::string &alias = "",
                           bool addColAlias = false);

  static void dropTableIfExists(mrdb::Connection *conn,
                                const std::string &tname);

  /**
   * Query the DBMS in order to get the total number of groups
   * contained in the source table (BEFORE filtering it).
   * It returns that value if everything is ok, it throws a
   * MineruleException(MR_DATABASEERROR) if it cannot access to
   * that number after the query. It may
   * also throw an SQLException in case something goes wrong
   * during the query evaluation.
   */

  static size_t
  evaluateTotGroups(const ParsedMinerule &pmr);

  size_t evaluateTotGroups() const
      {
    return evaluateTotGroups(mr);
  }
};

} // namespace

#endif

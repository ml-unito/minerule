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
#ifndef __IDINCREMENTAL_ALGO_H__
#define __IDINCREMENTAL_ALGO_H__

#include<vector>
#include<set>

#include "IncrementalAlgorithm.hpp"
#include "minerule/Database/ItemType.hpp"
#include "minerule/Result/QueryResult.hpp"

namespace minerule {

/**
 * This class implements a very simple incremental algorithm
 * which can be used every time the predicates in the new query
 * are ALL item dependent ones. In such a situation, we can easily
 * retrieve the new result, by filtering out those itemset that 
 * does not satisfy the new constraints from the past result.
 * In particular, we can retrieve the list of all items that
 * do satisfy the constraint and use this list to prune the
 * old result set.
 */

class IDIncrementalAlgorithm : public IncrementalAlgorithm {
  typedef std::pair< std::set<ItemType>*, std::set<ItemType>* > ValidRule;
  typedef std::vector< ValidRule > ValidRules;
 protected:
  const ParsedMinerule::AttrVector* attrList;

  std::set<ItemType>* fillValidItems(const std::string& constraints) const throw(mrdb::SQLException);

  void getItemInfos( std::string& itemDescr, SourceRowColumnIds& hbsr ) const;
  bool checkInclusion(const std::set<ItemType>& validOnes, const ItemSet& foundOnes) const;
  bool checkInValidRules(const ValidRules& validRules, Rule& r) const;
  void filterQueries(const ValidRules& validRules) throw(MineruleException,mrdb::SQLException);

 public:

  IDIncrementalAlgorithm(const OptimizedMinerule& mr) : IncrementalAlgorithm(mr), attrList(NULL) { }
  virtual ~IDIncrementalAlgorithm() {}

  virtual void execute() throw(MineruleException,mrdb::SQLException);
};

} // namespace

#endif

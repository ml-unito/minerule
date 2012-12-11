#ifndef __IDINCREMENTAL_ALGO_H__
#define __IDINCREMENTAL_ALGO_H__

#include<vector>
#include<set>

#include "IncrementalAlgorithm.h"
#include "Database/ItemType.h"
#include "Result/QueryResult.h"

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

  std::set<ItemType>* fillValidItems(const std::string& constraints) const throw(odbc::SQLException);

  void getItemInfos( std::string& itemDescr, SourceRowColumnIds& hbsr ) const;
  bool checkInclusion(const std::set<ItemType>& validOnes, const ItemSet& foundOnes) const;
  bool checkInValidRules(const ValidRules& validRules, Rule& r) const;
  void filterQueries(const ValidRules& validRules) throw(MineruleException,odbc::SQLException);

 public:

  IDIncrementalAlgorithm(const OptimizedMinerule& mr) : IncrementalAlgorithm(mr), attrList(NULL) { }
  virtual ~IDIncrementalAlgorithm() {}

  virtual void execute() throw(MineruleException,odbc::SQLException);
};

} // namespace

#endif

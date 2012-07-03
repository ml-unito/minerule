#ifndef __RULEFORMATTER_H__
#define __RULEFORMATTER_H__

#include <vector>
#include <iostream>
#include <set>
#include "Optimizer/QueryResult.h"


namespace minerule {
/**
 * This is the base class for all RuleFormatter objects
 */
class RuleFormatter {
protected:
  std::ostream& os;
  std::string fieldSeparationString;

  static std::string quote(const std::string& elem);
  static std::string quoteElems(const ItemSetType& elems);
 public:
  RuleFormatter(std::ostream& out) : os(out), fieldSeparationString("") {
  }

  virtual ~RuleFormatter() {}

  virtual void
    printRule(const QueryResult::Rule&) = 0;

  void setFieldSeparationString(const std::string& s) {
    fieldSeparationString=s;
  }

  virtual void
  postExec()=0;
};


class SimpleRuleFormatter : public RuleFormatter {
public:
  SimpleRuleFormatter(std::ostream& os) : RuleFormatter(os) {};

  virtual ~SimpleRuleFormatter() {};

  virtual void
  printRule(const QueryResult::Rule&);

  virtual void
  postExec() {};
};


template <class Sorter>
class SortedRuleFormatter : public SimpleRuleFormatter {
  typedef std::set<QueryResult::Rule,Sorter> SortedContainer;
  SortedContainer sortedRules;
public:
  SortedRuleFormatter(std::ostream& os) : SimpleRuleFormatter(os) {};
  virtual ~SortedRuleFormatter() {};

  virtual void
  printRule(const QueryResult::Rule&);

  virtual void
  postExec();
};

template <class Sorter> void
SortedRuleFormatter<Sorter>::printRule(const QueryResult::Rule& r) {
  sortedRules.insert(r);
}

template <class Sorter> void
SortedRuleFormatter<Sorter>::postExec() {
  typename SortedContainer::const_iterator it;
  for(it=sortedRules.begin(); it!=sortedRules.end(); it++) {
    SimpleRuleFormatter::printRule(*it);
  }
}

} // namespace

#endif

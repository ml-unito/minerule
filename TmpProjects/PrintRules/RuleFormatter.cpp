#include "RuleFormatter.h"

using namespace std;

namespace minerule {

string 
RuleFormatter::quote(const string& elem) {
  return "["+elem+"]";
}

string 
RuleFormatter::quoteElems(const ItemSetType& elems) {
  string result;

  ItemSetType::const_iterator it=elems.begin();
  
  if(it!=elems.end()) {
    result = quote(it->asString());
    it++;
  }

  for(; it!=elems.end(); it++) {
    result +=  ", " + quote(it->asString());
  }

  return result;
}


void
SimpleRuleFormatter::printRule(const QueryResult::Rule& rule) {
  string bhSep = " => ";   // separates body from head 
  string rsSep = "\t";   // separates rule from support
  string scSep = "\t\t"; // separates support from confidence

  if(fieldSeparationString!="")
    bhSep=rsSep=scSep=fieldSeparationString;

  os << quoteElems(*rule.body) << bhSep << quoteElems(*rule.head) 
     << rsSep << rule.support << scSep << rule.confidence << endl;
}

} // namespace

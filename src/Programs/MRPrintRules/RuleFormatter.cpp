#include "RuleFormatter.h"



namespace minerule {
	std::string 
	RuleFormatter::quote(const std::string& elem) {
		return "["+elem+"]";
	}
	std::string 
	RuleFormatter::quoteElems(const ItemSet& elems) {
		std::string result;

		ItemSet::const_iterator it=elems.begin();
  
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
	SimpleRuleFormatter::printRule(const Rule& rule) {
		std::string bhSep = " => ";   // separates body from head 
		std::string rsSep = "\t";   // separates rule from support
		std::string scSep = "\t\t"; // separates support from confidence

		if(_fieldSeparationString!="")
			bhSep=rsSep=scSep=_fieldSeparationString;

		if( suppressLog() ) {
			cout << "\t" 
				<< quoteElems(*rule.body) << bhSep 
				<< quoteElems(*rule.head) << rsSep 
				<< rule.support << scSep << rule.confidence << std::endl;
		} else {
			MRLog() << "\t" 
				<< quoteElems(*rule.body) << bhSep 
				<< quoteElems(*rule.head) << rsSep 
				<< rule.support << scSep << rule.confidence << std::endl;				
		}
	}

} // namespace

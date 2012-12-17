#include "minerule/Result/RuleFormatter.h"
#include <iomanip>
#include <sstream>


namespace minerule {
	std::string RuleFormatter::quote(const std::string& elem) {
		return "["+elem+"]";
	}
	
	std::string RuleFormatter::quoteElems(const ItemSet& elems) {
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


	std::string
	SimpleRuleFormatter::formatRule(const Rule& rule) {		
		std::stringstream out;

		out	<< std::setw(_fieldWidths.body) << quoteElems(rule.getBody()) 
			<< _bhSep 
			<< std::left << std::setw(_fieldWidths.head) << quoteElems(rule.getHead()) << std::right
			<< " "
			<< std::setw(_fieldWidths.supp) << rule.getSupport()
			<< " "
			<< std::setw(_fieldWidths.conf) << rule.getConfidence();
	
		return out.str();
	}
	
	
	void
	SimpleRuleFormatter::printRule(const Rule& rule) {
		if( suppressLog() ) {
			std::cout << formatRule(rule)  << std::endl ;
		} else {
			MRLog() << formatRule(rule)  << std::endl;
		}
	}
	

} // namespace

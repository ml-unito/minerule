#include "Result/RuleFormatter.h"
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
		std::string bhSep = " => ";   // separates body from head 
		int bodyWidth = 30;  // width of the body column
		int headWidth = 30;  // width of the head column
		int suppWidth = 9;
		int confWidth = 9;
		
		std::stringstream out;

		out	<< std::setw(bodyWidth) << quoteElems(rule.getBody()) 
			<< bhSep 
			<< std::left << std::setw(headWidth) << quoteElems(rule.getHead()) << std::right
			<< " "
			<< std::setw(suppWidth) << rule.getSupport()
			<< " "
			<< std::setw(confWidth) << rule.getConfidence() << std::endl;
	
		return out.str();
	}
	
	
	void
	SimpleRuleFormatter::printRule(const Rule& rule) {
		if( suppressLog() ) {
			std::cout << formatRule(rule);
		} else {
			MRLog() << formatRule(rule);
		}
	}
	

} // namespace

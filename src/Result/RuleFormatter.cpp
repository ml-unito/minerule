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
#include "minerule/Result/RuleFormatter.hpp"
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
	SimpleRuleFormatter::formatRule(const Rule& rule, bool includeSuppConf) {		
		std::stringstream out;

		out	<< std::setw(_fieldWidths.body) << quoteElems(rule.getBody()) 
			<< _bhSep 
			<< std::left << std::setw(_fieldWidths.head) << quoteElems(rule.getHead());
		
		if(includeSuppConf) {
			out << std::right
				<< " "
				<< std::setw(_fieldWidths.supp) << rule.getSupport()
				<< " "
				<< std::setw(_fieldWidths.conf) << rule.getConfidence();
		}
		
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

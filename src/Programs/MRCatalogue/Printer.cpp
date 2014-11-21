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
#include "Printer.hpp"
#include "minerule/Utils/StringUtils.hpp"
#include <sstream>

namespace mrc {
	const size_t  Printer::OUTPUT_MAX_LEN = 70;
	
	
	void Printer::printIndex() const {
		std::stringstream ss;
		ss <<  _result_index;
		_out << "-" << minerule::StringUtils::toBold( ss.str() ) << "- ";
	}
		
	void Printer::format(std::string header, std::string info) {
		printIndex();
		std::vector<std::string>* chunks = minerule::StringUtils::splitToLength(info, OUTPUT_MAX_LEN);
		assert(chunks->size()!=0);
		
		std::vector<std::string>::const_iterator it = chunks->begin();
		_out << minerule::StringUtils::toGreen(header) << "\t" << *it << std::endl;					
		++it;
		
		for(; it!=chunks->end(); ++it) {
			_out << "\t" << *it << std::endl;
		}
		
		delete chunks;
	}
	
	void Printer::format(std::string header, size_t info) {
		printIndex();
		_out << minerule::StringUtils::toGreen(header) << "\t"  << info << std::endl;
	}

	
	void Printer::print(const minerule::CatalogueInfo& info) {
		format("name:", info.qryName);
	  
		if(_options.getListFormat().size) {
			format("size:", info.resSize);
		}
  
		if(_options.getListFormat().result) {			
			format("tables:\n", minerule::StringUtils::join(info.resTables, ", "));
		}
  
		if(_options.getListFormat().text) {
			format("text:\n", info.qryText);
		}  
	}
	
	void Printer::print(const std::vector<minerule::CatalogueInfo>& list)  {
		_out << "Found " << list.size() << " result sets:" << std::endl;
		_result_index = 1;
		std::vector<minerule::CatalogueInfo>::const_iterator it;
		for(it=list.begin(); it!=list.end(); it++) {
			print(*it);
			_result_index++;
		}
	}
}
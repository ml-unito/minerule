#include "Printer.h"
#include "Utils/StringUtils.h"
#include <sstream>

namespace mrc {
	const size_t  Printer::OUTPUT_MAX_LEN = 70;
	
	
	void Printer::printIndex() const {
		std::stringstream ss;
		ss <<  _result_index;
		_out << "-" << minerule::StringUtils::to_bold( ss.str() ) << "- ";
	}
		
	void Printer::format(std::string header, std::string info) {
		printIndex();
		std::vector<std::string>* chunks = minerule::StringUtils::split_to_length(info, OUTPUT_MAX_LEN);
		assert(chunks->size()!=0);
		
		std::vector<std::string>::const_iterator it = chunks->begin();
		_out << minerule::StringUtils::to_green(header) << "\t" << *it << std::endl;					
		++it;
		
		for(; it!=chunks->end(); ++it) {
			_out << "\t" << *it << std::endl;
		}
		
		delete chunks;
	}
	
	void Printer::format(std::string header, size_t info) {
		printIndex();
		_out << minerule::StringUtils::to_green(header) << "\t"  << info << std::endl;
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
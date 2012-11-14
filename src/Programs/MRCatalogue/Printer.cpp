#include "Printer.h"


namespace mrc {
	const size_t  Printer::OUTPUT_MAX_LEN = 70;
	
	
	void Printer::printIndex() const {
		_out << "-" << _result_index << "- ";
	}
	
	
	void Printer::split(const std::string& str, std::vector<std::string>& result) {
		size_t len = str.size();
		size_t cur_pos = 0;
		while( cur_pos < len ) {
			std::string chunk;
			size_t new_pos = std::min( cur_pos + OUTPUT_MAX_LEN, len );
			result.push_back(str.substr(cur_pos, new_pos-cur_pos));
			cur_pos = new_pos;
		}
	}
	
	void Printer::format(std::string header, std::string info) {
		printIndex();
		std::vector<std::string> chunks;
		split(info, chunks);
		
		std::vector<std::string>::const_iterator it = chunks.begin();
		_out << header << "\t" << *it << std::endl;					
		++it;
		
		for(; it!=chunks.end(); ++it) {
			_out << "\t" << *it << std::endl;
		}
	}
	
	void Printer::format(std::string header, size_t info) {
		printIndex();
		_out << header << "\t"  << info << std::endl;
	}

	
	void Printer::print(const minerule::CatalogueInfo& info) {
		format("name:", info.qryName);
	  
		if(_options.getListFormat().size) {
			format("size:", info.resSize);
		}
  
		if(_options.getListFormat().result) {
			format("tables:", info.resName);
		}
  
		if(_options.getListFormat().text) {
			format("text:\n", info.qryText);
		}  
	}
	
	void Printer::print(const std::vector<minerule::CatalogueInfo>& list)  {
		_out << "Found " << list.size() << "result sets:" << std::endl;
		_result_index = 1;
		std::vector<minerule::CatalogueInfo>::const_iterator it;
		for(it=list.begin(); it!=list.end(); it++) {
			print(*it);
			_result_index++;
			_out << std::endl;
		}
		
		_out << std::endl;
	}
}
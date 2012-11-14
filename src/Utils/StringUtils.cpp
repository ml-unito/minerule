#include "Utils/StringUtils.h"

namespace minerule {
	const std::string StringUtils::BOLD 	= "\x1B[1m";
	const std::string StringUtils::RED 		= "\x1B[31m";
	const std::string StringUtils::GREEN 	= "\x1B[32m";
	const std::string StringUtils::YELLOW 	= "\x1B[33m";
	const std::string StringUtils::BLUE 	= "\x1B[34m";	
	const std::string StringUtils::WHITE 	= "\x1B[37m";	
	const std::string StringUtils::CLOSE 	= "\x1B[0m";	

	std::vector<std::string>* StringUtils::split_to_length(const std::string& str, size_t out_len) {
		std::vector<std::string>* result = new std::vector<std::string>;
		size_t len = str.size();
		size_t cur_pos = 0;
		while( cur_pos < len ) {
			std::string chunk;
			size_t new_pos = std::min( cur_pos + out_len, len );
			result->push_back(str.substr(cur_pos, new_pos-cur_pos));
			cur_pos = new_pos;
		}
	
		return result;
	}

	std::string StringUtils::join(const std::vector<std::string>& vec, const std::string& sep) {
		std::string result;
		for(std::vector<std::string>::const_iterator it=vec.begin(); it!=vec.end(); ++it) {
			if(it!=vec.begin()) { result += sep; }
			result += *it;
		}
		
		return result;
	}

}

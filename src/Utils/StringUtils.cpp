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
#include "minerule/Utils/StringUtils.h"

namespace minerule {
	bool StringUtils::enableColors = true;

	const std::string StringUtils::BOLD 	= "\x1B[1m";
	const std::string StringUtils::RED 		= "\x1B[31m";
	const std::string StringUtils::GREEN 	= "\x1B[32m";
	const std::string StringUtils::YELLOW 	= "\x1B[33m";
	const std::string StringUtils::BLUE 	= "\x1B[34m";
	const std::string StringUtils::WHITE 	= "\x1B[37m";
	const std::string StringUtils::CLOSE 	= "\x1B[0m";

	std::vector<std::string>* StringUtils::splitToLength(const std::string& str, size_t out_len) {
		std::vector<std::string>* result = new std::vector<std::string>;
		size_t len = str.size();
		size_t cur_pos = 0;
		while( cur_pos < len ) {
			std::string chunk;
			size_t new_pos = std::min( cur_pos + out_len, len );


			size_t last_space = str.find_last_of(" ", new_pos);
			if( last_space != std::string::npos &&  // found something AND
					last_space > cur_pos && 						// still moving forward AND
					last_space + 10 >= new_pos && 			// not too far back w.r.t. from new_pos
					!(cur_pos == len))										// not at the end of the string
				new_pos = last_space+1;

			result->push_back(str.substr(cur_pos, new_pos-cur_pos));
			cur_pos = new_pos;
		}

		return result;
	}

	std::vector<std::string> StringUtils::split(const std::string& str, const std::string& sep) {
		std::vector<std::string> result;
		size_t pos = 0;
		size_t size = str.size();
		while( pos < size ) {
			size_t found_pos = str.find(sep,pos);
			std::string cur_piece = str.substr(pos,found_pos);
			pos+=cur_piece.size();

			result.push_back(cur_piece);

			if( pos < size ) { // if something where found, we move beyond the sep string.
				pos+=sep.size();
			}
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

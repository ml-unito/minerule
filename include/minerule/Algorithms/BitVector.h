//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Marco Botta (botta@di.unito.it)
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
#ifndef BIT_VECTOR__H
#define BIT_VECTOR__H
#include <vector>
#include <cstring>

namespace minerule {
	class BitVector: public std::vector<bool>{

	public:


		BitVector operator& (BitVector input){
			BitVector ris;
			for (size_t i=0;i<this->size();++i)
				ris.push_back((*this)[i]&input[i]);
			return ris;
		}

		BitVector operator| (BitVector input){
			BitVector ris;
			for (size_t i=0;i<this->size();++i)
				ris.push_back((*this)[i]|input[i]);
			return ris;
		}


	};

}


#endif


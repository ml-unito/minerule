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
#include "minerule/Database/SourceRowColumnIds.hpp"

namespace minerule {
	
	unsigned int 
	SourceRowColumnIds::setElems(std::vector<int>& elems, unsigned int start, unsigned int numCols) const {
		elems.clear();
		for( unsigned int i=0; i<numCols; i++ ) {
			elems.push_back(start+i);
		}

		return start+numCols-1;
	}


	unsigned int 
	SourceRowColumnIds::setgroupElems(unsigned int start,unsigned int numCols) {
		return setElems(groupElems, start, numCols);
	}

	unsigned int 
	SourceRowColumnIds::setClusterBodyElems(unsigned int start, unsigned int numCols) {
		return setElems(clusterBodyElems, start, numCols);
	}

	unsigned int 
	SourceRowColumnIds::setBodyElems(unsigned int start,unsigned int numCols) {
		return setElems(bodyElems, start, numCols);
	}

	unsigned int 
	SourceRowColumnIds::setClusterHeadElems(unsigned int start, unsigned int numCols) {
		return  setElems(clusterHeadElems, start, numCols);
	}

	unsigned int 
	SourceRowColumnIds::setHeadElems(unsigned int start, unsigned int numCols) {
		return setElems(headElems, start, numCols);
	}
}
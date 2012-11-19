#include "Database/SourceRowColumnIds.h"

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
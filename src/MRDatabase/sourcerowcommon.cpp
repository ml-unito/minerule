#include "MRDatabase/sourcerowcommon.h"

using namespace minerule;

unsigned int 
HeadBodySourceRowDescription::setElems(std::vector<int>& elems, unsigned int start, unsigned int numCols) const {
  for( unsigned int i=0; i<numCols; i++ ) {
    elems.push_back(start+i);
  }

  return start+numCols-1;
}


unsigned int 
HeadBodySourceRowDescription::setGroupBodyElems(unsigned int start,unsigned int numCols) {
  return setElems(groupBodyElems, start, numCols);
}

unsigned int 
HeadBodySourceRowDescription::setClusterBodyElems(unsigned int start, unsigned int numCols) {
  return setElems(clusterBodyElems, start, numCols);
}

unsigned int 
HeadBodySourceRowDescription::setBodyElems(unsigned int start,unsigned int numCols) {
  return setElems(bodyElems, start, numCols);
}

unsigned int 
HeadBodySourceRowDescription::setClusterHeadElems(unsigned int start, unsigned int numCols) {
  return  setElems(clusterHeadElems, start, numCols);
}

unsigned int 
HeadBodySourceRowDescription::setHeadElems(unsigned int start, unsigned int numCols) {
  return setElems(headElems, start, numCols);
}

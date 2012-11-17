#ifndef __SOURCEROWCOMMON_H__
#define __SOURCEROWCOMMON_H__

#include <cstdio>
#include <vector>
#include <string>
#include <iterator>

namespace minerule {

class SourceRowColumnIds {
  public:
  std::vector<int> groupBodyElems;
  std::vector<int> clusterBodyElems;
  std::vector<int> bodyElems;
  std::vector<int> clusterHeadElems;
  std::vector<int> headElems;
    
  SourceRowColumnIds() {
  }
  
  ~SourceRowColumnIds() {
  }
  
  
  // The following function is a generalization of the set???Elem function defined below (see the 
  // corresponding comment for explanation about start, numCols and return values). Its intended usage
  // is: 
  //   SourceRowColumnIds des;
  //   des.setElems(des.bodyElems, 1, 5)
  // which is equivalent to
  //   des.setBodyElems(1,5).
  // It is provided as a convenience method...

  unsigned int setElems(std::vector<int>& elems, unsigned int start, unsigned int numCols) const;

  // The following functions are meant to help in setting
  // the elems vectors.
  // All functions: takes the start column "start" and the number "numCols" of columns that the elem represent
  //   and returns start+numCols-1. As a side effect, the columns start, start+1,...,start+numCols-1 are added
  // to the target elem
  // their intended usage is something like:
  //    /* the following lines sets the description as follows:  
  //       groupBody : from column 1 to 3
  //       clusterBody: from column 4 to 5
  //       body : from column 6 to 10
  //       clusterHead: from column 11 to 12
  //       head : from column 13 to 18 */
  //    SourceRowColumnIds des;
  //    int lastCol;
  //    lastCol = des.setGroupBodyElems(1,3);
  //    lastCol = des.setClusterBodyElems(lastCol+1,2);
  //    lastCol = des.setBodyElems(lastCol+1,5);
  //    lastCol = des.setClusterHead(lastCol+1,2);
  //    lastCol = des.setHead(lastCol+1,6);


  unsigned int setGroupBodyElems(unsigned int start,unsigned int numCols);
  unsigned int setClusterBodyElems(unsigned int start, unsigned int numCols);
  unsigned int setBodyElems(unsigned int start,unsigned int numCols);
  unsigned int setClusterHeadElems(unsigned int start, unsigned int numCols);
  unsigned int setHeadElems(unsigned int start, unsigned int numCols);

};

inline std::ostream& operator<<(std::ostream& o, const SourceRowColumnIds& rowDes) {
	o << "head:"; copy( rowDes.headElems.begin(), rowDes.headElems.end(), std::ostream_iterator<int>(o, " ") ); 
	o << " - ";
	o << "body:"; copy( rowDes.bodyElems.begin(), rowDes.bodyElems.end(), std::ostream_iterator<int>(o, " ") ); 
	return o;
}

} // namespace

#endif

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
#ifndef __SOURCEROWCOMMON_H__
#define __SOURCEROWCOMMON_H__

#include <cstdio>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>

namespace minerule {

/**
 * A SourceRowColumnIds object maintains the information about which columns
 * of the source table corresponds to which elements of the mining rule, e.g.,
 * which columns are to be used as head elements and which needs to be used as
 * group ones.
 */

class SourceRowColumnIds {
  public:
  std::vector<int> groupElems;
  std::vector<int> clusterBodyElems;
  std::vector<int> bodyElems;
  std::vector<int> clusterHeadElems;
  std::vector<int> headElems;

  SourceRowColumnIds(const SourceRowColumnIds& rhs)
	  : groupElems(rhs.groupElems), clusterBodyElems(rhs.clusterBodyElems),
		bodyElems(rhs.bodyElems), clusterHeadElems(rhs.clusterHeadElems),
		headElems(rhs.headElems) {}

  SourceRowColumnIds() {
  }

  ~SourceRowColumnIds() {
  }


  /// @param start the column at which the group elements start
  /// @param numCols the number of contiguous columns to be set
  /// @return start+numCols+1
  ///
  /// Adds the columns start, start+1,...,start+numCols-1
  /// to the group elements.

  unsigned int setGroupElems(unsigned int start,unsigned int numCols);


  /// @param start the column at which the cluster body elements start
  /// @param numCols the number of contiguous columns to be set
  /// @return start+numCols+1
  ///
  /// Adds the columns start, start+1,...,start+numCols-1
  /// to the cluster body elements.

  unsigned int setClusterBodyElems(unsigned int start, unsigned int numCols);

  /// @param start the column at which the body elements start
  /// @param numCols the number of contiguous columns to be set
  /// @return start+numCols+1
  ///
  /// Adds the columns start, start+1,...,start+numCols-1
  /// to the body elements.

  unsigned int setBodyElems(unsigned int start,unsigned int numCols);

  /// @param start the column at which the clustr head elements start
  /// @param numCols the number of contiguous columns to be set
  /// @return start+numCols+1
  ///
  /// Adds the columns start, start+1,...,start+numCols-1
  /// to the clustr head elements.

  unsigned int setClusterHeadElems(unsigned int start, unsigned int numCols);

  /// @param start the column at which the head elements start
  /// @param numCols the number of contiguous columns to be set
  /// @return start+numCols+1
  ///
  /// Adds the columns start, start+1,...,start+numCols-1
  /// to the head elements.

  unsigned int setHeadElems(unsigned int start, unsigned int numCols);


private:
  /// A generalization of the set???Elem methods defined in this class (see the
  /// corresponding comment for explanation about start, numCols and return values).
  /// It is used internally to implement the mentione setter methods.
  /// Its intended usage
  /// is:
  ///   SourceRowColumnIds des;
  ///   des.setElems(des.bodyElems, 1, 5)
  /// which is equivalent to
  ///   des.setBodyElems(1,5).
  /// It is provided as a convenience method...

  unsigned int setElems(std::vector<int>& elems, unsigned int start, unsigned int numCols) const;

};

inline std::ostream& operator<<(std::ostream& o, const SourceRowColumnIds& rowDes) {
  // o << "test";
	// o << "head:";
  copy( rowDes.headElems.begin(), rowDes.headElems.end(), std::ostream_iterator<int>(o, " ") );
	// o << " - ";
	// o << "body:";
  copy( rowDes.bodyElems.begin(), rowDes.bodyElems.end(), std::ostream_iterator<int>(o, " ") );
	return o;
}

} // namespace

#endif

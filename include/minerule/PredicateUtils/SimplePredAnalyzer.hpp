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
#ifndef __SIMPLEPREDANALYZER_H__
#define __SIMPLEPREDANALYZER_H__

#include <string.h>
#include "minerule/mrdb/Types.hpp"
#include "minerule/Utils/MineruleException.hpp"
#include "minerule/Utils/SQLUtils.hpp"
#include "minerule/Parsers/ParsedMinerule.hpp"


namespace minerule {

  class SimplePredAnalyzer {
  protected:
    // operators are ordered as follows: "< <= = >= > <>"
    static size_t getOperatorIndex(const char* op) ;
    // relations are ordered as follows "v1<v2 v1=v2 v1>v2"
    static size_t getValuesRelationshipIndex(
		      const char* value1, 
		      const char* value2, 
		      SQLUtils::Type type) ;

     /* 
     * Given two predicates X=(x opx a) and Y=(y opy b) 
     * the opRelationTable returns a char which identifies
     * the relation between X and Y. In order to access the
     * table the two operators and the relation between a and b
     * must be encoded (the encodingis given by 'getOperatorIndex' 
     * and 'getValuesRelationshipCode' methods), the result is
     * a character which identifiy the type of the relation. For
     * instance let us fix X=(A<5) and Y=(A<3), and hypothetize
     * that the encoding for '<' is 0, and the encoding for (A,3,5)
     * is 2, then opRelationTable[0][0][2] returns 'l' which has
     * to be interpreted as a left arrow ("<-") which means that
     * the second predicate implies the first. More details about
     * returned values are described in the implementation file.
     */
    static const char* opRelationTable[6][6];
  public:
    static char 
      getRelation(const char* Xop, const char* Xvalue, 
		  const char* Yop, const char* Yvalue, 
		  SQLUtils::Type)  ;

    static bool 
      isAttrOpValuePredicate(simple_pred* X,
			     char*& attr,
			     char*& value,
			     bool& reverseOp);
  };
}

#endif

#ifndef __SIMPLEPREDANALYZER_H__
#define __SIMPLEPREDANALYZER_H__

#include <string.h>
#include <odbc++/types.h>
#include "minerule/Utils/MineruleException.h"
#include "minerule/Utils/SQLUtils.h"
#include "minerule/Parsers/ParsedMinerule.h"


namespace minerule {

  class SimplePredAnalyzer {
  protected:
    // operators are ordered as follows: "< <= = >= > <>"
    static size_t getOperatorIndex(const char* op) throw( MineruleException );
    // relations are ordered as follows "v1<v2 v1=v2 v1>v2"
    static size_t getValuesRelationshipIndex(
		      const char* value1, 
		      const char* value2, 
		      SQLUtils::Type type) throw( MineruleException);

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
		  SQLUtils::Type)  throw(MineruleException);

    static bool 
      isAttrOpValuePredicate(simple_pred* X,
			     char*& attr,
			     char*& value,
			     bool& reverseOp);
  };
}

#endif

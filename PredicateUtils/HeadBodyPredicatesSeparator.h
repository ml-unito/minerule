#ifndef __HEADBODYPREDICATESEPARATOR_H__
#define __HEADBODYPREDICATESEPARATOR_H__

#include <string>
#include "Parsers/ParsedMinerule.h"

namespace minerule { 

  // ----------------------------------------------------------------------
  // HeadBodyPredicatesSeparator
  // ----------------------------------------------------------------------

  /**
   * This class exports the separate static function which takes a
   * list_AND_node and returns the BODY part and the HEAD part of the
   * predicates as std::strings. It throws a MineruleException if
   * any cross predicate is found.
   * Notice: at the present time the HEAD and BODY part of each attribute
   * name is removed before inserted in the result.
   */
  
  class HeadBodyPredicatesSeparator {
  private:
    typedef enum { NO_INFO, BODY_CONSTR, HEAD_CONSTR, ERROR} ConstraintClass;
    static void
      setState( ConstraintClass& current, ConstraintClass newone )  
      throw(MineruleException);

    static ConstraintClass
      setStateAccordinglyToString(std::string& str, ConstraintClass state)  
      throw(MineruleException);

    static void
      updateConstraint( std::string& str, 
			const std::string& v1, 
			const std::string& op, 
			const std::string& v2);
  public:
    static void
      separate(list_AND_node* l_and, 
	       std::string& bodyConstraints,
	       std::string& headConstraints) throw(MineruleException);
  };

}

#endif

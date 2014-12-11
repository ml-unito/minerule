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
#ifndef __HEADBODYPREDICATESEPARATOR_H__
#define __HEADBODYPREDICATESEPARATOR_H__

#include <string>
#include "minerule/Parsers/ParsedMinerule.hpp"

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
      ;

    static ConstraintClass
      setStateAccordinglyToString(std::string& str, ConstraintClass state)  
      ;

    static void
      updateConstraint( std::string& str, 
			const std::string& v1, 
			const std::string& op, 
			const std::string& v2);
  public:
    static void
      separate(list_AND_node* l_and, 
	       std::string& bodyConstraints,
	       std::string& headConstraints) ;
  };

}

#endif

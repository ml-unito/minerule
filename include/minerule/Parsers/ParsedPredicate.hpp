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
#ifndef __PREDICATEPARSER_H__
#define __PREDICATEPARSER_H__

#include <iostream>
#include <list>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <set>
#include <typeinfo>

#include "PredicateBase.hpp"
#include "ParsedMinerule.hpp"

namespace minerule {

  /**
   * This class represent a single comparison with two terms  
   *  the attribute aggr_f is marked as true if this predicate contains an aggregation functions for
   * example SUM, AVG, STD, ...
   */
  class ParsedSimplePredicate: public SimplePredicateBase {
    bool partOfBorH;
    bool aggr_f;
  public:
    /**                                                                                                                                                             
     * The constructor of the class without parameter
     *
     */
    ParsedSimplePredicate() : SimplePredicateBase(), partOfBorH(false), aggr_f(false) {}

    /** 
     *  This constructor take three parameter, two terms and an operator of comparison
     * @param val1 the first term
     * @param op   the comparison operator
     * @param val2 the second term
     */

    ParsedSimplePredicate(const std::string& val1, const std::string& op, const std::string& val2, bool BorH, bool aggr): 
      SimplePredicateBase(val1, op, val2), partOfBorH(BorH), aggr_f(aggr){}

    /**                                                                                                                                                              
     *  This is the copy constructor
     * @param rhs another ParsedSimplePredicate obj
     */																		
    
    ParsedSimplePredicate(const ParsedSimplePredicate& rhs) :
      SimplePredicateBase(rhs), partOfBorH(rhs.partOfBorH), aggr_f(rhs.aggr_f) {}

    /**
     * this method is very important because permit to clone the exact objcet in the hierachy of class
     * because the RTTI mechanism use the right method with a specific type of object
     * @return SimplePredicateBase the object of super-class that incapsule a new object of this class
     */   

    virtual SimplePredicateBase* cloneInstance() const {
      return new ParsedSimplePredicate(*this);
    }

    /**                                                                                                                                                             
     * this method is very important because permit to create a new instance of the exact objcet in the hierachy of class                                           
     * because the RTTI mechanism use the right method with a specific type of object                                                      
     * @return SimplePredicateBase the object of super-class that incapsule 
     */
    
    virtual SimplePredicateBase* newInstance() const {
        return new ParsedSimplePredicate();
    }
    /**
     *
     */
    PredicateBase* newPredicate() const;

    /**
     *
     */
    PredConjunctionBase* newPredConjunction() const;
    
    
    
    /**
     * this method control if the simplePredicate contains a BODY or HEAD attribute
     * @return true if this object contains a BODY or HEAD attribute, false in other situation
     */	
    bool isPartOfBorH() const {
      return partOfBorH;
    }
    
    /**
     * this method control if the simplePredicate contains a BODY or HEAD attribute
     * @return true if this object represent an aggregation function as SUM, STD, AVG, ecc..
     */
    bool isAggr_function() const {
      return aggr_f;
    }
  };

  //----------------------------------------------------------------------
  // ParsedPredConjunction
  //-----------------------------------------------------------------------

  /**
   * This class represent a conjunction of SimplePredicate  
   *
   */
  class ParsedPredConjunction : public PredConjunctionBase{
  	
    /**
     * Build a new ParsedPredConjunction that contain the old vector plus the new element and not modify the old vector
     */  	
  public:

    /**
      * The empty Constructor of the class 
      */
    ParsedPredConjunction(): PredConjunctionBase() {};

    /**
      * This is the copy constructor 
      *@param rhs another object of this class
      */
    ParsedPredConjunction(const ParsedPredConjunction& rhs) {
      ParsedPredConjunction::const_iterator it;
      for (it=rhs.begin();it!=rhs.end();it++) {
	this->push_back( new ParsedSimplePredicate(dynamic_cast<const ParsedSimplePredicate&>(**it)) );
      }
    }

    /**                                                                                                                                                            * this method is very important because permit to clone the exact objcet in the hierachy of class                                                               * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                          * subclass of this class you must overriding this method for your class                                                                                        * @return PredConjunctioBase the object of super-class that incapsule a new object of this class                                                               */
    
    virtual PredConjunctionBase* cloneInstance() const {
      return new ParsedPredConjunction( *this );
    }
    /**                                                                                                                                                        \     * this method is very important because permit to create a new instance of the exact objcet in the hierachy of class                                     \     * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                   \     * subclass of this class you must overriding this method for your class                                                                                        * @return PredConjunctionBase the object of super-class that incapsule                                                                                          */

    virtual PredConjunctionBase* newInstance() const {
      return new ParsedPredConjunction( );
    }
    
    /**
     *
     */
    PredicateBase* newPredicate()const ;

    /**
     *
     */
    PredConjunctionBase* newPredConjunction()const;

    /**
     * check if all the SimplePredicate are part of body or part of the head
     * @return true if only if all predicate are part of body or head
     */
    bool areAllBorH() const {
      ParsedPredConjunction::const_iterator it;
      for (it=this->begin();
	   it!=this->end() && dynamic_cast<const ParsedSimplePredicate&>(**it).isPartOfBorH(); 
	   it++); /* note the empty body of the for loop */

      return it==this->end();
    }
    
    /**
     * check if all the SimplePredicate are part of body or part of the head
     * @return true if only if all predicate are the aggregation function
     */
    bool areAllAggr_f() const {
      ParsedPredConjunction::const_iterator it;
      for (it=this->begin();
           it!=this->end() && dynamic_cast<const ParsedSimplePredicate&>(**it).isAggr_function();
           it++); /* note the empty body of the for loop */
      return it==this->end();
    }
	  
    /**
     * Check if at least One the SimplePredicate are part of body or part of the head
     * @return true at least One predicate is part of body or head
     */         

    bool atLeastOneBorH()const {
      ParsedPredConjunction::const_iterator it;
      for (it=this->begin();
	   it!=this->end() && !dynamic_cast<const ParsedSimplePredicate&>(**it).isPartOfBorH(); 
	   it++); /* note the empty body of the for loop */

      return it!=this->end();
    }


  };

  //----------------------------------------------------------------------
  // ParsedPredicate
  //-----------------------------------------------------------------------
 
  /**
   * This class represent a disjunction of PredConjunction, the prefer form of rappresentation is the disjunction form 
   */
  class ParsedPredicate : public PredicateBase {
  public:
    /**
     * The empty constructor
     */
    ParsedPredicate() : PredicateBase() {}
    /**
     * The copy constructor 
     *@param rhs another object of this class
     */

    ParsedPredicate(const ParsedPredicate& rhs) : PredicateBase(rhs) {}

    /**                                                                                                                                                            * this method is very important because permit to clone the exact objcet in the hierachy of class                                                               * because the RTTI mechanism use the right method with a specific type of object.                                                                              * @return PredicateBase the object of super-class that incapsule a new object of this class                                                                    */

    virtual ParsedPredicate* cloneInstance() const {
      return new ParsedPredicate(*this);
    }
    
    /**                                                                                                                                                              * this method is very important because permit to create a new instance of the exact objcet in the hierachy of class                                           * because the RTTI mechanism use the right method with a specific type of object.                                                                              * @return PredidcateBase the object of super-class that incapsule                                                                                              */

    virtual ParsedPredicate* newInstance() const {
      return new ParsedPredicate();
    }
    
    /**
     *
     */
    PredicateBase* newPredicate()const;

    /**
     *
     */
    PredConjunctionBase* newPredConjunction() const;
    
    /**
     * Check if all the SimplePredicate are part of body or part of the head
     * @return true if only if all predicate are part of body or head
     */     
    bool areAllBorH() const {
      ParsedPredicate::const_iterator it;

      for (it=this->begin();
	   it!=this->end() && dynamic_cast<const ParsedPredConjunction&>(**it).areAllBorH(); 
	   it++); /* note the empty body of the for loop */
    
      return it==this->end();    
    }
     	  
    
    /**
     * Check if all the SimplePredicate are part of body or part of the head
     * @return true if only if all predicate are aggregate function
     */
    bool areAllAggr_f() const {
      ParsedPredicate::const_iterator it;
      for (it=this->begin();
           it!=this->end() && dynamic_cast<const ParsedPredConjunction&>(**it).areAllAggr_f();
           it++); /* note the empty body of the for loop */
      return it==this->end();
    }


    /**
     * Check if at least One the SimplePredicate are part of body or part of the head
     * @return true at least One predicate is part of body or head
     */         
    bool atLeastOneBorH() const {
      ParsedPredicate::const_iterator it;

      for (it=this->begin();
	   it!=this->end() && !dynamic_cast<const ParsedPredConjunction&>(**it).atLeastOneBorH(); 
	   it++); /* note the empty body of the for loop */
    
      return it!=this->end();
    }


    /** 
     * Convert the ParsedPredicate to the "equivalent" c-structure.
     */

    list_OR_node* convert() const;
    list_AND_node* convert_and_list(const PredConjunctionBase&) const;
	
  };

} // namespace

#endif



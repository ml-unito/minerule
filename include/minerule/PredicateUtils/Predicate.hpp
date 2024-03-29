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
#ifndef __PREDICATE_H__
#define __PREDICATE_H__

#include <vector>
#include <set>
#include <string>

#include "minerule/Parsers/ParsedMinerule.hpp"
#include "minerule/PredicateUtils/VarSet.hpp"

namespace minerule {
  // forward declaration needed by SimplePredicate
  class PtrSimplePredComp;

  // ----------------------------------------------------------------------
  // SimplePredicate
  // ----------------------------------------------------------------------

  class SimplePredicate {
    typedef std::set<SimplePredicate*, PtrSimplePredComp> PredicatePool;

    static  PredicatePool predicatePool;


    SimplePredicate(const simple_pred* pred ) :
      val1(pred->val1),
      val2(pred->val2),
      op(pred->op),
      varId(0) {
    }

    SimplePredicate( const std::string& v1, 
		     const std::string& o, 
		     const std::string& v2 ) :
      val1(v1),
      val2(v2),
      op(o) {
    }

    std::string val1;
    std::string val2;
    std::string op;

    size_t varId;
  public:
   

    // We need to maintain a unique copy of identical predicates.
    // All new predicates will be inserted in the predicate pool,
    // when we try to construct a new predicate which is identical
    // to one olready known, we simply return the known version

    static SimplePredicate& 
      newSimplePredicate( const simple_pred* pred );

    static SimplePredicate& 
      newSimplePredicate(const std::string& val1,
			 const std::string& op,
			 const std::string& val2);

    // It can be called to reclaim the memory used by the pool
    // when it is no longer used.
    static void
      freeSimplePredicatePool();

    const std::string& getVal1() const {
      return val1;  
    }

    const std::string& getVal2() const {
      return val2;
    }

    const std::string& getOp() const {
      return op;
    }
    
    bool operator==( const SimplePredicate& rhs ) const {
      return 
	val1==rhs.val1 &&
	val2==rhs.val2 &&
	op==rhs.op;
    }

    bool operator<(const SimplePredicate& rhs) const {
      return val1+op+val2 < rhs.val1+rhs.op+rhs.val2;
    }

    void setVarId( size_t id ) {
      varId = id;
    }

    size_t getVarId() const {
      return varId;
    }
  };

  // ----------------------------------------------------------------------
  // PtrSimplePredComp
  // ----------------------------------------------------------------------

  /**
   * SimplePredicate ptrs comparison function. It is needed 
   * because of the use of std::set<SimplePredicates*> objects.
   * We need to order the objects in the set using the <
   * operator defined on SimplePredicates, not using the <
   * operator defined on pointers (which is the default
   * used by class set.
   */

  class PtrSimplePredComp {
  public:
    bool operator()(const SimplePredicate* lhs, 
		    const SimplePredicate* rhs) const {
      return *lhs < *rhs;
    }
  };

  // ----------------------------------------------------------------------
  // PredConjunction
  // ----------------------------------------------------------------------

  class PredConjunction : public std::vector<SimplePredicate*> {
  public:
    PredConjunction() {};
    PredConjunction( const list_AND_node*);
    PredConjunction( const PredConjunction& );
    ~PredConjunction() { }

    bool evaluate(const VarSet&) const;

    /**
     * Builds the logical and of *this and p and return *this.
     */
    PredConjunction& operator&=(const PredConjunction& p);
  };

  // ----------------------------------------------------------------------
  // Predicate. Notice: this is a vector of Predicate Conjunctions with
  //       another name and some handy functions. 
  // ----------------------------------------------------------------------

  class Predicate : public std::vector<PredConjunction*> {
  private:
    std::set<SimplePredicate*, PtrSimplePredComp>* predList;	
    size_t numVariables;

  public:
    Predicate() :
      predList(NULL), 
      numVariables(0)  {};

    Predicate(const Predicate&);

    Predicate( const list_OR_node* node );
    ~Predicate();

    Predicate& operator=(const Predicate& rhs) {
      //freeing used memory
      for( iterator it=begin(); it!=end(); it++ ) {
	delete *it;
      }

      // initializing variables
      predList=NULL;
      numVariables=0;
      // copying rhs contents
      for(Predicate::const_iterator it=rhs.begin(); it!=rhs.end(); it++) {
	push_back( new PredConjunction(**it) );
      }

      return *this;
    }


    /** 
     * Builds the logical and of *this and p and returns *this
     **/
    Predicate& operator&=( const Predicate& p  );

    /**
     * Builds the logical or of *this and p and returns *this
     */
    Predicate& operator|=(const Predicate& p );

    // Get the std::set of all simple predicates contained in
    // in this predicate. The user should call this function
    // passing true for parameter rebuildSet if the object
    // changed since the last time this function has been
    // called (otherwise, a cached, probably wrong, result
    // will be returned).

    std::set<SimplePredicate*,PtrSimplePredComp>&
      getPredicateList(bool rebuildSet=false) ;

    bool evaluate(const VarSet&) const;

    void  setNumVariables(size_t n) {
      numVariables=n;
    }
    size_t getNumVariables() const {
      return numVariables;
    }
  };

  inline std::ostream& operator<<(std::ostream& os, const SimplePredicate& s) {
    os << s.getVal1() << s.getOp() << s.getVal2();
    return os;
  }

  inline std::ostream& operator<<(std::ostream& os, const PredConjunction& p) {
    PredConjunction::const_iterator it2;
    for(it2=p.begin(); it2!=p.end(); it2++) {
      if(it2!=p.begin()) {
	os << " AND ";
      }
      
      os << **it2;
    }
    
    return os;
  }

  inline std::ostream& operator<<(std::ostream& os, const Predicate& p) {
    Predicate::const_iterator it;
    for( it=p.begin(); it!=p.end(); it++) {
      if(it!=p.begin()) {
	os << " OR ";
      }

      os << **it;
    }

    return os;
  }

} // namespace

#endif

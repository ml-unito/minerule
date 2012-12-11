#ifndef __PREDICATE_UTILS_H__
#define __PREDICATE_UTILS_H__

#include <vector>
#include <set>
#include <string>

#include "Parsers/ParsedMinerule.h"
#include "PredicateUtils/VarSet.h"
#include "PredicateUtils/ExpressionNFCoder.h"
#include "PredicateUtils/Predicate.h"
#include <iterator>

namespace minerule {
  // ----------------------------------------------------------------------
  // CountingIterator
  // ----------------------------------------------------------------------
  /**
   *  A simple output_iterator which does nothing but using the metod
   * setVarId in SimplePredicate to assign to each predicate a unique
   * identifier. The unique identifier is obtained by counting the
   * number of times that the asssignment operator is called.
   * Note the strange way the curNum is treated! This is as hack which
   * is needed since the set_union function takes the output_iterator
   * by value. Then in order to allow a proper counting we need to 
   * use a reference to an external variable (in this way all object
   * created by copying this object will share the same reference).
   */

  class CountingIterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
    size_t& curNum;
    list_AND_node*& l_and;
  public:
    static list_AND_node* new_list_AND_node( const char* v1,
					     const char* op,
					     const char* v2,
					     list_AND_node* oldList );
    
    static void delete_list_AND_node(list_AND_node*&);

    CountingIterator(size_t& counter, list_AND_node*& l) : curNum(counter), l_and(l) {
    }

    CountingIterator(const CountingIterator& c) : curNum(c.curNum), l_and(c.l_and) {}

    CountingIterator& operator=(SimplePredicate* const& pred) {
      pred->setVarId(curNum);
      list_AND_node* tmp = new_list_AND_node( pred->getVal1().c_str(), 
					      pred->getOp().c_str(), 
					      pred->getVal2().c_str(), l_and );
      l_and =tmp;

      return *this;
    }

    CountingIterator& operator*() {
      return *this;
    }

    CountingIterator& operator++(int) {
      curNum++;
      return *this;
    }

    CountingIterator& operator++() {
      curNum++;
      return *this;
    }
  };

 


  // ----------------------------------------------------------------------
  // PredicateUtils
  // ----------------------------------------------------------------------

  class PredicateUtils {
  public:
    typedef EncodedNF::CodesRelationship PredicateRelationship;

    static bool predicatesAreEquivalent(Predicate& p1,
					Predicate& p2,
					const std::string& tab_source);
    static PredicateRelationship 
      getPredicateRelationship( Predicate& p1,
				Predicate& p2,
				const std::string& tab_source);


    
  };
  
} // namespace

#endif

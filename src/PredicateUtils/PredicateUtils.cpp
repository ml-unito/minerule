#include "PredicateUtils/PredicateUtils.h"

#include <algorithm>
#include "PredicateUtils/ExpressionNFCoder.h"

namespace minerule {

  // ----------------------------------------------------------------------
  // PredicateUtils
  // ----------------------------------------------------------------------

  list_AND_node* CountingIterator::new_list_AND_node( const char* v1,
						      const char* op,
						      const char* v2,
						      list_AND_node* oldList ) {
    list_AND_node* tmp = new list_AND_node;
    tmp->sp = new simple_pred;
    tmp->sp->val1 = const_cast<char*>(v1);
    tmp->sp->op = const_cast<char*>(op);
    tmp->sp->val2 = const_cast<char*>(v2);
    tmp->next = oldList;
    return tmp;
  }
    
  void CountingIterator::delete_list_AND_node(list_AND_node*& l) {
    while( l!=NULL ) {
      list_AND_node* tmp = l->next;;

      delete l->sp;
      delete l;

      l=tmp;
    }
  }
 
 

  // The following returns the semantic relationship between 
  // two predicates (A and B). The possible answers are:
  // FirstMoreGeneral,FirstMoreSpecific, Equivalent, Unrelated
  // Their meaning is the following:
  // Equivalet:
  // two predicates are equivalent if they have the same
  // canonical form representation (a predicate is in 
  // canonical form if it is in disjunctive normal form and
  // if every disjunct contains every predicate (being it
  // negated or not)). Obviously the canonical form must
  // be evaluated on the union of the simple predicates appearing
  // in the two predicates.
  // FirstMoreGeneral:
  // A is more general than B if its canonical form is a superset
  // of B's one.
  // FirstMoreSpecific:
  // A is more specific than B iff B is more general than A.
  // Unrelated:
  // None of the definitions above holds for A,B.


  PredicateUtils::PredicateRelationship
  PredicateUtils::getPredicateRelationship(Predicate& p1,
					   Predicate& p2,
					   const string& tab_source) {
    // The following two lines assign unique variables id
    // to elements in p1,p2. It do so setting the variable id
    // for each element of the union (through ci) to the position
    // in which the predicate appear in the union itself. Note
    // that this work only because two equivalent predicates are
    // indeed two references to the same object (look at SimplePredicate
    // class to understand why this is true).
    size_t counter=0;
    list_AND_node* allPreds=NULL;

    CountingIterator ci(counter,allPreds);
    set<SimplePredicate*, PtrSimplePredComp>& sp1 = 
      p1.getPredicateList();
    set<SimplePredicate*, PtrSimplePredComp>& sp2 = 
      p2.getPredicateList();

    std::set_union( sp1.begin(), sp1.end(), 
		    sp2.begin(), sp2.end(),
		    ci );


    p1.setNumVariables(counter);
    p2.setNumVariables(counter);

    InvalidConfigurationFilter filter( tab_source, allPreds );

    ExpressionNFCoder e1(filter);
    ExpressionNFCoder e2(filter);
  
    PredicateUtils::PredicateRelationship rel=
      EncodedNF::getCodesRelationship(e1.encode(p1),e2.encode(p2));

    CountingIterator::delete_list_AND_node(allPreds);
    return rel;
  }


  // This is a more specific version of getPredicateRelationship.
  // It's presence is motivated because it shorten a little bit
  // the notation and because operator== is slightly more efficient
  // than getCodesRelationship (In fact the first returns as soon
  // as the first not identical element of the NFCode is found, the
  // latter has to wait until an incongruence is found).
  // Actually the difference between the two should not be noticeable,
  // but since this function has been done first, there is no point
  // in deleting it...
  bool
  PredicateUtils::predicatesAreEquivalent(Predicate& p1,
					  Predicate& p2,
					  const string& tab_source) {
    // The following two lines assign unique variables id
    // to elements in p1,p2. It do so setting the variable id
    // for each element of the union (through ci) to the position
    // in which the predicate appear in the union itself. Note
    // that this work only because two equivalent predicates are
    // indeed to references to the same object (look at SimplePredicate
    // class to understand why this is true).
    size_t counter=0;
    list_AND_node* allPreds=NULL;

    CountingIterator ci(counter, allPreds);
    set<SimplePredicate*, PtrSimplePredComp>& sp1 = 
      p1.getPredicateList();
    set<SimplePredicate*, PtrSimplePredComp>& sp2 = 
      p2.getPredicateList();

    std::set_union( sp1.begin(), sp1.end(), 
		    sp2.begin(), sp2.end(),
		    ci );


    p1.setNumVariables(counter);
    p2.setNumVariables(counter);

    InvalidConfigurationFilter filter( tab_source, allPreds );

    ExpressionNFCoder e1(filter);
    ExpressionNFCoder e2(filter);
  
    return e1.encode(p1)==e2.encode(p2);
  }





} // namespace
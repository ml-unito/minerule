#include "minerule/PredicateUtils/Predicate.h"

#include <algorithm>
#include "minerule/PredicateUtils/ExpressionNFCoder.h"


namespace minerule {

  // ----------------------------------------------------------------------
  // SimplePredicate
  // ----------------------------------------------------------------------

  SimplePredicate::PredicatePool SimplePredicate::predicatePool; // definition of static member


  SimplePredicate& 
  SimplePredicate::newSimplePredicate( const simple_pred* pred ) {
    SimplePredicate* newpred = new SimplePredicate(pred);
      
    PredicatePool::iterator it=predicatePool.find(newpred);
    if( it==predicatePool.end() ) {
      predicatePool.insert(newpred);
      return *newpred;
    } else {
      delete newpred;
      return **it;
    }
  }

  SimplePredicate& 
  SimplePredicate::newSimplePredicate(const std::string& val1,
				      const std::string& op,
				      const std::string& val2) {
    SimplePredicate* newpred = new SimplePredicate(val1,op,val2);
    PredicatePool::iterator it=predicatePool.find(newpred);
    if( it==predicatePool.end() ) {
      predicatePool.insert(newpred);
      return *newpred;
    } else {
      delete newpred;
      return **it;
    }
  }


  void 
  SimplePredicate::freeSimplePredicatePool() {
    PredicatePool::iterator it;
    for( it=predicatePool.begin(); it!=predicatePool.end(); it++ ) {
      delete *it;
    }

    predicatePool.clear();
  }

  // ----------------------------------------------------------------------
  // PredConjunction
  // ----------------------------------------------------------------------

  PredConjunction::PredConjunction(const list_AND_node* land) {
    while(land!=NULL) {
      push_back(&SimplePredicate::newSimplePredicate(land->sp));
      land = land->next;
    }
  }

  PredConjunction::PredConjunction(const PredConjunction& rhs) {
    *this = rhs;
  }  
  

  bool
  PredConjunction::evaluate( const VarSet& vset ) const {
    for(const_iterator it=begin(); it!=end(); it++) {
      if( vset.getVar((*it)->getVarId())==false )
	return false;
    }
    
    return true;
  }

  PredConjunction&
  PredConjunction::operator&=(const PredConjunction& p) {
    insert( end(), p.begin(), p.end() );
    return *this;
  }
  
  // ----------------------------------------------------------------------
  // Predicate
  // ----------------------------------------------------------------------

  Predicate::Predicate( const list_OR_node* lor ) :
    predList(NULL),
    numVariables(0) {
    while( lor!=NULL ) {
      push_back( new PredConjunction(lor->l_and) );
      lor=lor->next;
    }
  }

  Predicate::Predicate( const Predicate& rhs ) :
    predList(NULL),
    numVariables(0) {
    Predicate::const_iterator it;
    for(it=rhs.begin(); it!=rhs.end(); it++) {
      push_back( new PredConjunction(**it) );
    }
  }

      


  Predicate::~Predicate() {
    for( iterator it=begin(); it!=end(); it++ ) {
      delete *it;
    }
  }

  Predicate& Predicate::operator&=(const Predicate& p) {
    size_t oldSize = this->size();
    // making room for the new OR predicates
    Predicate thisCopy(*this);
    for(size_t i=0; i<p.size()-1; i++) {
      *this |= thisCopy;
    }

    //    std::cout << "This copy:" << thisCopy << std::endl;
    //    std::cout << "P:" << p << std::endl;

    // building the conjunctions (now there are oldSize*p.size() conj
    // in this predicate. The first oldSize ones need to be conjuncted
    // with p[0], the second oldSize ones with p[1]... the p.size()'th
    // oldSize ones with p[p.size()-1].
    for(size_t i=0; i<p.size(); i++) {
      for(size_t j=0; j<oldSize; j++) {
	assert(j+i*oldSize < this->size() );
	//	std::cout << "Pred("<<i<<","<<j<<"):" << *(*this)[j+i*oldSize] << std::endl;
	*(*this)[j+i*oldSize] &= *p[i];
      }
    }

    return *this;
  }

  Predicate& Predicate::operator|=(const Predicate& p) {
    for(Predicate::const_iterator it=p.begin(); it!=p.end(); it++) {
      push_back( new PredConjunction( **it ) );
    }

    return *this;
  }


  std::set<SimplePredicate*,PtrSimplePredComp>&
  Predicate::getPredicateList(bool rebuildSet)  {
    if(rebuildSet && predList!=NULL) {
      delete predList;
      predList=NULL;
    }
    
    if( predList!=NULL )
      return *predList;

    predList = new std::set<SimplePredicate*,PtrSimplePredComp>();

    iterator it;
    for( it=begin(); it!=end(); it++ ) {
      PredConjunction::iterator cit;
      for( cit=(*it)->begin(); cit!=(*it)->end(); cit++ ) {
	predList->insert(*cit);
      }
    }

    return *predList;
  }

 

  bool
  Predicate::evaluate( const VarSet& vset ) const {
    if(empty())
      return true;

    for( const_iterator it=begin(); it!=end(); it++ ) {
      if( (*it)->evaluate(vset)==true )
	return true;
    }

    return false;
  }
} // namespace

#include "minerule/Parsers/PredicateBase.h"

#include <algorithm>
#include <string>



namespace minerule {

  PredicateBase* SimplePredicateBase::newPredicate() const {
    return new PredicateBase();
  }

  PredConjunctionBase* SimplePredicateBase::newPredConjunction() const {
    return new PredConjunctionBase();
  }

  PredicateBase* PredConjunctionBase::newPredicate() const {
    return new PredicateBase();
  }

  PredConjunctionBase* PredConjunctionBase::newPredConjunction() const {
    return new PredConjunctionBase();
  }

  PredicateBase* PredicateBase::newPredicate() const {
    return new PredicateBase();
  }

 PredConjunctionBase* PredicateBase::newPredConjunction() const {
    return new PredConjunctionBase();
  }



  PredicateBase& 
  SimplePredicateBase::operator!() const {
   std::string nop;
    if (op==">") { nop="<="; }
    else if (op==">=") { nop="<"; }
    else if (op=="<") { nop=">="; }
    else if (op=="<=") { nop=">"; }
    else if (op=="=") { nop="<>"; }
    else if (op=="<>") { nop="="; }
    SimplePredicateBase* sp= this->cloneInstance();
    PredConjunctionBase* pc= this->newPredConjunction();
    PredicateBase*       pb= this->newPredicate();

    sp->op = nop;
    pc->push_back(sp);
    pb->push_back(pc);

    return *pb;
  }


  // ----------------------------------------------------------------------
  // PredConjunctionBase
  // ----------------------------------------------------------------------


  PredConjunctionBase::PredConjunctionBase(const PredConjunctionBase& rhs) {
    PredConjunctionBase::const_iterator it;
    for(it=rhs.begin(); it!=rhs.end(); ++it) {
      this->push_back( (*it)->cloneInstance() );
    }
  }

  /**
    * The distructor of this class
    */ 

  PredConjunctionBase::~PredConjunctionBase() {
	for (iterator it=begin();it!=end();++it){
		   delete *it;
	}
	
   }

  /**
   * Find if this PredConjunctionBase contains the SimplePredicateBase *sp .
   */

  bool PredConjunctionBase::find(SimplePredicateBase* sp) const {
    PredConjunctionBase::const_iterator it;
    
    // Checking if sp is present in this PredConjunctionBase
    for (it=this->begin();
	 it!=this->end() && **it != *sp ;
	 it++); // note the body of the for is empty
    
    return it!=this->end();
  }


  PredConjunctionBase&
  PredConjunctionBase::operator&=(const PredConjunctionBase& rhs) {
    PredConjunctionBase::const_iterator it;
    for(it=rhs.begin(); it!=rhs.end(); ++it) {
      if( !this->find(*it) )
	this->push_back( (*it)->cloneInstance() );
    }

    return *this;
  }


  PredicateBase&
  PredConjunctionBase::operator!() const {
    PredicateBase* result = newPredicate();
    PredConjunctionBase::const_iterator it;
    for(it=this->begin(); it!=this->end(); ++it) {
      (*result) |= !(**it);
    }
    
    return *result;
  }
  
  // ----------------------------------------------------------------------
  // PredicateBase
  // ----------------------------------------------------------------------


  PredicateBase& PredicateBase::operator=(const PredicateBase& rhs) {
    //freeing used memory
    for( iterator it=begin(); it!=end(); it++ ) {
      delete *it;
    }

    // copying rhs contents
    for(PredicateBase::const_iterator it=rhs.begin(); it!=rhs.end(); it++) {
      push_back( (*it)->cloneInstance() );
    }

    return *this;
  }

  PredicateBase::PredicateBase( const PredicateBase& rhs ) {
    PredicateBase::const_iterator it;
    for(it=rhs.begin(); it!=rhs.end(); it++) {
      push_back( (*it)->cloneInstance() );
    }
  }

  PredicateBase::~PredicateBase() {
    for( iterator it=begin(); it!=end(); it++ ) {
      delete *it;
    }
  }

  PredicateBase& PredicateBase::operator&=(const PredicateBase& p) {
    size_t oldSize = this->size();
    // making room for the new OR predicates
    PredicateBase* thisCopy = this->cloneInstance();
    for(size_t i=0; i<p.size()-1; i++) {
      *this |= *thisCopy;
    }

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

  PredicateBase& 
  PredicateBase::operator|=(const PredicateBase& p) {
    for(PredicateBase::const_iterator it=p.begin(); it!=p.end(); it++) {
      push_back( (*it)->cloneInstance() );
    }

    return *this;
  }


  PredicateBase&
  PredicateBase::operator!() const {
    assert( this->size() != 0 );

    PredicateBase* result = newPredicate();
    PredicateBase::const_iterator it=this->begin();

    (*result) |= !(**it);
    ++it;

    for(; it!=this->end(); ++it ) {
      (*result) &= !(**it);
    }

    return *result;
  }

#if 0
  PredicateBase* 
  PredicateBase::negate_or() const {
    PredicateBase* p1; /* Temporary predicate (it will contain the predicate which will be returned) */ 
    PredConjunctionBase* pc1;
    SimplePredicateBase* tmp;
    PredConjunctionBase::iterator it_pc;
    PredicateBase::const_iterator it_p;
    PredicateBase::iterator it_p2;

    it_p = this->begin();

    assert(it_p!=this->end());

    // Initializing temporary list (only the first time we enter the loop)
    p1= this->newInstance();
    for (it_pc=(*it_p)->begin();it_pc!=(*it_p)->end(); it_pc++){
      pc1= (*it_p)->newInstance();
      // aggiungo a p1 il risultato dell'append tra la lista pc1 (predConjunction) e l'elemento *it_pc (SimplePredicateBase) negato
      pc1->push_back((*it_pc)->negate_predicate());
      p1->push_back(pc1);
    }

    it_p++;

    for (;it_p!=this->end() ;it_p++) {
      /* Temporary predicate (used at each iteration to hold*/
      PredicateBase* p2 = this->newInstance();

      for (it_pc=(*it_p)->begin();it_pc!=(*it_p)->end(); it_pc++){//ciclo per ogni elemento di *it_p	
	tmp=(*it_pc)->negate_predicate();
	for (it_p2=p1->begin();it_p2!=p1->end();it_p2++){//ciclo per ogni sottolista di p1	
	  //se l'elemento e' gia presente non lo inserisco
	  if (!(*it_p2)->find(tmp))	
	    p2->push_back((*it_p2)->copy_and_append(tmp));	
	  else 
	    p2->push_back(*it_p2);
	}
	delete tmp;
	delete p1;
	p1=p2;
      }
    }

    return p1;
  }

#endif

} // namespace

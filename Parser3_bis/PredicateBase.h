#ifndef __PREDICATEBASE_H__
#define __PREDICATEBASE_H__


#include <iterator>
#include <vector>
#include <iostream>
#include <cassert>
/**
 * this file contains the basic class for modelling Predicate in disjunction form
 *
 */

namespace minerule {

  class PredicateBase;
  class PredConjunctionBase;

  // ----------------------------------------------------------------------
  // SimplePredicateBase
  // ----------------------------------------------------------------------

  /**
   * This class represent a simple Predicate in form of term comparison term
   */


  class SimplePredicateBase  {
    std::string val1;
    std::string val2;
    std::string op;
  public:
	
    /**
     * the Constructor of class SimplePredicate
     * @param string v1 the first term
     * @param string op the comparison term
     * @param string v2 the second term
     */
    SimplePredicateBase( const std::string& v1, 
			 const std::string& o, 
			 const std::string& v2 ) :
      val1(v1), val2(v2),  op(o) {  }
    
    /**
     * The empty constructor
     */ 
    SimplePredicateBase() {}

    /**
     * The copy constructor
     * @param SimplePredicateBase s another object of this class that i want copy
     */
    SimplePredicateBase( const SimplePredicateBase& s) : 
      val1(s.val1), val2(s.val2), op(s.op) {}


 /**                                                                                                                                                              * this method is very important because permit to clone the exact objcet in the hierachy of class                                                               * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                          * subclass of this class you must overriding this method for your class                                                                                         * @return SimplePredicateBase the object of super-class that incapsule a new object of this class                                                           */
   
    virtual SimplePredicateBase* cloneInstance() const {
      return new SimplePredicateBase(*this);
    }
    /**                                                                                                                                                              * this method is very important because permit create a new instance of the exact objcet in the hierachy of the class                                          * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                          * subclass of this class you must overriding this method for your class                                                                                        * @return SimplePredicateBase the object of super-class that incapsule a new object of this class                                                              */
   
    virtual SimplePredicateBase* newInstance() const {
      return new SimplePredicateBase();
    }

    /**
     * This is the destructor of class
     */
    virtual ~SimplePredicateBase() {}

    /**
     *
     */
    virtual PredicateBase* newPredicate() const;
    
    /**
     *
     */
    virtual PredConjunctionBase* newPredConjunction() const;
    /**
     * This method return a pointer to first term of Simple Predicate
     *@return string& val1 the firt term
     */
    const std::string& getVal1() const {
      return val1;  
    }
    
    /**
     * This method return a pointer to second term of Simple Predicate
     * @return string& val2 the second term
     */
    const std::string& getVal2() const {
      return val2;
    }
    
    /**
     * This method return a pointer to comparison operator of Simple Predicate
     * @return string& op the comparison operator
     */
    const std::string& getOp() const {
      return op;
    }
    
    /**
     * Redefine the operator ==
     * @return true if and only if all the part of Simple Predicate are equals
     */
    virtual bool operator==( const SimplePredicateBase& rhs ) const {
      return 
	val1==rhs.val1 &&
	val2==rhs.val2 &&
	op==rhs.op;
    }
    
    /**
     * Redefine the operator !=
     * @return true if at least one of the part of Simple Predicate ar different
     */
    virtual bool operator!=( const SimplePredicateBase& rhs ) const {
      return 
	val1!=rhs.val1 ||
	val2!=rhs.val2 ||
	op!=rhs.op;
    }

    /**
     * Redefine the < operator
     * @return true if the first term is lesser than second
     */	
    virtual bool operator<(const SimplePredicateBase& rhs) const {
      return val1+op+val2 < rhs.val1+rhs.op+rhs.val2;
    }

    /**
     * Define a method that return a new PredicateBase& that contains the negation of this Simple Predicate
     * @return PredicateBase& that contains the negation of this Simple Predicate
     */
    virtual PredicateBase& operator!() const;
  };
  

  // ----------------------------------------------------------------------
  // PredConjunctionBase 
  // ----------------------------------------------------------------------
  /**
   * this class represent a conjunction of simplePredicate
   */
  class PredConjunctionBase : public std::vector<SimplePredicateBase*> {
  public:
    /**
     * The empty constructor
     */
    PredConjunctionBase() {};
    
    /**
     * The copy constructor
     */
    PredConjunctionBase( const PredConjunctionBase& );
    
    /**
     * The distructor of this class
     */
    virtual ~PredConjunctionBase() ;

    /**
     *
     */
    virtual PredicateBase* newPredicate()const ;
       
    /**
     *
     */
    virtual PredConjunctionBase* newPredConjunction()const;
    

/**                                                                                                                                                            * this method is very important because permit to clone the exact objcet in the hierachy of class                                                              * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                          * subclass of this class you must overriding this method for your class                                                                                        * @return PredConjunctioBase a new object that was a copy of this object 
     */
    virtual PredConjunctionBase* cloneInstance() const { 
      return new PredConjunctionBase(*this);
    }
    
    /**                                                                                                                                                              * this method is very important because permit to create a new instance of the exact objcet in the hierachy of class                                          * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                        * subclass of this class you must overriding this method for your class                                                                                          * @return PredConjunctioBase a new object of this class                                                                                                       */
    virtual PredConjunctionBase* newInstance() const {
      return new PredConjunctionBase();
    }

   
    /**
     * Builds the logical and of *this and p and return *this.
     */
    PredConjunctionBase& operator&=(const PredConjunctionBase& p);

    /**
     * Builds the negation of this PredConjunctionBase and return a PredicateBase object (in form disjunctive)
     * @return PredicateBase& the negation of this conjunction predicate 
     */
    PredicateBase& operator!() const;
         
    /**
     * Find if this PredConjunctionBase contains the SimplePredicateBase *sp .
     */
    bool find(SimplePredicateBase* sp) const;
	
    /**
     * Build a new PredConjunctionBaseParse that contain the old vector plus the new 
     * element and not modify the old vector
     * @param SimplePredicateBase* sp the simple term that i want insert in the new PredConjunctionBase 
     * 
     */  	
    virtual PredConjunctionBase* copy_and_append(SimplePredicateBase* sp){
      PredConjunctionBase* conj =  this->cloneInstance();
      conj->push_back( sp->cloneInstance() );
      return conj;
    }    
  };

  // ----------------------------------------------------------------------
  // Predicate. Notice: this is a vector of Predicate Conjunctions with
  //       another name and some handy functions. 
  // ----------------------------------------------------------------------
  /**
   * This class represent a disjunction of conjunction(PredConjunctionBase) of condition (simplePredicate)
   */

  class PredicateBase : public std::vector<PredConjunctionBase*> {
  public:
    /**
     * The empty constructor
     */
    PredicateBase() {};
    
    /**
     * The copy constructor
     */
    PredicateBase(const PredicateBase&);
    
    /**                                                                                                                                                            * this method is very important because permit to clone the exact objcet in the hierachy of class                                                              * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                        * subclass of this class you must overriding this method for your class                                                                                        * @return PredicateBase a copy of this object                                                                                                                 */
    
    virtual PredicateBase* cloneInstance() const {
      return new PredicateBase(*this);
    }
    
    /**                                                                                                                                                             * this method is very important because permit to create a new instance of the exact objcet in the hierachy of class                                           * because the RTTI mechanism use the right method with a specific type of object. if you want create a                                                          * subclass of this class you must overriding this method for your class                                                                                     * @return PredidcateBase a new instance of this class
      */
    virtual PredicateBase* newInstance() const {
      return new PredicateBase();
    }

    /**
     * This is the descructor of this class
     */
    virtual ~PredicateBase();
    
    /**
     *
     */
    virtual PredicateBase* newPredicate()const;

    /**
     *
     */
    virtual PredConjunctionBase* newPredConjunction() const; 
   
    /**
     * Redefine the assigment operation 
     */
    virtual PredicateBase& operator=(const PredicateBase& rhs);

    /** 
     * Builds the logical and of *this and p and returns *this
     **/
    PredicateBase& operator&=( const PredicateBase& p  );

    /**
     * Builds the logical or of *this and p and returns *this
     */
    PredicateBase& operator|=(const PredicateBase& p );


    /**
     *merge unordered the two vector into the first and delete all the element of the vector pass as parameter
     */
    void merge(PredicateBase* l2) {
      copy( l2->begin(), l2->end(), std::back_insert_iterator<PredicateBase>(*this));
      erase(l2->begin(), l2->end());

      /*    Predicate::iterator it_and;
	    it_and=l2->begin();
	    while (it_and!=l2->end()){
	    this->push_back(*it_and);
	    *it_and=NULL;
	    it_and++;
	    }*/
    }
	
    /**                                                                                                                                                              * Builds the logical negation but i return another object of this class (in disjunction form)                                                                  */ 
    PredicateBase&  operator!() const;

    /**
     * This is a simple function that print a PredicateBase object
     */	
    void stamp(){
      if (this!=NULL){
	int c_and=0;
	int c_or=0;
	PredicateBase::iterator it_and = this->begin();
	PredConjunctionBase::iterator it_sp;
	while (it_and!=this->end()){
	  if (c_or) std::cout<<"\n OR \n";
	  it_sp=(*it_and)->begin();
	  c_and=0;
	  while(it_sp!=(*it_and)->end()){
	    if (c_and) std::cout<<" AND ";
	    std::cout<<(*it_sp)->getVal1()<<" ";
	    std::cout<<(*it_sp)->getOp()<<" ";	
	    std::cout<<(*it_sp)->getVal2();
	    it_sp++;
	    c_and++;
	  }
	  //delete *it_and;
	  it_and++;
	  c_or++;
	}
	std::cout<<"\n\n\n\n";
      }
		
    }

  };


} // namespace
 
#endif

#include "PredicateUtils/ExpressionNFCoder.h"

namespace minerule {

  /* ======================================================================
   * ExpressionNFCoder
   * ====================================================================== */

  const size_t ExpressionNFCoder::bitsPerCell = (sizeof(EncodingBaseType)<<3);


  EncodedNF
  ExpressionNFCoder::encode(Predicate& ee) 
    throw(MineruleException) {

    cleanBuf();

    size_t nvars = ee.getNumVariables();
    VarSetEnumerator vse(nvars);

    if( nvars > 31 ) {
     std::string error = 
	"ExpressionNFCoder::encode - the current implementation "
	"does not support expression with more than 31 variables";

      throw MineruleException(MR_ERROR_INTERNAL,error);
    }



    // We are now going to evaluate the maximal storage required for the
    // encoding. Its value can be evaluated as nvars*numSets/bitsPerCell
    // Where bitsPerCells is the number of bits that each cell can hold
    // (which is sizeof(EncodingBaseType)*8 == sizeof(EncodingBaseType)<<3);
    // nvars is the number of variables and numSets is the value returned
    // by numEnumerations (i.e., 2^numVars).
    // In order to avoid overflows in the integer representation of the
    // result, we postpone the multiplication after the division. Thanks to
    // this little trick, we can manage up to 31 variables (in fact the maximal
    // value we can hold in a size_t element is 2^32. Assuming 31 variables we
    // end up to the following number: (2^31/2^5) * 31, the division output
    // 2^26 which can be safely multiplied by 31 without causing any overflow.
    // Note that the calculation has been performed assuming that
    // sizeof(EncodingBaseType)*8 == 32, which is true in the current
    // implementation but should be checked when the system is ported to other
    // platforms or when EncodingBaseType is set to be different from uint.


    assert(sizeof(EncodingBaseType)==4);
    size_t numCells = (1<<nvars) / bitsPerCell;
    numCells *= nvars;
    if((1<<nvars)%bitsPerCell > 0)
      numCells++;
 
    // we use malloc instead of new, because we need to use realloc later on
    buf = (EncodingBaseType*) calloc( numCells, sizeof(EncodingBaseType));

    size_t cellCounter = 0;
    size_t elemCounter = 0;
  
    while( vse++ ) {
      if(ee.evaluate(*vse) && !filter(*vse)) {
	for( size_t i=0; i<(*vse).size(); i++ ) {
	  if((*vse).getVar(i))
	    buf[cellCounter] |=  1<<elemCounter;
	  else
	    buf[cellCounter] &= ~(1<<elemCounter);

	  if(++elemCounter>=bitsPerCell) {
	    cellCounter++;
	    elemCounter=0;
	  }
	}
      } 
    }

    buf = (EncodingBaseType*) realloc(buf,sizeof(EncodingBaseType) * (cellCounter+1));
    EncodedNF result;
    result.encVector = buf;
    result.vecSize = cellCounter+1;
    result.codingLen = cellCounter*bitsPerCell+elemCounter;
    result.numVars = ee.getNumVariables();

    return  result;
  }


  /* ======================================================================
   * EncodedNFIterator
   * ======================================================================
   */

  bool 
  EncodedNFIterator::operator++(int) {
    static const size_t baseTypeSize = (sizeof(EncodingBaseType)*8);

    if(cellCounter*baseTypeSize+elemCounter>=target.codingLen) {
      setOk(false);
      return false;
    }

    setOk(true);
  
    for( size_t i=0; i<target.numVars; i++ ) {
      bool elem = target.encVector[cellCounter] & (1<<elemCounter);
      currentVarSet.setVar(i, elem);
      
      if(++elemCounter >= baseTypeSize) {
	++cellCounter;
	elemCounter=0;
      }
    }
  
    return true;
  }


  /* ======================================================================
   * EncodedNF
   * ======================================================================
   */

  bool
  EncodedNF::operator==(const EncodedNF& nf) const {
    if(numVars   != nf.numVars || 
       vecSize   != nf.vecSize || 
       codingLen != nf.codingLen )
      return false;

    for(size_t i=0; i<vecSize; i++) {
      if( encVector[i]!=nf.encVector[i] )
	return false;
    }

    return true;
  }

#if 0
  // Old version of getCodesRelationship. This version seems to be buggy.
  // The problem is that it compares the two encoded vectors element by element.
  // This would be correct if the vectors would represent the last column of the
  // truth table of the two predicates, which is false. The vectors represent
  // the lines in the table having one in the last column. Hence we could have
  // something similar to (I'm assuming 3 variables): v1[0]= 001 101 11 and
  // v1[1]= 1 000 000 0 (only the first one represent an interesting bit in
  // v[1]), and v2[0]= 101 000 00 (only the first three ones are interesting).
  // Here we have that v2 is more specific than v1, since v1 contains all the
  // lines that have a one in v2 (i.e., it contains 101).

  EncodedNF::CodesRelationship
  EncodedNF::getCodesRelationship( const EncodedNF& nf1,
				   const EncodedNF& nf2 ) {
    const char* UNEXPECTED_ERRMSG =
      "Unexpected relationship state... (this is a bug!)";


    if(nf1.numVars   != nf2.numVars || 
       nf1.vecSize   != nf2.vecSize || 
       nf1.codingLen != nf2.codingLen )
      return Unrelated;

    CodesRelationship relationship = Equivalent;

    for(size_t i=0; i<nf1.vecSize; i++) {

      if( nf1.encVector[i]==nf2.encVector[i] )
	continue;

      EncodingBaseType codeIntersection = 
	nf1.encVector[i] & nf2.encVector[i];

      if( codeIntersection == nf1.encVector[i] ) { 
	// nf1.encVector[i] is included in nf2.encVector[i]
	switch( relationship ) {
	case Equivalent:
	  relationship = FirstMoreSpecific;
	  break;
	case FirstMoreSpecific:
	  continue;
	case FirstMoreGeneral: 
	  return Unrelated;
	default:
	  throw MineruleException(MR_ERROR_INTERNAL,UNEXPECTED_ERRMSG);
	}
      } // if codeIntersection==nf1...

      if( codeIntersection == nf2.encVector[i] ) {
	// nf2.encVector[i] is included in nf1.encVector[i]
	switch( relationship ) {
	case Equivalent:
	  relationship = FirstMoreGeneral;
	  break;
	case FirstMoreSpecific:
	  return Unrelated;
	case FirstMoreGeneral:
	  continue;
	default:
	  throw MineruleException(MR_ERROR_INTERNAL,UNEXPECTED_ERRMSG);
	}
      } // if codeIntersection==nf2

      return Unrelated;
    }

    return relationship;
  }
#endif
  

  std::ostream& operator<<(std::ostream& os, const EncodedNF& nf) {
    EncodedNFIterator it(nf);
    it++;
    while(it.ok()) {
      os << *it << ".";
      it++;
    }

    return os;
  }


  EncodedNF::CodesRelationship
  EncodedNF::getCodesRelationship( const EncodedNF& nf1,
				   const EncodedNF& nf2 ) {
    if(nf1.numVars   != nf2.numVars)
      return Unrelated;

    CodesRelationship relationship = Equivalent;
   
    EncodedNFIterator it1(nf1);
    EncodedNFIterator it2(nf2);
    it1++;
    it2++;
    while(it1.ok() && it2.ok()) {
      if(*it1 == *it2) {
	it1++;
	it2++;
	continue;
      }

      if(*it1 < *it2 ) { 
	// in nf2 there is *not* the current value of
	// it1. Then nf1 does not contain at least one
	// disjunct that is present in nf2, and hence
	// this is evidence that nf1 is more general
	if(relationship==FirstMoreSpecific)
	  return Unrelated;
	else
	  relationship=FirstMoreGeneral;

	it1++;
	continue;
      }

      if(*it1 > *it2 ) {
	// in nf1 there is *not* the current value of
	// it2. Then nf1 does not contain at least one
	// disjunct that is present in nf2, and hence
	// this is evidence that nf2 is more general
	if(relationship==FirstMoreGeneral)
	  return Unrelated;
	else
	  relationship=FirstMoreSpecific;

	it2++;
      } 
    }
    
    if( !it1.ok() && !it2.ok() )
      return relationship;

    if( it1.ok() ) {
      // it1.ok() && !it2.ok() suggests 
      // nf1 is more general
      if( relationship==FirstMoreSpecific )
	return Unrelated;
      else
	return FirstMoreGeneral;
    }

    // !it1.ok() & it2.ok() suggests that
    //  nf2 is more general
    if( relationship==FirstMoreGeneral ) 
      return Unrelated;
    else
      return FirstMoreSpecific;
  }

  /**
   * The result of this function is equivalent to the following
   * algorithm: 
   *
   * 1. write down the sequence s1 of the ones and zeros of the truth
   *    table of the predicate that nf1 represents. 
   * 2. Do the same with nf2 and call the resultingstd::string s2. 
   * 3. Return the Hamming distance of s1 w.r.t. s2
   * in other words the function count the number of variable configurations
   * for which the two predicates return different answers.
   * 
   * The implementation does not match exactly the above algorithm only
   * because nf1 and nf2 does not contain the s1 and s2 strings (and
   * building s1 and s2 from them would be costly at execution time).
   * 
   * The implementation, instead iterates thru nf1 and nf2 counting 
   * the number of times they do not match.
   */

  size_t EncodedNF::computeHammingDistance(const EncodedNF& nf1, 
					   const EncodedNF& nf2) {
    assert(nf1.numVars == nf2.numVars);
    size_t edt = 0;

    EncodedNFIterator it1(nf1);
    EncodedNFIterator it2(nf2);
    it1++;
    it2++;
    while(it1.ok() && it2.ok()) {
      if( *it1 == *it2 ) {
	it1++;
	it2++;
	continue;
      }

      if(*it1 < *it2) {
	edt++;
	it1++;
	continue;
      }

      assert(*it1>*it2);
      edt++;
      it2++;
    }

    while(it1.ok()) {
      edt++;
      it1++;
    }

    while(it2.ok()) {
      edt++;
      it2++;
    }

    return edt;
  }


} // namespace



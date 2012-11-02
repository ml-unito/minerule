#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include "Utils/SQLUtils.h"

namespace minerule {

void HeadBodyPredicatesSeparator::setState( ConstraintClass& current, ConstraintClass newone )  throw(MineruleException) {
  if( current==NO_INFO || current==newone ) {
    current=newone;
    return;
  }

  current=ERROR;
  throw MineruleException(MR_ERROR_INTERNAL,
			  "The optimizer found a constraint defined  on both BODY and HEAD!");

} 

HeadBodyPredicatesSeparator::ConstraintClass
HeadBodyPredicatesSeparator::setStateAccordinglyToString(string& str, ConstraintClass state) throw(MineruleException)  {
  string substr = str.substr(0,5);
  if(substr=="BODY.")
    setState(state, BODY_CONSTR);
  else if(substr=="HEAD.")
    setState(state, HEAD_CONSTR);
  else if(SQLUtils::isAttribute(str))
    throw MineruleException(MR_ERROR_INTERNAL,
			    "The optimizer found a constraint defined neither on BODY nor on HEAD!");
  else return state;
  
  // we need to remove the HEAD. and BODY. part of the string
  SQLUtils::removeHeadBodyFromAttrName( str );
  return state;
}

void
HeadBodyPredicatesSeparator::updateConstraint( string& str, const string& v1, const string& op, const string& v2) {
  if(str!="")
    str+=" AND ";

  str+=v1+" "+op+" "+v2;
}

void
HeadBodyPredicatesSeparator::separate(list_AND_node* l_and, 
			 string& bodyConstraints,
			 string& headConstraints) throw(MineruleException) {
  if( l_and == NULL ) {
    bodyConstraints = "1=1";
    headConstraints = "1=1";
    return;
  }

  while(l_and!=NULL) {
    ConstraintClass state = NO_INFO;
    string v1 = l_and->sp->val1;
    string v2 = l_and->sp->val2;
    
    state = setStateAccordinglyToString(v1, state);
    state = setStateAccordinglyToString(v2, state);
   
    switch(state) {
    case BODY_CONSTR:
      updateConstraint(bodyConstraints, v1, l_and->sp->op, v2);
      break;
    case HEAD_CONSTR:
      updateConstraint(headConstraints, v1, l_and->sp->op, v2);
      break;
    default:
      throw MineruleException(MR_ERROR_INTERNAL,
			       "The optimizer found a constraint defined neither on BODY nor on HEAD!");
    };

    l_and=l_and->next;
  }
}

} // namespace

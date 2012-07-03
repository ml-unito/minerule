#include <iostream>
#include "PredicateUtils/PredicateUtils.h"
#include "Utils/MRLogger.h"
#include "Utils/Converter.h"

using namespace std;
using namespace minerule;

list_OR_node* newPredicate(size_t i) {
  char* buf=new char[2];
  list_OR_node* p = new list_OR_node;
  p->l_and = new list_AND_node;
  p->next = NULL;
  p->l_and->next = NULL;
  p->l_and->sp = new simple_pred;
  buf[0]='A'+char(i)-1;
  buf[1]='\0';
  p->l_and->sp->val1 = buf;
  p->l_and->sp->val2 = "5";
  p->l_and->sp->op = "<";

  return p;
}

list_OR_node* newPredicateList(size_t i) {
  list_OR_node* result=NULL;
  if(i>0)
   result=newPredicate(i);

  i--;

  list_OR_node* resPtr = result;
  while(i>0) {
    resPtr->next = newPredicate(i);
    i--;
    resPtr = resPtr->next;
  }

  return result;
}


void freePredicateList(list_OR_node* p) {
  list_OR_node* prevP;
  while( p!=NULL ) {
    prevP=p;
    p=p->next;
    delete prevP->l_and->sp->val1;
    delete prevP->l_and->sp;
    delete prevP->l_and;
    delete prevP;
  }
}


int
main(int argc, char** argv) {
  const size_t MAXPREDS = 30;
  ofstream os("optinfo.txt");
  os << "# i cpu d-cpu time d-time" << endl;

  list_OR_node* p = newPredicateList(1);
  MRLogger logger(cout);
  

  for( size_t i=1; i<MAXPREDS; i++ ) {
    list_OR_node* p2 = newPredicateList(i);

    logger.push("Starting analysis... i="+
		Converter(long(i)).toString());
    logger.push("Building first pred description");
    Predicate pred1(p);
    logger.pop();

    logger.push("Building second pred description len="+
		Converter(long(i)).toString());
    Predicate pred2(p2);
    logger.pop();

    logger.push("Evaluating equivalence...");
    if(PredicateUtils::predicatesAreEquivalent(pred1,pred2))
      logger.log() << "the predicates are equivalent" << endl;
    else
      logger.log() << "the predicates are NOT equivalent!" << endl;
    logger.pop();

    os << i << " "
       << logger.getCurrentCpuSecs() << " "
       << logger.getCurrentCpuDelta() << " "
       << logger.getCurrentTimeSecs() << " "
       << logger.getCurrentTimeDelta() << endl;
    logger.pop();
    freePredicateList(p2);
  }

  freePredicateList(p);
}

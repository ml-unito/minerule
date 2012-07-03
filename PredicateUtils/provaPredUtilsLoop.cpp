#include "PredicateUtils.h"
#include "Parsers/ParsedMinerule.h"
#include <iterator>
#include "Utils/MineruleOptions.h"

using namespace std;
using namespace minerule;

typedef struct {
  char* v1;
  char* op;
  char* v2;
} Pred;

Pred preds1[] = {
  {"price",">","100"},
  {"price","<","10000"},
  {"price",">","200"},
  {"price","<","9000"},
  {"price",">","300"},
  {"price","<","8000"},
  {"price",">","400"},
  {"price","<","7000"},
  {"price",">","500"},
  {"price","<","6000"},
  {"price",">","600"},
  {"price","<","5000"},
  {"price",">","700"},
  {"price","<","4000"},
  {"price",">","800"},
  {"price","<","3000"},
  {"price",">","900"},
  {"price","<","2000"}
};

Pred preds2[] = {
  {"price",">","200"},
  {"price","<","9000"},
  {"price",">","300"},
  {"price","<","8000"},
  {"price",">","400"},
  {"price","<","7000"},
  {"price",">","500"},
  {"price","<","6000"},
  {"price",">","600"},
  {"price","<","5000"},
  {"price",">","700"},
  {"price","<","4000"},
  {"price",">","800"},
  {"price","<","3000"},
  {"price",">","900"},
  {"price","<","2000"},
  {"price",">","950"},
  {"price","<","1000"}
};

void
printConj(PredConjunction& p) {
  PredConjunction::iterator it;
  for(it=p.begin(); it!=p.end(); it++) {
    if(it!=p.begin()) 
      cout << " AND ";

    cout << (*it)->getVal1() << (*it)->getOp() << (*it)->getVal2();
  }

}

void
printPredicate(Predicate& p) {
  Predicate::iterator it;
  for(it=p.begin(); it!=p.end(); it++) {
    if(it!=p.begin())
      cout << " OR ";
    cout << "(";
    printConj(**it);
    cout << ")";
  }

}

ostream& operator<<(ostream& os, const SimplePredicate* sp) {
  os << sp->getVal1() << sp->getOp() << sp->getVal2();
}


list_OR_node* addPredCondition( int i, Pred preds[], list_OR_node* l ) {
  if(l==NULL) {
    l=new list_OR_node;
    l->l_and = NULL;
    l->next  = NULL;
  }

  list_AND_node* tmp = new list_AND_node;
  tmp->next = l->l_and;;
  tmp->sp = new simple_pred;
  tmp->sp->val1 = preds[i].v1;
  tmp->sp->op   = preds[i].op;
  tmp->sp->val2 = preds[i].v2;
  l->l_and = tmp;

  return l;
}


int 
main(int argc, char** argv) {

  MineruleOptions::getSharedOptions().readFromFile("mroptions.txt");

  ofstream results("results.txt");
  list_OR_node* pred1=NULL;
  list_OR_node* pred2=NULL;

  pred1 = addPredCondition(0, preds1, pred1);
  pred2 = addPredCondition(0, preds2, pred2);

  for( int i=1; i<18; i++ ) {
    Predicate p1(pred1);
    Predicate p2(pred2);
    cout << "p1:";
    printPredicate(p1);
    cout << endl;
    cout << "p2:";
    printPredicate(p2);
    cout << endl;

    int npreds = i==0?2:(i+3);

    MRLogPush(Converter(npreds).toString());
    results << PredicateUtils::getPredicateRelationship(p1,p2,"Sales") << " ";
    results << npreds << " "
	    << MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().getCurrentTimeDelta()
	    << endl;
    MRLogPop();

    pred1 = addPredCondition(i, preds1, pred1);
    pred2 = addPredCondition(i, preds2, pred2);
  }
  

    /*  } catch (std::exception& e) {
    cout << e.what() << endl;
    throw;
    }*/

  return 0;
}

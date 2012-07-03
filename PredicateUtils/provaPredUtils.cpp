#include "PredicateUtils.h"
#include "Parsers/ParsedMinerule.h"
#include <iterator>
#include "Utils/MineruleOptions.h"

using namespace std;
using namespace minerule;

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

int 
main(int argc, char** argv) {

  MineruleOptions::getSharedOptions().readFromFile("mroptions.txt");

  if(argc!=3) {
    cerr << "Use " << argv[0] << " pred1 pred2" << endl;
    exit(1);
  }

  //  try {
    list_OR_node* pred1 = predicate_parser(argv[1]);
    list_OR_node* pred2 = predicate_parser(argv[2]);

  
    Predicate p1(pred1);
    Predicate p2(pred2);

    /*
      cout << "pred1:";
      printPredicate(p1);
  
      cout << endl << "pred2:";
      printPredicate(p2);
      cout << endl;


      set<SimplePredicate*,PtrSimplePredComp>& set1 = p1.getPredicateList();
      set<SimplePredicate*,PtrSimplePredComp>& set2 = p2.getPredicateList();
      set<SimplePredicate*,PtrSimplePredComp> setunion;


      set_union( set1.begin(), set1.end(), 
      set2.begin(), set2.end(),
      insert_iterator< set<SimplePredicate*,PtrSimplePredComp> >(setunion, setunion.begin()));

      set<SimplePredicate*,PtrSimplePredComp>::iterator it;
      for(it=setunion.begin(); it!=setunion.end(); it++) {
      cout << *it << endl;
      } */
  
  
    switch( PredicateUtils::getPredicateRelationship(p1,p2,"Sales") ) {
    case EncodedNF::Equivalent:
      cout << "Predicates are found to be equivalent" << endl;
      break;
    case EncodedNF::FirstMoreGeneral:
      cout << "first is more general" << endl;
      break;
    case EncodedNF::FirstMoreSpecific:
      cout << "first is more specific" << endl;
      break;
    case EncodedNF::Unrelated:
      cout << "Unrelated" << endl;
      break;
    }

    /*  } catch (std::exception& e) {
    cout << e.what() << endl;
    throw;
    }*/

  return 0;
}

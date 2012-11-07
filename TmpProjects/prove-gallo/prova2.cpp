#include <vector>
#include <iterator>
#include <iostream>

typedef std::vector<int> ItemSetType;
#include "SubsetIterator.h"

using namespace std;
using namespace minerule;


//per algo distruttivo
//costruisco tutti itemset con almeno un elemento appartenente al primo vettore
//simile alla creazione di regole, ma ora AC->BD e' uguale a A->BCD.
//Li distinguo mettendo tutto in un std::set<int> e poi passo tutto in un vettore

void
buildHeads(const vector<int>& body, const vector<int>& heads) {
  vector<int> curHeadSet;
  back_insert_iterator< vector<int> > ins_it(curHeadSet);
  set_difference(heads.begin(), heads.end(),
		 body.begin(), body.end(),
		 ins_it);
  SubsetIterator it(curHeadSet);
  std::set<int> tmp;
  while(it.next()) {
    vector<int> curHead;
    it.getCurrentSet(curHead);
    vector<int>::iterator itint=body.begin();
    while(itint!=body1.end()){
       tmp.insert(*itint);
       itint++;
    }
    itint=curHead.begin();
    while(itint!=body2.end()){
       tmp.insert(*itint);
       itint++;
    }
    std::set<int>::iterator its=tmp.begin();
    cout<<"itemset:"<<std::endl;
    while(its!=tmp.end()){
      cout<<(*its)<<" ";
      its++;
    }
    cout<<std::endl;
    /*copy(body.begin(), body.end(),
	 std::ostream_iterator<int>(cout, " "));
    cout << " -> ";

    copy(curHead.begin(), curHead.end(),
	 std::ostream_iterator<int>(cout, " "));

    cout << endl;*/
  }
}

int
main(int argc, char** argv) {
  vector<int> body;
  body1.push_back(2);
  body1.push_back(3);
  body1.push_back(8);
  body1.push_back(9);

  vector<int> head;
  body2.push_back(1);
  body2.push_back(2);
  body2.push_back(3);

  SubsetIterator si(body1);
  while(si.next()) {
    vector<int> result;
    si.getCurrentSet(result);
    buildHeads(result,body2);
  }

  return 0;
}

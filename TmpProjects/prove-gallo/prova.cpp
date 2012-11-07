#include <vector>
#include <iterator>
#include <iostream>

typedef std::vector<int> ItemSetType;
#include "SubsetIterator.h"

using namespace std;
using namespace minerule;


void
buildHeads(const vector<int>& body, const vector<int>& heads) {
  vector<int> curHeadSet;
  back_insert_iterator< vector<int> > ins_it(curHeadSet);
  set_difference(heads.begin(), heads.end(),
		 body.begin(), body.end(),
		 ins_it);
  SubsetIterator it(curHeadSet);
  while(it.next()) {
    vector<int> curHead;
    it.getCurrentSet(curHead);

    copy(body.begin(), body.end(),
	 std::ostream_iterator<int>(cout, " "));
    cout << " -> ";

    copy(curHead.begin(), curHead.end(),
	 std::ostream_iterator<int>(cout, " "));

    cout << endl;
  }
}

int
main(int argc, char** argv) {
  vector<int> body;
  body.push_back(2);
  body.push_back(3);
  body.push_back(8);
  body.push_back(9);

  vector<int> head;
  head.push_back(1);
  head.push_back(2);
  head.push_back(3);

  SubsetIterator si(body);
  while(si.next()) {
    vector<int> result;
    si.getCurrentSet(result);
    buildHeads(result,head);
  }

  return 0;
}

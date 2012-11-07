#include <vector>
#include <iterator>
#include <iostream>

typedef std::vector<int> ItemSetType;
#include "SubsetIterator.h"

using namespace std;
using namespace minerule;

//funziona

void
buildHeads(const vector<int>& body) {
    copy(body.begin(), body.end(),
	 std::ostream_iterator<int>(cout, " "));
    cout << endl;
}

int
main(int argc, char** argv) {
  vector<int> body;
  body.push_back(2);
  body.push_back(3);
  body.push_back(8);
  body.push_back(9);

  SubsetIterator si(body);
  while(si.next()) {
    vector<int> result;
    si.getCurrentSet(result);
    buildHeads(result);
  }

  return 0;
}


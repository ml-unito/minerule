// CORE.CPP

//using std::cout;
//using std::cin;
//using std::setiosflags;
//using std::ios;
//using std::setprecision;

// Qui definisco il tipo TYPENODE. Tutte le classi sono dichiarate come <String>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>



#include "Utils/MineruleOptions.h"
#include "MRDatabase/MIndex.h"


#define MROPTIONFILE "mroptions"

int main(int argc, char *argv[]) {
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  mrOpts.readFromFile(MROPTIONFILE);

  vector<int> groupCols;
  groupCols.insert(groupCols.end(), 1);
  vector<int> itemCols;
  itemCols.insert(itemCols.end(), 2);

  MIndex mi1("minidb","item",groupCols,itemCols);
  MIndex mi2("minidb","item",groupCols,itemCols);

  MIndexIterator * b2;
  MIndexIterator * h2;

  bt_query_t *query = new bt_query_t(bt_query_t::bt_gt,
				     (void*) "B",
				     NULL);
  b2 = new MIndexIterator(mi1, query);
  h2 = new MIndexIterator(mi2, query);

  
  try {
    int howMany =0;
    for (; b2->current() != b2->end(); (*b2)++) {
      ItemType gid(*SourceRowElement::deserializeElementFromString("n "+b2->getCurrentGID()));
      ItemType item(*SourceRowElement::deserializeElementFromString("g @[ "+b2->getCurrentItem()+" @]"));
      
      cout << gid.asString() << " " << item.asString() << endl;
    }
  } catch (std::exception& e) {
    cerr<<e.what()<<endl;
  }
}


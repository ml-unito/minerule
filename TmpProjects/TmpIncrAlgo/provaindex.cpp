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



#include "minerule/Utils/MineruleOptions.hpp"
#include "MRDatabase/MIndex.h"


#define MROPTIONFILE "mroptions"

int main(int argc, char *argv[]) {
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  mrOpts.readFromFile(MROPTIONFILE);

  vector<int> groupCols;
  groupCols.insert(groupCols.end(), 1);
  vector<int> itemCols;
  itemCols.insert(itemCols.end(), 2);

  MIndex mi1("Sales","descCategory",groupCols,itemCols);

  MIndexIterator * b2;
  //  MIndexIterator * h2;

  bt_query_t *query = new bt_query_t(bt_query_t::bt_betw,
				     (void*) "a",(void*)"b");
  b2 = new MIndexIterator(mi1, query);

  //h2 = new MIndexIterator(mi2, query);

  
  try {
    int howMany =0;
    ItemType oldGid;
    for (; b2->current() != b2->end(); (*b2)++) {
      ItemType gid(*SourceRowElement::deserializeElementFromString("n "+b2->getCurrentGID()));
      //  ItemType item(*SourceRowElement::deserializeElementFromString("g @[ "+b2->getCurrentItem()+" @]"));
      
      cout << gid << endl;

      oldGid = gid;
      howMany++;
    }
  } catch (std::exception& e) {
    std::cerr<<e.what()<<std::endl;
  }
}


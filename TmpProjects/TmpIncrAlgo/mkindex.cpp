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

  MIndex mi1("minidb","item",groupCols,itemCols);
  if(!mi1.existIndex())
    mi1.createIndex();
}



#ifndef ALGO_H
#define ALGO_H
#include "Database/ItemType.h"
#include <vector>
#include <iostream>
#include <map>
#include "classi.h"
#include "MRDatabase/MIndex.h"

namespace minerule {

class DestrTree/*:public IncrementalAlgorithms*/{

 private:
 Body* root;


 public:
 DestrTree(){root = new Body();}
 Body* getRoot(){return root;}
 Body* insertRulesInStructure(const std::string& queryname, int ngroups);
 /*Body* adjustSupp(MIndexIterator* b1nb2,MIndexIterator* h1nh2,
                  MIndexIterator* b1,MIndexIterator* h1);*/
};



class ConstrTree /*:public IncrementalAlgorithms*/{

 private:
 Body* root;
 int ngroups;

 public:
 ConstrTree(){root = new Body();ngroups=0;}
 ~ConstrTree(){delete root;}
 int getNGroups(){return ngroups;}
 Body* getRoot(){return root;}
 Body* insertRulesInStructure(const std::string& queryname, int ng);
 void buildAndFindBodies(ItemSet& body);
 void buildAndFindRules(ItemSet& body,ItemSet& heads);
 void adjustSupp(MIndexIterator* b2,MIndexIterator* h2);
};

}//namespace

#endif

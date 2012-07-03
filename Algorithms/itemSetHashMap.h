#ifndef ITEMSETHASHMAP_H
#define ITEMSETHASHMAP_H

#include <ext/hash_map>
#include "utils.h"

using namespace std;
using namespace __gnu_cxx;

struct eqstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};



template <class NODETYPE>
class itemSetHashMap {
 public:
  itemSetHashMap();
  void print();
  void add(const char* elem,NODETYPE item);
  bool findTies(String ancestor, NODETYPE item,int lev);
  hash_map<const char*,NODETYPE, hash <const char* >, eqstr > getHash() {return hashLevel;}

 private:
  hash_map<const char*, NODETYPE, hash <const char* >, eqstr > hashLevel;
};


template< class NODETYPE >
itemSetHashMap< NODETYPE >::itemSetHashMap()
 {
   //cout<<"costruttore HashMap"<<endl;
 }

template< class NODETYPE >
void itemSetHashMap< NODETYPE >::add(const char* elem, NODETYPE item)
 {
   //hashLevel.insert(make_pair(elem,item));
   //cout<<"Nella add "<<elem<<" item "<<item<<endl;

   hashLevel[elem]=item;
 }


template< class NODETYPE >
bool itemSetHashMap< NODETYPE >::findTies(String ancestor, NODETYPE item,int lev)
 {
  //cout<<"findTies"<<endl;
  //cout<<"ancestor "<<ancestor<<" item "<<item<<endl;

  typename hash_map<const char*, NODETYPE, hash <const char* >, eqstr >::iterator k;
  int i;
  String us;
  boolean ok;

 // print();
  i=1;
  ok=true;
  while ( (i<lev) && (ok) )
   {
     us=underString(ancestor,i);
     i++;
     us=us+item;

     const char* like=(us).c_str();
   //  cout<<"undersTring"<<like<<"--"<<endl;
     k=hashLevel.find(like);
     //cout<<"k "<<k->first<<endl;
     if (k==hashLevel.end())  ok=false;

   }

  return ok;
 }


template< class NODETYPE >
void itemSetHashMap< NODETYPE >::print()
 {
  cout<<"PRINT : Hash Map Size"<<hashLevel.size()<<endl;
  typename hash_map<const char*, NODETYPE, hash <const char* >, eqstr >::iterator i;
  i=hashLevel.begin();


  for (int j=0;j<hashLevel.size(); j++,i++)
     {
          cout<<"elemento Hash"<<i->first<<endl;
    }
  cout<<"Fine PRINT hash"<<endl;
 }


#endif

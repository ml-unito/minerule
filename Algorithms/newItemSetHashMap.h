#ifndef ITEMSETHASHMAP_H
#define ITEMSETHASHMAP_H

//#include <ext/hash_set>
#include <unordered_set>
#include <vector>
#include "utils.h"
#include <iterator>

#define hash_set unordered_set

using namespace std;
using namespace __gnu_cxx;

template<class NODETYPE>
class HashingHelper;


template <class NODETYPE>
class newItemSetHashMap {
 public:

  typedef vector<NODETYPE> HASHTYPE;

  newItemSetHashMap();
  void print();
  void add(const HASHTYPE& item);
  bool findTies(const HASHTYPE&, const NODETYPE& item,unsigned int lev);

    hash_set<HASHTYPE,
    HashingHelper<NODETYPE>, 
    HashingHelper<NODETYPE> > 
    getHash() {return hashLevel;}

 private:
  hash_set<HASHTYPE, HashingHelper<NODETYPE>, HashingHelper<NODETYPE> > hashLevel;
};

template<class NODETYPE>
class HashingHelper {
 public:
  typedef typename newItemSetHashMap<NODETYPE>::HASHTYPE HASHTYPE;

  size_t operator()(const HASHTYPE&  h1) const {
    std::string s1="";
    
    hash<const char*> stringHash;
    typename HASHTYPE::const_iterator it = h1.begin();
    
    for(;it!=h1.end();it++) {
      s1+=(*it).asString();
  }

  return stringHash(s1.c_str());
  }

  bool operator()(const HASHTYPE& h1, const HASHTYPE& h2) const {
    typename HASHTYPE::const_iterator it1=
      h1.begin();
    typename HASHTYPE::const_iterator it2=
      h2.begin();

    for(; it1!=h1.end() &&
	  it2!=h2.end() &&
	  *it1 == *it2; it1++, it2++ );

    return (it1==h1.end() && it2==h2.end());
  }

};


template< class NODETYPE >
newItemSetHashMap< NODETYPE >::newItemSetHashMap()
 {
   //cout<<"costruttore HashMap"<<endl;
 }

template< class NODETYPE >
void newItemSetHashMap< NODETYPE >::add(const HASHTYPE& item)
 {
   //hashLevel.insert(make_pair(elem,item));
   //cout<<"Nella add "<<elem<<" item "<<item<<endl;

   hashLevel.insert(item);
 }

template< class NODETYPE >
bool newItemSetHashMap< NODETYPE >::findTies(
						const HASHTYPE& ancestor, 
						const NODETYPE& item,
						unsigned int lev)
{
  //#define DEBUG_FINDTIES
  //cout<<"findTies"<<endl;
  //cout<<"ancestor "<<ancestor<<" item "<<item<<endl;
#ifdef DEBUG_FINDTIES
  cout << "FIND TIES ancestor:";
  copy( ancestor.begin(), ancestor.end(), ostream_iterator<NODETYPE>(cout,"!"));
  cout << ":end ancestor" << endl 
       << "FIND TIES item:" << item << ":end item" << endl;
  cout << "FIND TIES lev:" << lev << ":end lev" << endl;
  cout << "FIND TIES hash_set:";
  typename hash_set<HASHTYPE, HashingHelper<NODETYPE>, HashingHelper<NODETYPE> >::const_iterator 
    it = hashLevel.begin();
  while( it!=hashLevel.end() ) {
    cout << "HT(" << endl;
    copy( it->begin(),it->end(), ostream_iterator<NODETYPE>(cout,"@"));
    cout << ") " <<endl;
    it++;
  }
  cout << ":end hash_set" << endl;
#endif
  
  typename hash_set<HASHTYPE, 
    HashingHelper<NODETYPE>, 
    HashingHelper<NODETYPE> >::iterator k;
  

  bool ok;
  
  /*  if( ancestor.size() != lev-1 ) {
    cout << "lev-1:" << lev-1 << " " << ancestor.size() <<endl;
    typename HASHTYPE::const_iterator it = ancestor.begin();
    cout << "ancestor:";
    while( it!=ancestor.end() ) {
      cout << *it << " ";
      it++;
    }
    cout << endl;
    }*/
  assert( ancestor.size() == lev-1 );
  // print();
  
  
  ok=true;
  unsigned int i=0;
  while ( i<lev-1 && (ok) ) {
      HASHTYPE subPath = ancestor;
      typename HASHTYPE::iterator it = subPath.begin();
      // it++; // we skip the first, empty, elem
      for(unsigned int j=0;j<i;j++)
	it++;

      subPath.erase(it);
      subPath.push_back(item);
#ifdef DEBUG_FINDTIES
      cout << "FIND TIES subPath:";
      copy( subPath.begin(), subPath.end(), ostream_iterator<NODETYPE>(cout,"!"));
      cout << ":end subPath" << endl;
#endif
      k=hashLevel.find(subPath);
      //cout<<"k "<<k->first<<endl;
      if (k==hashLevel.end())  
	ok=false;
      
      i++;
    }
#ifdef DEBUG_FINDTIES
  cout << "FIND-TIES result:" << ok << endl;
#endif
  return ok;
}


template< class NODETYPE >
void newItemSetHashMap< NODETYPE >::print()
 {
  cout<<"PRINT : Hash Map Size"<<hashLevel.size()<<endl;
  typename hash_set<HASHTYPE, 
    HashingHelper<NODETYPE>, 
    HashingHelper<NODETYPE> >::iterator i;
  
  i=hashLevel.begin();
  
  for(; i!=hashLevel.end(); i++) {
    cout << "Elemento Hash:";
    typename HASHTYPE::const_iterator it = i->begin();
    for(; it!=i->end(); it++ ) {
      cout << (*it) << " ";
    }

    cout << endl;
  }
  


  /*
  for (int j=0;j<hashLevel.size(); j++,i++)
     {
          cout<<"elemento Hash"<<i->first<<endl;
    }
  */
  cout<<"Fine PRINT hash"<<endl;
 }




#endif

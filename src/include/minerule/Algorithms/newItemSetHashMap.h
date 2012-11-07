#ifndef ITEMSETHASHMAP_H
#define ITEMSETHASHMAP_H

//#include <ext/hash_set>
// #include <unordered_set>
#include <tr1/unordered_set>
#include <vector>
#include "utils.h"
#include <iterator>

#define hash_set std::tr1::unordered_set


using namespace __gnu_cxx;

template<class NODETYPE>
class HashingHelper;


template <class NODETYPE>
class newItemSetHashMap {
 public:

  typedef std::vector<NODETYPE> HASHTYPE;

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
	  // unimplemented
	  throw	new std::exception();
	  return 0;
  //   std::string s1="";
  //   
  //   hash_map<const char*> stringHash;
  //   typename HASHTYPE::const_iterator it = h1.begin();
  //   
  //   for(;it!=h1.end();it++) {
  //     s1+=(*it).asString();
  // }
  // 
  // return stringHash[s1.c_str()];
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
   //cout<<"costruttore HashMap"<<std::endl;
 }

template< class NODETYPE >
void newItemSetHashMap< NODETYPE >::add(const HASHTYPE& item)
 {
   //hashLevel.insert(make_pair(elem,item));
   //cout<<"Nella add "<<elem<<" item "<<item<<std::endl;

   hashLevel.insert(item);
 }

template< class NODETYPE >
bool newItemSetHashMap< NODETYPE >::findTies(
						const HASHTYPE& ancestor, 
						const NODETYPE& item,
						unsigned int lev)
{
  //#define DEBUG_FINDTIES
  //cout<<"findTies"<<std::endl;
  //cout<<"ancestor "<<ancestor<<" item "<<item<<std::endl;
#ifdef DEBUG_FINDTIES
  std::cout << "FIND TIES ancestor:";
  copy( ancestor.begin(), ancestor.end(), std::ostream_iterator<NODETYPE>(cout,"!"));
  std::cout << ":end ancestor" << std::endl 
       << "FIND TIES item:" << item << ":end item" << std::endl;
  std::cout << "FIND TIES lev:" << lev << ":end lev" << std::endl;
  std::cout << "FIND TIES hash_set:";
  typename hash_set<HASHTYPE, HashingHelper<NODETYPE>, HashingHelper<NODETYPE> >::const_iterator 
    it = hashLevel.begin();
  while( it!=hashLevel.end() ) {
    std::cout << "HT(" << std::endl;
    copy( it->begin(),it->end(), std::ostream_iterator<NODETYPE>(cout,"@"));
    std::cout << ") " <<std::endl;
    it++;
  }
  std::cout << ":end hash_set" << std::endl;
#endif
  
  typename hash_set<HASHTYPE, 
    HashingHelper<NODETYPE>, 
    HashingHelper<NODETYPE> >::iterator k;
  

  bool ok;
  
  /*  if( ancestor.size() != lev-1 ) {
    std::cout << "lev-1:" << lev-1 << " " << ancestor.size() <<std::endl;
    typename HASHTYPE::const_iterator it = ancestor.begin();
    std::cout << "ancestor:";
    while( it!=ancestor.end() ) {
      std::cout << *it << " ";
      it++;
    }
    std::cout << std::endl;
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
      std::cout << "FIND TIES subPath:";
      copy( subPath.begin(), subPath.end(), std::ostream_iterator<NODETYPE>(cout,"!"));
      std::cout << ":end subPath" << std::endl;
#endif
      k=hashLevel.find(subPath);
      //cout<<"k "<<k->first<<std::endl;
      if (k==hashLevel.end())  
	ok=false;
      
      i++;
    }
#ifdef DEBUG_FINDTIES
  std::cout << "FIND-TIES result:" << ok << std::endl;
#endif
  return ok;
}


template< class NODETYPE >
void newItemSetHashMap< NODETYPE >::print()
 {
  std::cout<<"PRINT : Hash Map Size"<<hashLevel.size()<<std::endl;
  typename hash_set<HASHTYPE, 
    HashingHelper<NODETYPE>, 
    HashingHelper<NODETYPE> >::iterator i;
  
  i=hashLevel.begin();
  
  for(; i!=hashLevel.end(); i++) {
    std::cout << "Elemento Hash:";
    typename HASHTYPE::const_iterator it = i->begin();
    for(; it!=i->end(); it++ ) {
      std::cout << (*it) << " ";
    }

    std::cout << std::endl;
  }
  


  /*
  for (int j=0;j<hashLevel.size(); j++,i++)
     {
          std::cout<<"elemento Hash"<<i->first<<std::endl;
    }
  */
  std::cout<<"Fine PRINT hash"<<std::endl;
 }




#endif

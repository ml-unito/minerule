#ifndef GIDCIDLIST_H
#define GIDCIDLIST_H


#include <map>
#include <list>
#include <algorithm>
#include <cassert>

//#include "utils.h"
using namespace std;

template <class NODETYPELIST>
class gidCidList {
 public:
  typedef map<NODETYPELIST, map <NODETYPELIST,int /*, NODETYPELIST*/>/*, NODETYPELIST*/ > MapType;

  gidCidList() {}
  void insertValue(NODETYPELIST gid, NODETYPELIST cid);
  void print();
  string getListToDesign();
  double numDistinctGID() const;
  bool existGidCid(NODETYPELIST gid, NODETYPELIST cid);
  MapType& getMap() {return lista;}
  void set_intersection( gidCidList <NODETYPELIST>& t1, gidCidList<NODETYPELIST>& newLst);
  void set_gid_intersection(gidCidList<NODETYPELIST>& list);
  bool isBigger(double nSup);
  int Csize() {return lista.size();}

  void
    clear() {
    lista.clear();
  }
 private:
  MapType lista;
};

template< class NODETYPELIST >
bool gidCidList< NODETYPELIST >::isBigger(double nSup)
 {
   return (lista.size() >=nSup);
 }


/**
 * Intersection of this->getMap() and list.getMap() taking in account the gid only.
 * algorithm: standard intersection algorithm between ordered sets.
 */ 

template< class NODETYPELIST >
void 
gidCidList< NODETYPELIST >::set_gid_intersection(gidCidList<NODETYPELIST>& list) {
  MapType newGidList;
  std::set_intersection(getMap().begin(), getMap().end(),
		   list.getMap().begin(), list.getMap().end(),
		   inserter(newGidList,newGidList.begin()));
  getMap() = newGidList;  
/*
  typename MapType::iterator selfIt = getMap().begin();
  typename MapType::iterator listIt = list.getMap().begin();
  vector<NODETYPELIST> elemsToBeErased;

  while( selfIt!=getMap().end() && listIt!=list.getMap().end() ) {
    if( selfIt->first == listIt->first ) {
      selfIt++;
      listIt++;
    } else {
      const NODETYPELIST& selfElem=selfIt->first;
      const NODETYPELIST& listElem=listIt->first;

      if( selfElem < listElem ) { 
	//  selfIt->first < listIt->first
	elemsToBeErased.push_back(selfElem);
	
	selfIt++;
      } else
	listIt++;
    }
  }
  
  if( selfIt!=getMap().end() ) {
    // selfIt points to the first uncommon element of this->getMap()
    // all elements from this point to the end of this->getMap() do not
    // belong to the intersection
    getMap().erase( selfIt, getMap().end() );
  }

  typename vector<NODETYPELIST>::iterator it;
  for(it=elemsToBeErased.begin(); it!=elemsToBeErased.end(); it++) {
    getMap().erase(*it);
  }
*/
}


/**
 * NOTE: il fatto che il risultato di questa funzione sia una gidCidList e'
 * molto inefficiente! bisognerebbe passare la gidCidList per riferimento...
 * (l'inefficienza e' dovuta al fatto che viene effettuata una copia della
 *  gicCidList...)
 */
template< class NODETYPELIST >
void
gidCidList< NODETYPELIST >::set_intersection(gidCidList <NODETYPELIST>& t1,
					     gidCidList <NODETYPELIST>& newLst)
 {
   typename MapType::const_iterator i;
   typename map <NODETYPELIST,int/*,NODETYPELIST*/>::const_iterator j;

   //cout<<"gidcidlista padre"<<endl;
   //t1.print();
   //cout<<"gidcidlista figlio"<<endl;
   //print();
   const MapType& t1map=t1.getMap();

   for (i=t1map.begin();i!=t1map.end();i++)
    {
        for (j=i->second.begin();j!=i->second.end();j++)
            if (existGidCid(i->first,j->first))
               newLst.insertValue(i->first,j->first);
     }
  // cout<<"fine set_inter"<<endl;
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

   assert( newLst.numDistinctGID()<=min(t1.numDistinctGID(),numDistinctGID()));
 }

template< class NODETYPELIST >
bool gidCidList< NODETYPELIST >::existGidCid(NODETYPELIST gid,NODETYPELIST cid)
 {
  typename MapType::iterator i=lista.find(gid);;
  map<NODETYPELIST,int/*, NODETYPELIST*/> temp;
  //list < NODETYPELIST> tList;

  if (i==lista.end())
   {
    return false;
   }
  else
   {
     if (i->second.find(cid)==i->second.end())
         return false;
     else
         return true;
   }

 }


template< class NODETYPELIST >
void gidCidList< NODETYPELIST >::insertValue(NODETYPELIST gid,NODETYPELIST cid)
 {
  typename MapType::iterator i;
  map<NODETYPELIST,int/*,NODETYPELIST*/> temp;
  //list < NODETYPELIST> tList;

 // cout<<"inserTVALUE gid "<<gid<<" cid "<<cid<<endl;

  i=lista.find(gid);

 if (i==lista.end())
   {
    // cout<<"l'elemento della map non esiste"<<endl;
     temp[cid]=1;
     lista[gid]=temp;
   }
  else
   {
     if (i->second.find(cid)==i->second.end())
         i->second.insert(make_pair(cid,1));
     //else cout<<"l'elemento esiste già"<<endl;

   }

 }

template< class NODETYPELIST >
double gidCidList< NODETYPELIST >::numDistinctGID() const
 {
   return lista.size();
 }


template< class NODETYPELIST >
void gidCidList< NODETYPELIST >::print()
 {
  typename MapType::iterator i;
  typename map<NODETYPELIST,int/*,NODETYPELIST*/ >::iterator j;

  //cout<<"itno ";
  for( i=lista.begin( ) ; i != lista.end( ) ; i++ )
     {

       for (j=i->second.begin(); j!=i->second.end();j++)
           cout<<i->first<<","<<j->first<<"   ";

     }
   cout<<endl;
 }


template< class NODETYPELIST >
string gidCidList< NODETYPELIST >::getListToDesign()
 {
  typename MapType::iterator i;
  typename map<NODETYPELIST,int/*, NODETYPELIST*/>::iterator j;
  string result;
  // char numgid[20],numcid[20];

  //cout<<"itno ";
  result="";
  for( i=lista.begin( ) ; i != lista.end( ) ; i++ )
     {
       // sprintf(numgid,"%d",i->first);
       for (j=i->second.begin(); j!=i->second.end();j++)
         {
	   //  sprintf(numcid,"%d",j->first);
           result=result+" "+
	     i->first.c_str()+";"+
	     j->first.c_str()+" ";
          //result="pipo";
          // cout<<i->first<<","<<j->first<<"   ";
         }

     }
   //cout<<endl;
  return result;
 }

#endif

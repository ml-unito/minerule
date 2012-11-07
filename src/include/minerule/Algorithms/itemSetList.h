#ifndef ITEMSETLIST_H
#define ITEMSETLIST_H


#include <list>
#include <vector>
#include "utils.h"



template< class NODETYPE > class itemSet;  // dichiarazione succ.

template <class NODETYPE>
class itemSetList {
 public:
 itemSetList();
 std::list<itemSet<NODETYPE>* >* getList(int pos) {return vect[pos];}
 void init();
 std::vector<std::list<itemSet<NODETYPE>* >* > vect;

 void print();
 void printAddrItemSet();
 void removeAll();
 void remove(int lev,itemSet<NODETYPE>* p);
};


template< class NODETYPE >
itemSetList< NODETYPE >::itemSetList()
 {
  //cout<<"costruttore pList"<<std::endl;
 }



template< class NODETYPE >
void itemSetList< NODETYPE >::init()
 {
  //cout<<"Init"<<std::endl;
  std::list<itemSet<NODETYPE>* >* temp=NULL;

  vect.push_back(temp);
  vect.push_back(temp);
  //cout<<"size vect"<<vect.size()<<std::endl;

 }


template< class NODETYPE >
void itemSetList< NODETYPE >::print()
 {
  std::cout<<"PRINT Lista ItemSet"<<std::endl;
  //list<itemSet<NODETYPE>* >* temp;
  typename std::vector<std::list<itemSet<NODETYPE>* >* >::iterator i;

   for ( i=vect.begin(); i!=vect.end(); i++)
     {
          std::cout<<"*i -"<<*i<<std::endl;
     }
    std::cout<<"Fine PRINT itemSet"<<std::endl;
 }


template< class NODETYPE >
void itemSetList< NODETYPE >::printAddrItemSet()
 {
  std::cout<<"PRINT Lista itemset"<<std::endl;
  std::list<itemSet<NODETYPE>* >* temp;
  typename std::vector<std::list<itemSet<NODETYPE>* >* >::iterator i;
  typename std::list<itemSet<NODETYPE>* >::iterator j;
  int lev;

  lev=0;
   for ( i=vect.begin(); i!=vect.end(); i++)
     {
          std::cout<<lev<<") Indirizzo Lista "<<*i<<std::endl;
          lev++;
          if ((*i)!=NULL)
          {
           for ( j=(*i)->begin(); j!=(*i)->end(); j++)
            {
          	std::cout<<"\t"<<"ISet : "<<*j<<std::endl;
		//cout<<"FOOT"<<*i<<std::endl;

            }
          }
     }
    std::cout<<"Fine PRINT Lista itemset"<<std::endl;
 }

template< class NODETYPE >
void itemSetList< NODETYPE >::removeAll()
 {
  //cout<<"Remove All"<<std::endl;
   //  std::list<itemSet<NODETYPE>* >* temp;
  typename std::vector<std::list<itemSet<NODETYPE>* >* >::iterator i;
  //  std::cout<<"dentro remove ALL"<<std::endl;
   for ( i=vect.begin(); i!=vect.end(); i++)
     {
       //          std::cout<<"*i -"<<*i<<std::endl;
          delete (*i);
     }
     std::cout<<"a"<<std::endl;
   vect.clear();
   //  std::cout<<"finitoremove"<<std::endl;
 }

template< class NODETYPE >
void itemSetList< NODETYPE >::remove(int lev,itemSet<NODETYPE>* p)
 {
  std::cout<<"Remove P in level"<<lev<<std::endl;
  std::cout<<"Address puntatore "<<p<<std::endl;
  std::list<itemSet<NODETYPE>* >* temp;
  typename std::list<itemSet<NODETYPE>* >::iterator i;
  bool trovato;

  temp=getList(lev);
  i=temp->begin();
  trovato=false;

  while ( (i!=temp->end()) && (!trovato))
     {
        std::cout<<"*i -"<<*i<<" puntatore "<<p<<std::endl;
        if ((*i)==p)
        {
          std::cout<<"cancello"<<std::endl;
          //temp->erase(i);
          trovato=true;
        }
      i++;
     }

 }


#endif

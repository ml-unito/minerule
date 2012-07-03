#ifndef ITEMSETLIST_H
#define ITEMSETLIST_H


#include <list>
#include <vector>
#include "utils.h"

using namespace std;

template< class NODETYPE > class itemSet;  // dichiarazione succ.

template <class NODETYPE>
class itemSetList {
 public:
 itemSetList();
 list<itemSet<NODETYPE>* >* getList(int pos) {return vect[pos];}
 void init();
 vector<list<itemSet<NODETYPE>* >* > vect;

 void print();
 void printAddrItemSet();
 void removeAll();
 void remove(int lev,itemSet<NODETYPE>* p);
};


template< class NODETYPE >
itemSetList< NODETYPE >::itemSetList()
 {
  //cout<<"costruttore pList"<<endl;
 }



template< class NODETYPE >
void itemSetList< NODETYPE >::init()
 {
  //cout<<"Init"<<endl;
  list<itemSet<NODETYPE>* >* temp=NULL;

  vect.push_back(temp);
  vect.push_back(temp);
  //cout<<"size vect"<<vect.size()<<endl;

 }


template< class NODETYPE >
void itemSetList< NODETYPE >::print()
 {
  cout<<"PRINT Lista ItemSet"<<endl;
  //list<itemSet<NODETYPE>* >* temp;
  typename vector<list<itemSet<NODETYPE>* >* >::iterator i;

   for ( i=vect.begin(); i!=vect.end(); i++)
     {
          cout<<"*i -"<<*i<<endl;
     }
    cout<<"Fine PRINT itemSet"<<endl;
 }


template< class NODETYPE >
void itemSetList< NODETYPE >::printAddrItemSet()
 {
  cout<<"PRINT Lista itemset"<<endl;
  list<itemSet<NODETYPE>* >* temp;
  typename vector<list<itemSet<NODETYPE>* >* >::iterator i;
  typename list<itemSet<NODETYPE>* >::iterator j;
  int lev;

  lev=0;
   for ( i=vect.begin(); i!=vect.end(); i++)
     {
          cout<<lev<<") Indirizzo Lista "<<*i<<endl;
          lev++;
          if ((*i)!=NULL)
          {
           for ( j=(*i)->begin(); j!=(*i)->end(); j++)
            {
          	cout<<"\t"<<"ISet : "<<*j<<endl;
		//cout<<"FOOT"<<*i<<endl;

            }
          }
     }
    cout<<"Fine PRINT Lista itemset"<<endl;
 }

template< class NODETYPE >
void itemSetList< NODETYPE >::removeAll()
 {
  //cout<<"Remove All"<<endl;
   //  list<itemSet<NODETYPE>* >* temp;
  typename vector<list<itemSet<NODETYPE>* >* >::iterator i;
  //  cout<<"dentro remove ALL"<<endl;
   for ( i=vect.begin(); i!=vect.end(); i++)
     {
       //          cout<<"*i -"<<*i<<endl;
          delete (*i);
     }
     cout<<"a"<<endl;
   vect.clear();
   //  cout<<"finitoremove"<<endl;
 }

template< class NODETYPE >
void itemSetList< NODETYPE >::remove(int lev,itemSet<NODETYPE>* p)
 {
  cout<<"Remove P in level"<<lev<<endl;
  cout<<"Address puntatore "<<p<<endl;
  list<itemSet<NODETYPE>* >* temp;
  typename list<itemSet<NODETYPE>* >::iterator i;
  bool trovato;

  temp=getList(lev);
  i=temp->begin();
  trovato=false;

  while ( (i!=temp->end()) && (!trovato))
     {
        cout<<"*i -"<<*i<<" puntatore "<<p<<endl;
        if ((*i)==p)
        {
          cout<<"cancello"<<endl;
          //temp->erase(i);
          trovato=true;
        }
      i++;
     }

 }


#endif

#ifndef ITEMSETLISTCLUSTER_H
#define ITEMSETLISTCLUSTER_H


#include <list>
#include <vector>
#include "utils.h"

using namespace std;

template< class NODETYPE > class itemSetCluster;  // dichiarazione succ.

template <class NODETYPE>
class itemSetListCluster {
 public:
 itemSetListCluster();
 list<itemSetCluster<NODETYPE>* >* getList(int pos) {return vect[pos];}
 void init();

 vector<list<itemSetCluster<NODETYPE>* >* > vect;
 //list<itemSet<NODETYPE>* > operator[ ] (int which);

 void print();
 void printAddrItemSet();
 void removeAll();

};


template< class NODETYPE >
itemSetListCluster< NODETYPE >::itemSetListCluster()
 {
  //cout<<"costruttore pList"<<endl;
 }



template< class NODETYPE >
void itemSetListCluster< NODETYPE >::init()
 {
  
  list<itemSetCluster<NODETYPE>* >* temp=NULL;

  vect.push_back(temp);
  vect.push_back(temp);


 }


template< class NODETYPE >
void itemSetListCluster< NODETYPE >::print()
 {
  cout<<"PRINT Lista ItemSet"<<endl;
  list<itemSetCluster<NODETYPE>* >* temp;
  typename vector<list<itemSetCluster<NODETYPE>* >* >::iterator i;

   for ( i=vect.begin(); i!=vect.end(); i++)
     {
          cout<<"*i -"<<*i<<endl;
     }
    cout<<"Fine PRINT itemSet"<<endl;
 }



template< class NODETYPE >
void itemSetListCluster< NODETYPE >::printAddrItemSet()
 {
  cout<<"PRINT Lista itemset"<<endl;
  list<itemSetCluster<NODETYPE>* >* temp;
  typename vector<list<itemSetCluster<NODETYPE>* >* >::iterator i;
  typename list<itemSetCluster<NODETYPE>* >::iterator j;
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
          	cout<<"\t"<<"ItemSetCluster : "<<*j<<endl;
		//cout<<"FOOT"<<*i<<endl;

            }
          }
     }
    cout<<"Fine PRINT Lista itemset"<<endl;
 }

template< class NODETYPE >
void itemSetListCluster< NODETYPE >::removeAll() {
  //cout<<"Remove All"<<endl;
  //  list<itemSetCluster<NODETYPE>* >* temp;
  typename vector<list<itemSetCluster<NODETYPE>* >* >::iterator i;
  
  for ( i=vect.begin(); i!=vect.end(); i++)  {
       //      cout<<"*i -"<<*i<<endl;
       delete (*i);
     }
  vect.clear();
}

#endif

#ifndef ITEMSETLISTCLUSTER_H
#define ITEMSETLISTCLUSTER_H


#include <list>
#include <vector>
#include "utils.h"



template< class NODETYPE > class itemSetCluster;  // dichiarazione succ.

template <class NODETYPE>
class itemSetListCluster {
 public:
 itemSetListCluster();
 std::list<itemSetCluster<NODETYPE>* >* getList(int pos) {return vect[pos];}
 void init();

 std::vector<std::list<itemSetCluster<NODETYPE>* >* > vect;
 //list<itemSet<NODETYPE>* > operator[ ] (int which);

 void print();
 void printAddrItemSet();
 void removeAll();

};


template< class NODETYPE >
itemSetListCluster< NODETYPE >::itemSetListCluster()
 {
  //cout<<"costruttore pList"<<std::endl;
 }



template< class NODETYPE >
void itemSetListCluster< NODETYPE >::init()
 {
  
  std::list<itemSetCluster<NODETYPE>* >* temp=NULL;

  vect.push_back(temp);
  vect.push_back(temp);


 }


template< class NODETYPE >
void itemSetListCluster< NODETYPE >::print()
 {
  std::cout<<"PRINT Lista ItemSet"<<std::endl;
  std::list<itemSetCluster<NODETYPE>* >* temp;
  typename std::vector<std::list<itemSetCluster<NODETYPE>* >* >::iterator i;

   for ( i=vect.begin(); i!=vect.end(); i++)
     {
          std::cout<<"*i -"<<*i<<std::endl;
     }
    std::cout<<"Fine PRINT itemSet"<<std::endl;
 }



template< class NODETYPE >
void itemSetListCluster< NODETYPE >::printAddrItemSet()
 {
  std::cout<<"PRINT Lista itemset"<<std::endl;
  std::list<itemSetCluster<NODETYPE>* >* temp;
  typename std::vector<std::list<itemSetCluster<NODETYPE>* >* >::iterator i;
  typename std::list<itemSetCluster<NODETYPE>* >::iterator j;
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
          	std::cout<<"\t"<<"ItemSetCluster : "<<*j<<std::endl;
		//cout<<"FOOT"<<*i<<std::endl;

            }
          }
     }
    std::cout<<"Fine PRINT Lista itemset"<<std::endl;
 }

template< class NODETYPE >
void itemSetListCluster< NODETYPE >::removeAll() {
  //cout<<"Remove All"<<std::endl;
  //  std::list<itemSetCluster<NODETYPE>* >* temp;
  typename std::vector<std::list<itemSetCluster<NODETYPE>* >* >::iterator i;
  
  for ( i=vect.begin(); i!=vect.end(); i++)  {
       //      std::cout<<"*i -"<<*i<<std::endl;
       delete (*i);
     }
  vect.clear();
}

#endif

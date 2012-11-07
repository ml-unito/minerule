#ifndef ITEMSETENTRYCLUSTER_H
#define ITEMSETENTRYCLUSTER_H

#include "gidCidList.h"
#include "itemSetListCluster.h"




template< class NODETYPE > class itemSetCluster;  // dichiarazione succ.
template< class NODETYPE > class itemSetListCluster;


template< class NODETYPE >
class itemSetEntryCluster {
  public:
   itemSetEntryCluster();
   itemSetEntryCluster(NODETYPE value);
   const NODETYPE& getData() { return data; }
   gidCidList <int>& getGidCidList()   { return liste; }
   void setGidList(const gidCidList <int>& gidcid)  { liste=gidcid; }

   itemSetCluster<NODETYPE>* getItemSet()  { return myItemSetCluster; }
   itemSetCluster<NODETYPE>* getHead()  { return myItemSetHead; }
   void setData(const NODETYPE& val)  { data=val; }
   itemSetListCluster <NODETYPE>& getListPtrHead() { return listHead; }
   void setListPtrHead( const itemSetListCluster <NODETYPE>& nPtrH )  { listHead=nPtrH; }
   void setItemSetChild(itemSetCluster< NODETYPE >* pNew)   { myItemSetCluster=pNew; }
   void setHead(itemSetCluster < NODETYPE >* iscH) {myItemSetHead=iscH;}
   void setCountGid(double nGid) {countGid=nGid;}
   double getCountGid() {
     return countGid;
   }

   void releaseHead() {
     delete myItemSetHead;
     myItemSetHead = NULL;
   }
   void releaseItemSet() {
     delete myItemSetCluster;
     myItemSetCluster = NULL;
   }

   private:
   NODETYPE data;

   itemSetCluster< NODETYPE >* myItemSetCluster;
   itemSetCluster< NODETYPE >* myItemSetHead;

   itemSetListCluster <NODETYPE> listHead;

   gidCidList <int> liste;

   double countGid;
};



/* IMPLEMENTAZIONE DEI METODI DI */
template< class NODETYPE >
itemSetEntryCluster< NODETYPE >::itemSetEntryCluster()
 {
 // std::cout<<"costruttore pEntry"<<std::endl;

  myItemSetCluster=NULL;
  myItemSetHead=NULL;
  countGid=0;

 }


// Costruttore. Imposto i valori degli elementi principali.
template< class NODETYPE >
itemSetEntryCluster< NODETYPE >::itemSetEntryCluster(NODETYPE value)
 {
 // std::cout<<"costruttore pEntry"<<std::endl;
  data=value;
  myItemSetHead=NULL;
  myItemSetCluster=NULL;
  countGid=0;
 }


#endif

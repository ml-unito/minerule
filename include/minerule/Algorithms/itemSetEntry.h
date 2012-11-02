// ItemSetEntry.H


#ifndef _ITEMSETENTRY_H
#define _ITEMSETENTRY_H

#include <map>
#include <list>
#include <set>


using namespace std;

template< class NODETYPE > class itemSet;  // dichiarazione succ.

template< class NODETYPE >
class itemSetEntry {
public:
   // Costruttore
   itemSetEntry();
   itemSetEntry(NODETYPE value);

   // Ritorna il valore della entry
   NODETYPE getData() { return data; }
  // void setGidList(int gid) { gidList=gid; }
   itemSet<NODETYPE>* getItemSet()  { return myItemSet; }
   void setData(NODETYPE val)  { data=val; }
   void setGidList(int gid)  { gidList.insert(gid); }
   void setGidList(const set<int>& gidL)  { gidList=gidL; }
   set<int>& getGidList()  { return gidList; }
   void setCountGid(double cGid) {
     countGid=cGid;
   }
   double getCountGid() const {
#ifdef MRUSERWARNING
#warning da mettere a posto...
#endif
     //     return countGid;
     return gidList.size();
   }
   void setItemSetChild(itemSet< NODETYPE >* pNew)   { myItemSet=pNew; }
   //void setTotPart()  { numPartOK++; }
   //double getTotPart()  {return numPartOK; }
   void gidListRemove() { 
     //#warning commentata la remove
     // return;
     gidList.erase(gidList.begin(),gidList.end()); }

   int numDistinctGID() {
     return gidList.size();
   }

   void releaseItemSet() {
     delete myItemSet;
     myItemSet = NULL;
   }
private:
   // livello che indica il k-itemset
   NODETYPE data;

   //
   itemSet< NODETYPE >* myItemSet;

   // CONTATORE TOTALE
   double countGid;

   // Lista delle gidlist
   set<int> gidList;

};


/* IMPLEMENTAZIONE DEI METODI DI PARTITIONENTRY.H */
template< class NODETYPE >
itemSetEntry< NODETYPE >::itemSetEntry()
 {
 // cout<<"costruttore pEntry"<<endl;
  countGid=0;
  //numPartOK=0;
  myItemSet=NULL;
 }


// Costruttore. Imposto i valori degli elementi principali.
template< class NODETYPE >
itemSetEntry< NODETYPE >::itemSetEntry(NODETYPE value)
 {
 // cout<<"costruttore pEntry"<<endl;
  data=value;
  myItemSet=NULL;
 }


#endif

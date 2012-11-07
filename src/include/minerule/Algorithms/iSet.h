#ifndef ISET_H
#define ISET_H

#include "itemSetList.h"
#include "itemSetHashMap.h"



template< class NODETYPE >
class iSet {
 public:
 virtual void setItemSet(NODETYPE value,int gidList)=0;
 virtual void gen_Large_ItemSet_Base(double,double)=0;
 virtual boolean gen_Large_ItemSet(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev)=0;
 virtual void printToDesign(int isPart)=0;
 virtual void save_Large_ItemSet(int lev,int isPart)=0;
 virtual void mergeItemSet(int isPart,itemSetList<NODETYPE>& ptrList)=0;
 virtual void setItemSetIfExist(NODETYPE value,int gidList)=0;
 virtual void checkSupportBaseDef(double,double,  itemSetHashMap<NODETYPE>&,itemSetList<NODETYPE>& )=0;
 virtual boolean gen_final_count(itemSetList<NODETYPE>& ,double,double,int,itemSetHashMap<NODETYPE>&)=0;
 virtual void extractRule(String regola,int lev)=0;
 virtual void printItemSetRecursive(int lev)=0;
};

#endif


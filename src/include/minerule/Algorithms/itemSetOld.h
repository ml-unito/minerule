/* ITEMSET.H */

#ifndef ITEMSET_H
#define ITEMSET_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip.h>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>


#include "iSet.h"

#include "itemSetEntry.h"
#include "itemSetList.h"
#include "itemSetHashMap.h"

#include "utils.h"




typedef int boolean;



template< class NODETYPE > class itemSetEntry;

template< class NODETYPE >
class itemSet {
public:
   // Costruttore
   itemSet() {   numLevel=1; myAncestor=""; }
   itemSet(int lev) { numLevel=lev; myAncestor="";}

   void setLevel(int lev) { numLevel=lev;}

   int getLevel()  { return numLevel;}
   void setAncestor(String anc) {myAncestor=anc;}
   String getAncestor() {return myAncestor;}

   void setRefList(typename std::list<itemSet<NODETYPE>* >::iterator riferimento) {refList=riferimento;}
   typename std::list<itemSet<NODETYPE>* >::iterator getRefList()  {return refList;}
   void setItemSet(NODETYPE value,NODETYPE gidList);
   void setItemSetIfExist(NODETYPE value,NODETYPE gidList);
   void setItemSet(NODETYPE value,list<NODETYPE> gidList);
   void setItemSet(NODETYPE value,list<NODETYPE> gidList,double totGroups);
   void setItemSet(NODETYPE value);
   void printItemSet();
   void printToDesign(int isPart);
   void printItemSetRecursive(int lev);
   void save_Large_ItemSet(int lev,int isPart);
   void cancellaRef(list<itemSet<NODETYPE>* >* listPtr);
   // Ricorsiva che scende giù in modo ricorsivo.
   // Non si possono cancallare le gid immediatamente
   void createNewItemSet(double,double,int);

   boolean gen_Large_ItemSet(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev);
   boolean gen_final_count(itemSetList<NODETYPE>& ,double,double,int,itemSetHashMap<NODETYPE>&);

   void gen_Large_ItemSet_Base(double,double);
   void checkSupportBaseDef(double,double,  itemSetHashMap<NODETYPE>&,itemSetList<NODETYPE>& );
   void mergeItemSet(int isPart,itemSetList<NODETYPE>& ptrList);
   void remove(itemSetList<NODETYPE>& ptrList);
   boolean existEntry(NODETYPE item);
   void extractRule(String regola,int lev);

   void removeEntry(NODETYPE elem);
   std::map <NODETYPE,itemSetEntry< NODETYPE > > getMap() {return prtt;}

private:
   int numLevel;
   // Root
   std::map <NODETYPE,itemSetEntry< NODETYPE > > prtt;

   String myAncestor;

   typename std::list<itemSet<NODETYPE>* >::iterator refList;


   //  std::vector<std::list<itemSet<String>* > > vectLevel;

   void printToDesignHelper(String here,String padre,ofstream& f1);
   void save_Large_ItemSetHelper(int lev,ofstream& out);
   int  mergeItemSetHelper(String& nToken,ifstream& in,NODETYPE prec,boolean OK,itemSetList<NODETYPE>& ptrList,String ancestor);
   boolean gen_Large_ItemSetHelper(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev);
   boolean gen_final_countHelper(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev,itemSetHashMap<NODETYPE>& pHashMap,itemSetHashMap<NODETYPE>& pHashMapNew);
   //void removeHelper(itemSetList<NODETYPE>& ptrList);
};



/* IMPLEMENTAZIONE DEI METODI DI PARTITION.H */

template< class NODETYPE >
boolean itemSet< NODETYPE >::existEntry(NODETYPE item )
 {
    boolean exist;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(item);
   // std::cout<<"gidList "<<gidList<<" Item "<<value<<std::endl;
    if (i==prtt.end())
       exist=false;
    else exist=true;

    return exist;


 }

template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,NODETYPE gidList )
 {
    itemSetEntry< NODETYPE > entry;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);
   // std::cout<<"gidList "<<gidList<<" Item "<<value<<std::endl;
    if (i==prtt.end())
      {
	//cout<<"new value"<<std::endl;
	entry.setData(value);
	entry.setGidList(gidList);
	prtt[value]=entry;
      }
    else {
      i->second.setGidList(gidList);
   // std::cout<<"inserisco valore nella gidLsit"<<std::endl;
    }

   //itemSetEntry <NODETYPE> entry = new itemSetEntry < NODETYPE > (value);
   //prtt[value]=*entry;

 }

template< class NODETYPE >
void itemSet< NODETYPE >::setItemSetIfExist(NODETYPE value,int gidList )
 {
    itemSetEntry< NODETYPE > entry;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);
   // std::cout<<"gidList "<<gidList<<" Item "<<value<<std::endl;
    if (i==prtt.end())
     {
     std::cout<<"NON ESISTE E NON LO INSERISCO! "<<value<<std::endl;
     }
    else {
    i->second.setGidList(gidList);
   // std::cout<<"inserisco valore nella gidLsit"<<std::endl;
    }

   //itemSetEntry <NODETYPE> entry = new itemSetEntry < NODETYPE > (value);
   //prtt[value]=*entry;

 }

template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,list<NODETYPE> gidList )
 {
    itemSetEntry< NODETYPE > entry;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);
//    std::cout<<"gidList "<<gidList<<" Item "<<value<<std::endl;
    if (i==prtt.end())
     {
  //   std::cout<<"new value"<<std::endl;
     entry.setData(value);
     entry.setGidList(gidList);
     prtt[value]=entry;
     }
    else {
    i->second.setGidList(gidList);
 //   std::cout<<"inserisco valore nella gidLsit"<<std::endl;
    }

   //itemSetEntry <NODETYPE> entry = new itemSetEntry < NODETYPE > (value);
   //prtt[value]=*entry;

 }


template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,list<NODETYPE> gidList,double totGroups )
 {
    itemSetEntry< NODETYPE > entry;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);

    if (i==prtt.end())
     {

     entry.setData(value);
     entry.setGidList(gidList);
     entry.setCountPercent(totGroups);
     prtt[value]=entry;
     }
    else {
    i->second.setGidList(gidList);
    }
 }


template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value )
 {
    itemSetEntry< NODETYPE > entry;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);

    if (i==prtt.end())
     {
      entry.setData(value);
      prtt[value]=entry;

     }
 }

template< class NODETYPE >
void itemSet< NODETYPE >::printItemSet()
 {
    std::list<NODETYPE> gList;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename std::list<NODETYPE>::iterator i2;
    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       std::cout<<"Item "<<i->first<<"GidList: ";
       gList=i->second.getGidList();
       for (i2=gList.begin(); i2!=gList.end();i2++)
           std::cout<<*i2<<"  ";

       std::cout<<std::endl;
     }

 }

template< class NODETYPE >
void itemSet< NODETYPE >::printItemSetRecursive(int lev)
 {
    std::list<NODETYPE> gList;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename std::list<NODETYPE>::iterator i2;

    //cout<<"dentro print"<<std::endl;
    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
        //cout<<"Address "<<i<<std::endl;
       for (int j=0; j<lev;j++)
            std::cout<<"  ";
       std::cout<<"Item "<<i->first<<" Ancestor "<<myAncestor<<" GidList: ";
       gList=i->second.getGidList();
       for (i2=gList.begin(); i2!=gList.end();i2++)
           std::cout<<*i2<<"  ";
        std::cout<<"level"<<getLevel()<<std::endl;
       //" refL "<<*refList<<std::endl;
       //cout<<"* "<<i->second.getItemSet()<<std::endl;
       if (i->second.getItemSet()!=NULL)
       {
         //cout<<"Address "<<i->second.getItemSet()<<std::endl;
         i->second.getItemSet()->printItemSetRecursive(lev+1);
       }
    }
     //cout<<"fine print"<<std::endl;
 }


template< class NODETYPE >
void itemSet< NODETYPE >::createNewItemSet(double nSup,double
totGroups,int lev)
 {
    std::list<NODETYPE> tList,t2,tListNew;
    itemSet<NODETYPE>* prttNew;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator j;
    typename std::list<NODETYPE>::iterator i2;
    boolean toLink;


    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
      // std::cout<<"PADRE!!!!  "<<i->first<<std::endl;
       j=i;
       j++;
       prttNew=new itemSet(lev);
       toLink=false;
       tList=i->second.getGidList();

       while (j!=prtt.end())
         {
         // std::cout<<"figlio generato "<<j->first;
          t2=j->second.getGidList();


          set_intersection(t2.begin(), t2.end(),tList.begin(), tList.end(), back_inserter(tListNew));
         // std::cout<<"intersezione... "<<std::endl;
         // for (i2=tListNew.begin(); i2!=tListNew.end();i2++)
         //     std::cout<<*i2<<"  ";
         // std::cout<<std::endl;
         // std::cout<<"ListNEW!!! "<<tListNew<<std::endl;

          //cout<<"size list"<<tListNew.size()<<std::endl;
          if (  (tListNew.size()>=nSup) && (!tListNew.empty()) )
          {
          //  std::cout<<"non vuoto"<<std::endl;

            prttNew->setItemSet(j->first,tListNew,totGroups);

            toLink=true;
          //  i->second.setItemSetChild(prttNew);
          }
          tListNew.clear();
        //  std::cout<<"sei solo tu"<<std::endl;
          j++;
         }
        i->second.gidListRemove();
        // std::cout<<"stampa prttNew"<<std::endl;
        // prttNew->printItemSet();
         if (toLink)
          {
             i->second.setItemSetChild(prttNew);

             prttNew->createNewItemSet(nSup,totGroups,lev+1);
          }
         else delete prttNew;
        // std::cout<<".."<<std::endl;
        }

 }

// Crea un livello

template< class NODETYPE >
boolean itemSet< NODETYPE >::gen_Large_ItemSet(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev)
{
   itemSet<NODETYPE>* prttNew;
   typename std::list<itemSet<NODETYPE>* >::iterator i;
   std::list<itemSet<NODETYPE>* >* listPtr;
   int rLev;
   boolean otherItemSet,toContinue;

   //cout<<"Dentro gen_Large_ItemSet "<<lev<<std::endl;
   toContinue=false;
   if (lev!=2)
   {
    rLev=lev-1;
    //cout<<"IF di lev!=2 size"<<ptrList.vect.size()<<"level "<<lev<<" RLEV"<<rLev<<std::endl;
    if ((ptrList.vect.size()-1)<=rLev)
        listPtr=ptrList.getList(rLev);

    //cout<<"OK2 "<<ptrList.vect[rLev]<<std::endl;
    for ( i=listPtr->begin(); i!=listPtr->end(); i++)
     {
        //cout<<"*i -"<<*i<<std::endl;

        otherItemSet=((itemSet*)(*i))->gen_Large_ItemSetHelper(ptrList,nSup,totGroups,lev);
        if (otherItemSet) toContinue=true;
      //  std::cout<<"altre partizioni create "<<otherItemSet<<std::endl;
     }
   }
   else toContinue=gen_Large_ItemSetHelper(ptrList,nSup,totGroups,lev);
   //ptrList.print();

   return toContinue;
}

template< class NODETYPE >
boolean itemSet< NODETYPE >::gen_Large_ItemSetHelper(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,unsigned es
int lev)
 {
    std::list<NODETYPE> tList,t2,tListNew;
    itemSet<NODETYPE>* prttNew;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator j;
    typename std::list<NODETYPE>::iterator i2;
    boolean toLink,otherPart;
    std::list<itemSet<NODETYPE>* >*  listPtr;

    otherPart=false;

    //cout<<"create New level"<<ptrList.vect.size()<<"level "<<lev<<std::endl;
    if ((ptrList.vect.size()-1)>=lev)
        listPtr=ptrList.getList(lev);
    else listPtr=new std::list<itemSet<NODETYPE>* >();

    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
      // std::cout<<"PADRE!!!!  "<<i->first<<std::endl;
       j=i;
       j++;
       prttNew=new itemSet(lev);
       toLink=false;
       tList=i->second.getGidList();
       while (j!=prtt.end())
         {
         // std::cout<<"figlio generato "<<j->first;
          t2=j->second.getGidList();


          set_intersection(t2.begin(), t2.end(),tList.begin(), tList.end(), back_inserter(tListNew));
         // std::cout<<"intersezione... "<<std::endl;
         // for (i2=tListNew.begin(); i2!=tListNew.end();i2++)
         //     std::cout<<*i2<<"  ";
         // std::cout<<std::endl;
         // std::cout<<"ListNEW!!! "<<tListNew<<std::endl;

         // std::cout<<"size list"<<tListNew.size()<<std::endl;
          if (  (tListNew.size()>=nSup) && (!tListNew.empty()) )
          {
          //  std::cout<<"non vuoto"<<std::endl;

            prttNew->setItemSet(j->first,tListNew,totGroups);

            toLink=true;
          //  i->second.setItemSetChild(prttNew);
          }
          tListNew.clear();
        //  std::cout<<"sei solo tu"<<std::endl;
          j++;
         }
        i->second.gidListRemove();
        // std::cout<<"stampa prttNew"<<std::endl;
        // prttNew->printItemSet();
         if (toLink)
          {
             i->second.setItemSetChild(prttNew);
             listPtr->push_back(prttNew);

             //cout<<"creato parti ptr "<<prttNew<<std::endl;



          }
         else delete prttNew;
        // std::cout<<".."<<std::endl;
     }
//    std::cout<<"lunghe lista"<<listPtr->size()<<std::endl;
    if (listPtr->size()>0)
     {
       otherPart=true;
       if ((ptrList.vect.size()-1)>=lev)
            ptrList.vect[lev]=listPtr;
       else ptrList.vect.push_back(listPtr);

     }

  //   std::cout<<"lista new dimension VETTORE"<<ptrList.vect.size()<<std::endl;
  //   std::cout<<"lista address"<<listPtr<<std::endl;
  //   ptrList.print();
     // typename std::list<itemSet<NODETYPE>* >::iterator k;
     //for ( k=listPtr.begin(); k!=listPtr.end(); k++)
     //   std::cout<<"*k -"<<*k<<std::endl;
     //cout<<"uffa"<<std::endl;
     return otherPart;
 }


//IMPORTante proprietà della map
//Erasing an element from a std::map also does not invalidate any iterators, except,
//of course, for iterators that actually point to the element that is being erased.
template< class NODETYPE >
void itemSet< NODETYPE >::gen_Large_ItemSet_Base(double nSup,double totGroups)
 {
    std::list<NODETYPE> gList;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    NODETYPE elemToDelete;
    boolean isDelete;

    isDelete=false;
   // typename std::list<NODETYPE>::iterator i2;
    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       if (isDelete)
         {
          prtt.erase(elemToDelete);
          isDelete=false;
         }

       if (i->second.getGidList().size() < nSup)
       {
          //cout<<"elimino entry dalla prt base"<<std::endl;
          gList=i->second.getGidList();

          elemToDelete=i->first;
          isDelete=true;

       }
       else i->second.setCountPercent(totGroups);
     }

     //se fosse l'ultimo elemento della MAP :-)
     if (isDelete)
       {
        prtt.erase(elemToDelete);
        isDelete=false;
       }

 }
template< class NODETYPE >
void itemSet< NODETYPE >::cancellaRef( std::list<itemSet<NODETYPE>* >* listPtr)
 {
     typename std::list<itemSet<NODETYPE>* >::iterator riferimento;
     std::cout<<"opa"<<std::endl;

     riferimento=getRefList();
     std::cout<<"refList"<<*riferimento<<std::endl;
     listPtr->erase(riferimento);
 }

//IMPORTante proprietà della map
//Erasing an element from a map also does not invalidate any iterators, except,
//of course, for iterators that actually point to the element that is being erased.

template< class NODETYPE >
void itemSet< NODETYPE >::checkSupportBaseDef(
			     double nSup,
			     double totGroups, 
			     itemSetHashMap<NODETYPE>& pHashMap,
			     itemSetList<NODETYPE>& ptrList)
 {

    itemSet<NODETYPE>* prttNew;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    int nlev;
    std::list<itemSet<NODETYPE>* >* listPtr;
    std::map <NODETYPE,itemSetEntry< NODETYPE > > tmpMap;
    NODETYPE elemToDelete;
    boolean isDelete;

    //tmpMap=getMap();

   // typename std::list<NODETYPE>::iterator i2;
    isDelete=false;

    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       if (isDelete)
         {
          prtt.erase(elemToDelete);
          isDelete=false;
         }
       std::cout<<"elemento "<<i->first<<std::endl;

       if (i->second.getGidList().size() < nSup)
       {
          std::cout<<"CheckSupportBaseDef size"<<i->first<<" "<<i->second.getGidList().size()<<std::endl;
          prttNew=i->second.getItemSet();
          if (prttNew!=NULL)
            {
              prttNew->remove(ptrList);
              // rimuovere la entry!!!!
              nlev=prttNew->getLevel();
             // std::cout<<"refList prima"<<*refList<<std::endl;
              std::cout<<"livello in cui cercare nella lista "<<nlev<<std::endl;
              listPtr=ptrList.getList(nlev);
  	      std::cout<<"delete ItemSet dalla lista"<<prttNew<<std::endl;
  	      prttNew->cancellaRef(listPtr);
              //listPtr->erase(refList);
              delete prttNew;
            }
          std::cout<<"elemento  che sto a cancella "<<i->first<<std::endl;
          elemToDelete=i->first;
          isDelete=true;
          //prtt.erase(i->first);
       }
       else
       {
           i->second.setCountPercent(totGroups);
           String sec=(String) i->first;
           const char* like=(i->first).c_str();
           pHashMap.add(like,sec);
          //hashLevel.insert(make_pair(like,"ok"));
       }
     }
  // Se fosse l'ultimo elemento della MAP ad essere cancellato :-)
  if (isDelete)
         {
          prtt.erase(elemToDelete);
          isDelete=false;
         }


 }

template< class NODETYPE >
void itemSet< NODETYPE >::save_Large_ItemSet(int lev,int isPart)
 {

    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    char filename[8];

    sprintf(filename,"example/L%d.txt",isPart);
    ofstream out(filename);

     for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {

      out<<lev<<":::"<<i->first<<":::"<<setprecision(4)<<i->second.getCount()<<std::endl;

      if (i->second.getItemSet()!=NULL)
         i->second.getItemSet()->save_Large_ItemSetHelper(lev+1,out);
      //prtt.erase(i);
    }

   prtt.clear();
   out.close();
 }

template< class NODETYPE >
void itemSet< NODETYPE >::save_Large_ItemSetHelper(int lev,ofstream& out)
 {
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

     for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {

    /*  out<<lev<<":::"<<i->first<<":::"<<std::endl;

      if (i->second.getItemSet()!=NULL)
      {
         i->second.getItemSet()->save_Large_ItemSetHelper(lev+1,out);
         //Quando esco dealloco il puntatore
         delete i->second.getItemSet();
      }
   */
    }

 }


template< class NODETYPE >
void itemSet< NODETYPE >::mergeItemSet(int isPart,itemSetList<NODETYPE>& ptrList)
 {
   char number[20];
   String token,anc;
   NODETYPE temp;
   int level;

   anc="";
   sprintf(number,"example/L%d.txt",isPart);
   ifstream in(number);
   level=0;

   temp=(NODETYPE)"";
   mergeItemSetHelper(token,in,temp,true,ptrList,anc);

}


template< class NODETYPE >
int itemSet< NODETYPE >::mergeItemSetHelper(String& nToken,ifstream&in,NODETYPE prec,boolean OK,itemSetList<NODETYPE>& ptrList, String ancestor)
{

    int level,levelPast,diff;
    float sup;
    NODETYPE item;
    String token;
    itemSet<NODETYPE>* prttNew;
    boolean isNewItemSet;
    std::list<itemSet<NODETYPE>* >*  listPtr;
    typename std::list<itemSet<NODETYPE>* >::iterator riferimento;

    //cout<<"---- Dentro HELPER ----"<<std::endl;

    level=0;
    isNewItemSet=false;
    std::cout<<"prec"<<prec<<"ancestor "<<ancestor<<std::endl;
    if (ancestor!="")
       ancestor=ancestor+prec+",";
    else {
           if (prec!="")
           ancestor=prec+",";
         }
    //cout<<"Ancestor prima del ciclo "<<ancestor<<std::endl;
    if (OK=true)
    {
      //cout<<"Ok True"<<std::endl;
      getline(in, token);

      StringTokenizer mytoken((String&)token,":::\r\n");

      if (mytoken.hasMoreTokens())
       {
        level=atoi(mytoken.nextToken().c_str());
        item=(NODETYPE)mytoken.nextToken();

       }
    }
    if (level==1) ancestor="";
    nToken=token;
    //cout<<"Level"<<level<<" Item: "<<item<<std::endl;
    //cout<<"Getlevel"<<getLevel()<<std::endl;
    diff=level-getLevel();
    levelPast=level;
    //cout<<"levelPast "<<levelPast<<std::endl;
    //cout<<"Differenza prima WHILE"<<diff<<std::endl;
    while (diff>=0)
      {
        if (diff>0)
          {
          //cout<<"livello successivo "<<item<<std::endl;
          typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator curr;

          curr=prtt.find(prec);
          prttNew=curr->second.getItemSet();
          if (prttNew==NULL)
          {
           prttNew=new itemSet();
           prttNew->setAncestor(ancestor);
           prttNew->setItemSet(item);
           prttNew->setLevel(level);
           curr->second.setItemSetChild(prttNew);
           isNewItemSet=true;
           //cout<<"creata ItemSet del livello "<<level<<" elemento "<<item<<std::endl;
           //cout<<"valore ItemSet creata "<<prttNew<<std::endl;
          }
          else
          {
           prttNew->setItemSet(item);

          }

          if (isNewItemSet)
            {
              isNewItemSet=false;

              if ((ptrList.vect.size()-1)>=level)
                {
                  listPtr=ptrList.getList(level);
                  //cout<<"utilizzo una lista già esistente "<<listPtr<<std::endl;
                  listPtr->push_back(prttNew);
		  riferimento=listPtr->end();
		  riferimento--;
		  //cout<<"FOOOT: addr ptr list"<<*riferimento<<std::endl;
                  prttNew->setRefList(riferimento);
                  ptrList.vect[level]=listPtr;
                }
              else
                {
                 listPtr=new std::list<itemSet<NODETYPE>* >();
                 //cout<<"creo nuova LISTA "<<listPtr<<std::endl;
                 listPtr->push_back(prttNew);
		 riferimento=listPtr->end();
	         riferimento--;
                 prttNew->setRefList(riferimento);
       	         //cout<<"FOOOT: addr ptr list"<<*riferimento<<std::endl;
                 ptrList.vect.push_back(listPtr);
                }
            }
           //else std::cout<<"ho gia' la ItemSet"<<std::endl;

          //cout<<"Inserico il link da "<<prec<<" a "<<item<<std::endl;
          std::cout<<"ancestor"<<ancestor<<std::endl;
          levelPast=prttNew->mergeItemSetHelper(nToken,in,item,true,ptrList,ancestor);
	  //cout<<"Ancestor dopo ricorsione :"<<ancestor<<" inpart"<<getAncestor()<<std::endl;
	  ancestor=getAncestor();
          //cout<<"Qui avevo item "<<prec<<std::endl;
          //cout<<"Uscita dalla ricorsione levelPast "<<levelPast<<std::endl;
          //cout<<"Uscita dalla ricorsione level "<<level<<std::endl;
          diff=levelPast-getLevel();


          }
        else
        {
          //cout<<"diff=0"<<std::endl;

          StringTokenizer mytoken2((String&)nToken,":::\r\n");

          if (mytoken2.hasMoreTokens())
          {
            level=atoi(mytoken2.nextToken().c_str());
            item=(NODETYPE)mytoken2.nextToken();
          }
          //cout<<"SetitemSet Item in stesso livello "<<getLevel()<<" "<<item<<std::endl;
          setItemSet(item);
          //printItemSetRecursive(1);
          levelPast=mergeItemSetHelper(nToken,in,item,true,ptrList,ancestor);
	  //cout<<"Ancestor dopo ricorsione :"<<ancestor;
          //cout<<"Uscita dalla ricorsione levelPast "<<levelPast<<std::endl;
          //cout<<"Uscita dalla ricorsione level "<<level<<std::endl;
          diff=levelPast-getLevel();
          //cout<<"=0 differenza "<<diff<<std::endl;
        }

      }
     //cout<<"finePROCEDURA"<<levelPast<<std::endl;
     return levelPast;

}



template< class NODETYPE >
boolean itemSet< NODETYPE >::gen_final_count(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev,itemSetHashMap<NODETYPE>& pHashMap)
{
   typename std::list<itemSet<NODETYPE>* >::iterator i;
   std::list<itemSet<NODETYPE>* >* listPtr;
   int rLev;
   boolean toContinue,okNext;
   //cout<<"creazione phashMapNew"<<std::endl;
   itemSetHashMap<NODETYPE> pHashMapNew;


   toContinue=true;
   std::cout<<"EstimateLevel: "<<lev<<std::endl;

   rLev=lev-1;
   if ((ptrList.vect.size()-1)>=rLev)
        listPtr=ptrList.getList(rLev);
   else toContinue=false;

//   if (toContinue) std::cout<<"Size lista: "<<listPtr->empty()<<std::endl;
//pHashMap.print();
   if (toContinue)
   {
        if (lev!=2)
        {
                toContinue=false;
                for ( i=listPtr->begin(); i!=listPtr->end(); i++)
                {
                        std::cout<<"lista address: "<<"*i -"<<*i<<std::endl;

                        okNext=((itemSet*)(*i))->gen_final_countHelper(ptrList,nSup,totGroups,lev,pHashMap,pHashMapNew);
                       // std::cout<<" okNext "<< okNext<<std::endl;
                       // if (okNext) toContinue=true;
                }
        }
        else toContinue=gen_final_countHelper(ptrList,nSup,totGroups,lev,pHashMap,pHashMapNew);


    pHashMap=pHashMapNew;
   }

   return toContinue;
}

// importante : va in profondità e allo stesso tempo sfrutta apriori.

// Rivedere e compattare questa procedura.
//

template< class NODETYPE >
boolean itemSet< NODETYPE >::gen_final_countHelper(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev,itemSetHashMap<NODETYPE>& pHashMap,itemSetHashMap<NODETYPE>& pHashMapNew)
 {
    std::list<NODETYPE> tList,t2,tListNew;
    itemSet<NODETYPE>* prttNew;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator j;
    typename std::list<NODETYPE>::iterator i2;
    boolean oneOK,doApriori;
    std::list<itemSet<NODETYPE>* >*  listPtr;
    String myAncestor;
    int level;
    std::map <NODETYPE,itemSetEntry< NODETYPE > > tmp;
    typename std::list<itemSet<NODETYPE>* >::iterator riferimento;
    NODETYPE elemToDelete;
    boolean isDelete;



    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       std::cout<<"PADRE  "<<i->first<<std::endl;

       prttNew=i->second.getItemSet();
       std::cout<<"indirizzo ItemSet figlia : "<<prttNew<<std::endl;
       tList=i->second.getGidList();

       oneOK=false;
       // il controllo su tList non dovrebbe più servire.
       if ( (prttNew!=NULL) && (!tList.empty()) )
       {
         myAncestor=prttNew->getAncestor();
         level=prttNew->getLevel();
         //cout<<"cosa sto per analizzare"<<std::endl;
         //prttNew->printItemSet();
         // std::cout<<"fine cosa sto per analizzare"<<std::endl;

         tmp=prttNew->getMap();

         isDelete=false;


         for( j=tmp.begin(); j !=tmp.end( ) ; j++ )
         {

           if (isDelete)
             {
                //tmp.erase(elemToDelete);
                prttNew->removeEntry(elemToDelete);
                isDelete=false;
             }

           std::cout<<"figlio in esame"<<j->first<<std::endl;
           doApriori=pHashMap.findTies(myAncestor,j->first,level);

           if (doApriori)
             {
              std::cout<<"tutti gli antenati ci sono dei fare merge lista"<<std::endl;
              t2=(prtt.find(j->first))->second.getGidList();
              //t2=j->second.getGidList();
              set_intersection(t2.begin(), t2.end(),tList.begin(), tList.end(), back_inserter(tListNew));
              //cout<<"size della lista "<<tListNew.size()<<std::endl;
              if (  (tListNew.size()>=nSup) && (!tListNew.empty()) )
        	{
                   prttNew->setItemSet(j->first,tListNew,totGroups);
                   String newElemHash=myAncestor+(j->first).c_str();
                   const char* it=newElemHash.c_str();
                   std::cout<<"Supporto Sufficiente nuovo elemnto in pHashMapNew"<<newElemHash<<std::endl;
                   //cout<<"elemento in hash"<<it<<"----"<<std::endl;
                   pHashMapNew.add(it,newElemHash);
                   oneOK=true;
                   //pHashMapNew.print();
                }
                else
                 {
                    std::cout<<"devo rimuovere tutto sotto "<<j->first<<" ItemSet "<<prttNew<<std::endl;

                    //ptrList.printAddrItemSet();
                    std::cout<<"ma non questo livello"<<std::endl;
                    if (j->second.getItemSet()!=NULL)
                       {
                        std::cout<<"address ItemSet figlia "<<j->second.getItemSet();
                        j->second.getItemSet()->remove(ptrList);
                        int nextLev;
                        nextLev= j->second.getItemSet()->getLevel();
                        listPtr=ptrList.getList(nextLev);
  	                std::cout<<"delete ItemSet dalla lista"<<j->second.getItemSet()<<std::endl;
  	                riferimento= j->second.getItemSet()->getRefList();
                        listPtr->erase(riferimento);


                        delete(j->second.getItemSet());
                       }
                       elemToDelete=j->first;
                       isDelete=true;

                 //cout<<"elemento  che sto a cancella "<<i->first<<std::endl;
                 //elemToDelete=i->first;
                 //isDelete=true;
                }
                tListNew.clear();
             }
           else
             {
               std::cout<<"posso fare aPriori"<<myAncestor<<" "<<j->first<<std::endl;
                 if (j->second.getItemSet()!=NULL)
                       {
                        std::cout<<"address ItemSet figlia "<<j->second.getItemSet();
                        j->second.getItemSet()->remove(ptrList);
                        int nextLev;
                        nextLev= j->second.getItemSet()->getLevel();
                        listPtr=ptrList.getList(nextLev);
  	                std::cout<<"delete ItemSet dalla lista"<<j->second.getItemSet()<<std::endl;
  	                riferimento= j->second.getItemSet()->getRefList();
                        listPtr->erase(riferimento);


                        delete(j->second.getItemSet());

                         //elemToDelete=j->first;
                         //isDelete=true;
                       }
                       elemToDelete=j->first;
                       isDelete=true;


             }


        }

       // se l'elemento da cancellare fosse l'ultimo della map :)
       if (isDelete)
          {
             std::cout<<"ultimo elem da cancellare "<<elemToDelete<<std::endl;
             prttNew->removeEntry(elemToDelete);
             isDelete=false;
          }

      //if (!oneOK) std::cout<<"posso cancellare questo livello non ho niente addrx :"<<prttNew<<std::endl;
      std::cout<<"ho finito entry "<<i->first<<" oneOK "<<oneOK<<" ItemSet a cui punto "<<prttNew<<std::endl;
      if (!oneOK)
         {
            std::cout<<"cancello prttNew"<<prttNew<<" pttrC di i "<<i->second.getItemSet()<<std::endl;
 	    i->second.getItemSet()->remove(ptrList);

            int nLev;
            nLev=(i->second.getItemSet()->getLevel())+1;
	    std::cout<<"nlev "<<nLev<<std::endl;
            listPtr=ptrList.getList(nLev);

            riferimento= i->second.getItemSet()->getRefList();
            listPtr->erase(riferimento);
            delete(i->second.getItemSet());
            i->second.setItemSetChild(NULL);


         }
   }
   else std::cout<<"non ho una ItemSet figlia"<<std::endl;
   i->second.gidListRemove();
  }



  std::cout<<"finito gen_final_count"<<std::endl;
  return true;

}



template< class NODETYPE >
void itemSet< NODETYPE >::removeEntry(NODETYPE elem)
 {
    prtt.erase(elem);
 }

template< class NODETYPE >
void itemSet< NODETYPE >::extractRule(String regola,int lev)
 {
   typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   String rule;

   rule=regola;

   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
        //cout<<"Address "<<i<<std::endl;
        regola=rule+(String)i->first+",";
       //cout<<"Item "<<i->first<<" GidList: ";
       //gList=i->second.getGidList();
       //for (i2=gList.begin(); i2!=gList.end();i2++)
       //    std::cout<<*i2<<"  ";
       //cout<<"level"<<getLevel()<<std::endl;
       if (lev!=1) combina2(regola);
       //  std::cout<<"regola"<<regola<<std::endl;
       if (i->second.getItemSet()!=NULL)
       {
         //cout<<"Address "<<i->second.getItemSet()<<std::endl;

         i->second.getItemSet()->extractRule(regola,lev+1);
         std::cout<<std::endl;
       }
     regola="";
    }

 }



template< class NODETYPE >
void itemSet< NODETYPE >::remove(itemSetList<NODETYPE>& ptrList)
 {
   typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   itemSet<NODETYPE>* p;
   std::list<itemSet<NODETYPE>* >* listPtr;
   int lev;
   typename std::list<itemSet<NODETYPE>* >::iterator riferimento;

   lev=getLevel();
   std::cout<<"Remove tutto lev"<<lev<<std::endl;
   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
         std::cout<<"Dentro REmove da taglio "<<i->first<<std::endl;
         p=i->second.getItemSet();
         if (p!=NULL)
  	    {
  	       p->remove(ptrList);
  	       listPtr=ptrList.getList(lev);
  	       std::cout<<"delete ItemSet dalla lista"<<p<<std::endl;
  	       riferimento=p->getRefList();
               listPtr->erase(riferimento);
  	       delete p;
            }
        //prtt.erase(i);

     }
   prtt.clear();
 }

/*
template< class NODETYPE >
void itemSet< NODETYPE >::removeHelper(itemSetList<NODETYPE>& ptrList)
 {
   typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   itemSet<NODETYPE>* p;
   int lev;


  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
         p=i->second.getItemSet();
         std::cout<<"item"<<i->first<<std::endl;

  	 if (p!=NULL)
  	    {
  	       //p->removeHelper(ptrList);
  	       lev=getLevel();
     	       //ptrList.remove(lev+1,p);
  	       delete p;
            }
        prtt.erase(i);
     }
  std::cout<<"fine removeHelper"<<std::endl;
}
*/





//------------------------- disegno GRAFICAMENTE la struttura : WinGraph //
template< class NODETYPE >
void itemSet< NODETYPE >::printToDesign(int isPart)
 {

   String files,label1,values,vStruct,nod,address,idList;
   typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   typename std::list<NODETYPE>::iterator i2;
   std::list<NODETYPE> tList;
   char filename[16];



   sprintf(filename,"design/L%d.txt",isPart);

   ofstream f1(filename);

   f1<<"digraph minerule { "<<std::endl;
   f1<<"graph[fontsize=8]; edge  [fontsize=8]; node  [fontsize=8]; ranksep = .30; nodesep = .25;"<<std::endl;
   f1<<"node [fontname=\"Courier\"]; "<<std::endl;
   f1<<"node [shape=record];"<<std::endl;

   vStruct="struct0";
   label1="struct0[shape=record,label=\"";
   values="";
   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
      idList="";
      values=values+"<"+i->first+">"+i->first;
      tList=i->second.getGidList();
      for (i2=tList.begin(); i2!=tList.end();i2++)
      {
            std::stringstream o2;
           o2 <<*i2;
           address=o2.str();
           idList=idList+address+"-";
      }

     values=values+":  "+idList+"|";

     }

   int siz=values.size();
   siz=siz-1;
   if (siz>0)  values.resize(siz);
   label1=label1+values+"\"];";
   f1<<label1<<std::endl;
   values="";

   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
   {

    nod=i->first;

     if ((i->second.getItemSet())!=NULL)
     {

      std::stringstream o2;
      o2 <<i->second.getItemSet();
      address=o2.str();
      nod=vStruct+":"+nod;

      i->second.getItemSet()->printToDesignHelper(nod,address,f1);
     }
   }

  f1<<"}"<<std::endl;

 }

// Segue la sintassi per creare un file testo per legare i vari nodi.
// Vedi WinGraph e programma vizuale.exe
template< class NODETYPE >
void itemSet< NODETYPE >::printToDesignHelper(String here,String padre,ofstream& f1)
 {
   typename std::map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   String files,label1,values,vStruct,nod,address,idList;
   typename std::list<NODETYPE>::iterator i2;
   std::list<NODETYPE> tList;


   label1="struct"+padre+"[shape=record,label=\""+padre+"|";
   values="";

   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
      idList="";
      values=values+"<"+i->first+">"+i->first;
         tList=i->second.getGidList();
      for (i2=tList.begin(); i2!=tList.end();i2++)
      {
            std::stringstream o2;
           o2 <<*i2;
           address=o2.str();
           idList=idList+address+"-";
      }
      //String myAncestor2=myAncestor;
      //myAncestor2.replace(1,size(),",");
      values=values+":  "+idList+"|";
     }

    int siz=values.size();
    siz=siz-1;

   if (siz>0)    values.resize(siz);

   label1=label1+values+"\"];";
   f1<<label1<<std::endl;
   f1<<here<<"->"<<"struct"+padre<<std::endl;
   values="";
   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
   {

     nod=i->first;

     if (i->second.getItemSet()!=NULL)
     {
      std::stringstream o2;
      o2 <<  i->second.getItemSet();
      address=o2.str();
      nod="struct"+padre+":"+nod;

      i->second.getItemSet()->printToDesignHelper(nod,address,f1);
     }

   }

 }

#endif


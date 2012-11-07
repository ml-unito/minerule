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


using namespace std;

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

   void setRefList(typename list<itemSet<NODETYPE>* >::iterator riferimento) {refList=riferimento;}
   typename list<itemSet<NODETYPE>* >::iterator getRefList()  {return refList;}
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
   map <NODETYPE,itemSetEntry< NODETYPE > > getMap() {return prtt;}

private:
   int numLevel;
   // Root
   map <NODETYPE,itemSetEntry< NODETYPE > > prtt;

   String myAncestor;

   typename list<itemSet<NODETYPE>* >::iterator refList;


   //  vector<list<itemSet<String>* > > vectLevel;

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
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(item);
   // cout<<"gidList "<<gidList<<" Item "<<value<<endl;
    if (i==prtt.end())
       exist=false;
    else exist=true;

    return exist;


 }

template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,NODETYPE gidList )
 {
    itemSetEntry< NODETYPE > entry;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);
   // cout<<"gidList "<<gidList<<" Item "<<value<<endl;
    if (i==prtt.end())
      {
	//cout<<"new value"<<endl;
	entry.setData(value);
	entry.setGidList(gidList);
	prtt[value]=entry;
      }
    else {
      i->second.setGidList(gidList);
   // cout<<"inserisco valore nella gidLsit"<<endl;
    }

   //itemSetEntry <NODETYPE> entry = new itemSetEntry < NODETYPE > (value);
   //prtt[value]=*entry;

 }

template< class NODETYPE >
void itemSet< NODETYPE >::setItemSetIfExist(NODETYPE value,int gidList )
 {
    itemSetEntry< NODETYPE > entry;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);
   // cout<<"gidList "<<gidList<<" Item "<<value<<endl;
    if (i==prtt.end())
     {
     cout<<"NON ESISTE E NON LO INSERISCO! "<<value<<endl;
     }
    else {
    i->second.setGidList(gidList);
   // cout<<"inserisco valore nella gidLsit"<<endl;
    }

   //itemSetEntry <NODETYPE> entry = new itemSetEntry < NODETYPE > (value);
   //prtt[value]=*entry;

 }

template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,list<NODETYPE> gidList )
 {
    itemSetEntry< NODETYPE > entry;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

    i=prtt.find(value);
//    cout<<"gidList "<<gidList<<" Item "<<value<<endl;
    if (i==prtt.end())
     {
  //   cout<<"new value"<<endl;
     entry.setData(value);
     entry.setGidList(gidList);
     prtt[value]=entry;
     }
    else {
    i->second.setGidList(gidList);
 //   cout<<"inserisco valore nella gidLsit"<<endl;
    }

   //itemSetEntry <NODETYPE> entry = new itemSetEntry < NODETYPE > (value);
   //prtt[value]=*entry;

 }


template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,list<NODETYPE> gidList,double totGroups )
 {
    itemSetEntry< NODETYPE > entry;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

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
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

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
    list<NODETYPE> gList;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename list<NODETYPE>::iterator i2;
    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       cout<<"Item "<<i->first<<"GidList: ";
       gList=i->second.getGidList();
       for (i2=gList.begin(); i2!=gList.end();i2++)
           cout<<*i2<<"  ";

       cout<<endl;
     }

 }

template< class NODETYPE >
void itemSet< NODETYPE >::printItemSetRecursive(int lev)
 {
    list<NODETYPE> gList;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename list<NODETYPE>::iterator i2;

    //cout<<"dentro print"<<endl;
    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
        //cout<<"Address "<<i<<endl;
       for (int j=0; j<lev;j++)
            cout<<"  ";
       cout<<"Item "<<i->first<<" Ancestor "<<myAncestor<<" GidList: ";
       gList=i->second.getGidList();
       for (i2=gList.begin(); i2!=gList.end();i2++)
           cout<<*i2<<"  ";
        cout<<"level"<<getLevel()<<endl;
       //" refL "<<*refList<<endl;
       //cout<<"* "<<i->second.getItemSet()<<endl;
       if (i->second.getItemSet()!=NULL)
       {
         //cout<<"Address "<<i->second.getItemSet()<<endl;
         i->second.getItemSet()->printItemSetRecursive(lev+1);
       }
    }
     //cout<<"fine print"<<endl;
 }


template< class NODETYPE >
void itemSet< NODETYPE >::createNewItemSet(double nSup,double
totGroups,int lev)
 {
    list<NODETYPE> tList,t2,tListNew;
    itemSet<NODETYPE>* prttNew;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator j;
    typename list<NODETYPE>::iterator i2;
    boolean toLink;


    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
      // cout<<"PADRE!!!!  "<<i->first<<endl;
       j=i;
       j++;
       prttNew=new itemSet(lev);
       toLink=false;
       tList=i->second.getGidList();

       while (j!=prtt.end())
         {
         // cout<<"figlio generato "<<j->first;
          t2=j->second.getGidList();


          set_intersection(t2.begin(), t2.end(),tList.begin(), tList.end(), back_inserter(tListNew));
         // cout<<"intersezione... "<<endl;
         // for (i2=tListNew.begin(); i2!=tListNew.end();i2++)
         //     cout<<*i2<<"  ";
         // cout<<endl;
         // cout<<"ListNEW!!! "<<tListNew<<endl;

          //cout<<"size list"<<tListNew.size()<<endl;
          if (  (tListNew.size()>=nSup) && (!tListNew.empty()) )
          {
          //  cout<<"non vuoto"<<endl;

            prttNew->setItemSet(j->first,tListNew,totGroups);

            toLink=true;
          //  i->second.setItemSetChild(prttNew);
          }
          tListNew.clear();
        //  cout<<"sei solo tu"<<endl;
          j++;
         }
        i->second.gidListRemove();
        // cout<<"stampa prttNew"<<endl;
        // prttNew->printItemSet();
         if (toLink)
          {
             i->second.setItemSetChild(prttNew);

             prttNew->createNewItemSet(nSup,totGroups,lev+1);
          }
         else delete prttNew;
        // cout<<".."<<endl;
        }

 }

// Crea un livello

template< class NODETYPE >
boolean itemSet< NODETYPE >::gen_Large_ItemSet(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev)
{
   itemSet<NODETYPE>* prttNew;
   typename list<itemSet<NODETYPE>* >::iterator i;
   list<itemSet<NODETYPE>* >* listPtr;
   int rLev;
   boolean otherItemSet,toContinue;

   //cout<<"Dentro gen_Large_ItemSet "<<lev<<endl;
   toContinue=false;
   if (lev!=2)
   {
    rLev=lev-1;
    //cout<<"IF di lev!=2 size"<<ptrList.vect.size()<<"level "<<lev<<" RLEV"<<rLev<<endl;
    if ((ptrList.vect.size()-1)<=rLev)
        listPtr=ptrList.getList(rLev);

    //cout<<"OK2 "<<ptrList.vect[rLev]<<endl;
    for ( i=listPtr->begin(); i!=listPtr->end(); i++)
     {
        //cout<<"*i -"<<*i<<endl;

        otherItemSet=((itemSet*)(*i))->gen_Large_ItemSetHelper(ptrList,nSup,totGroups,lev);
        if (otherItemSet) toContinue=true;
      //  cout<<"altre partizioni create "<<otherItemSet<<endl;
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
    list<NODETYPE> tList,t2,tListNew;
    itemSet<NODETYPE>* prttNew;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator j;
    typename list<NODETYPE>::iterator i2;
    boolean toLink,otherPart;
    list<itemSet<NODETYPE>* >*  listPtr;

    otherPart=false;

    //cout<<"create New level"<<ptrList.vect.size()<<"level "<<lev<<endl;
    if ((ptrList.vect.size()-1)>=lev)
        listPtr=ptrList.getList(lev);
    else listPtr=new list<itemSet<NODETYPE>* >();

    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
      // cout<<"PADRE!!!!  "<<i->first<<endl;
       j=i;
       j++;
       prttNew=new itemSet(lev);
       toLink=false;
       tList=i->second.getGidList();
       while (j!=prtt.end())
         {
         // cout<<"figlio generato "<<j->first;
          t2=j->second.getGidList();


          set_intersection(t2.begin(), t2.end(),tList.begin(), tList.end(), back_inserter(tListNew));
         // cout<<"intersezione... "<<endl;
         // for (i2=tListNew.begin(); i2!=tListNew.end();i2++)
         //     cout<<*i2<<"  ";
         // cout<<endl;
         // cout<<"ListNEW!!! "<<tListNew<<endl;

         // cout<<"size list"<<tListNew.size()<<endl;
          if (  (tListNew.size()>=nSup) && (!tListNew.empty()) )
          {
          //  cout<<"non vuoto"<<endl;

            prttNew->setItemSet(j->first,tListNew,totGroups);

            toLink=true;
          //  i->second.setItemSetChild(prttNew);
          }
          tListNew.clear();
        //  cout<<"sei solo tu"<<endl;
          j++;
         }
        i->second.gidListRemove();
        // cout<<"stampa prttNew"<<endl;
        // prttNew->printItemSet();
         if (toLink)
          {
             i->second.setItemSetChild(prttNew);
             listPtr->push_back(prttNew);

             //cout<<"creato parti ptr "<<prttNew<<endl;



          }
         else delete prttNew;
        // cout<<".."<<endl;
     }
//    cout<<"lunghe lista"<<listPtr->size()<<endl;
    if (listPtr->size()>0)
     {
       otherPart=true;
       if ((ptrList.vect.size()-1)>=lev)
            ptrList.vect[lev]=listPtr;
       else ptrList.vect.push_back(listPtr);

     }

  //   cout<<"lista new dimension VETTORE"<<ptrList.vect.size()<<endl;
  //   cout<<"lista address"<<listPtr<<endl;
  //   ptrList.print();
     // typename list<itemSet<NODETYPE>* >::iterator k;
     //for ( k=listPtr.begin(); k!=listPtr.end(); k++)
     //   cout<<"*k -"<<*k<<endl;
     //cout<<"uffa"<<endl;
     return otherPart;
 }


//IMPORTante proprietà della map
//Erasing an element from a map also does not invalidate any iterators, except,
//of course, for iterators that actually point to the element that is being erased.
template< class NODETYPE >
void itemSet< NODETYPE >::gen_Large_ItemSet_Base(double nSup,double totGroups)
 {
    list<NODETYPE> gList;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    NODETYPE elemToDelete;
    boolean isDelete;

    isDelete=false;
   // typename list<NODETYPE>::iterator i2;
    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       if (isDelete)
         {
          prtt.erase(elemToDelete);
          isDelete=false;
         }

       if (i->second.getGidList().size() < nSup)
       {
          //cout<<"elimino entry dalla prt base"<<endl;
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
void itemSet< NODETYPE >::cancellaRef( list<itemSet<NODETYPE>* >* listPtr)
 {
     typename list<itemSet<NODETYPE>* >::iterator riferimento;
     cout<<"opa"<<endl;

     riferimento=getRefList();
     cout<<"refList"<<*riferimento<<endl;
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
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    int nlev;
    list<itemSet<NODETYPE>* >* listPtr;
    map <NODETYPE,itemSetEntry< NODETYPE > > tmpMap;
    NODETYPE elemToDelete;
    boolean isDelete;

    //tmpMap=getMap();

   // typename list<NODETYPE>::iterator i2;
    isDelete=false;

    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       if (isDelete)
         {
          prtt.erase(elemToDelete);
          isDelete=false;
         }
       cout<<"elemento "<<i->first<<endl;

       if (i->second.getGidList().size() < nSup)
       {
          cout<<"CheckSupportBaseDef size"<<i->first<<" "<<i->second.getGidList().size()<<endl;
          prttNew=i->second.getItemSet();
          if (prttNew!=NULL)
            {
              prttNew->remove(ptrList);
              // rimuovere la entry!!!!
              nlev=prttNew->getLevel();
             // cout<<"refList prima"<<*refList<<endl;
              cout<<"livello in cui cercare nella lista "<<nlev<<endl;
              listPtr=ptrList.getList(nlev);
  	      cout<<"delete ItemSet dalla lista"<<prttNew<<endl;
  	      prttNew->cancellaRef(listPtr);
              //listPtr->erase(refList);
              delete prttNew;
            }
          cout<<"elemento  che sto a cancella "<<i->first<<endl;
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

    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    char filename[8];

    sprintf(filename,"example/L%d.txt",isPart);
    ofstream out(filename);

     for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {

      out<<lev<<":::"<<i->first<<":::"<<setprecision(4)<<i->second.getCount()<<endl;

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
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;

     for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {

    /*  out<<lev<<":::"<<i->first<<":::"<<endl;

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
    list<itemSet<NODETYPE>* >*  listPtr;
    typename list<itemSet<NODETYPE>* >::iterator riferimento;

    //cout<<"---- Dentro HELPER ----"<<endl;

    level=0;
    isNewItemSet=false;
    cout<<"prec"<<prec<<"ancestor "<<ancestor<<endl;
    if (ancestor!="")
       ancestor=ancestor+prec+",";
    else {
           if (prec!="")
           ancestor=prec+",";
         }
    //cout<<"Ancestor prima del ciclo "<<ancestor<<endl;
    if (OK=true)
    {
      //cout<<"Ok True"<<endl;
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
    //cout<<"Level"<<level<<" Item: "<<item<<endl;
    //cout<<"Getlevel"<<getLevel()<<endl;
    diff=level-getLevel();
    levelPast=level;
    //cout<<"levelPast "<<levelPast<<endl;
    //cout<<"Differenza prima WHILE"<<diff<<endl;
    while (diff>=0)
      {
        if (diff>0)
          {
          //cout<<"livello successivo "<<item<<endl;
          typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator curr;

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
           //cout<<"creata ItemSet del livello "<<level<<" elemento "<<item<<endl;
           //cout<<"valore ItemSet creata "<<prttNew<<endl;
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
                  //cout<<"utilizzo una lista già esistente "<<listPtr<<endl;
                  listPtr->push_back(prttNew);
		  riferimento=listPtr->end();
		  riferimento--;
		  //cout<<"FOOOT: addr ptr list"<<*riferimento<<endl;
                  prttNew->setRefList(riferimento);
                  ptrList.vect[level]=listPtr;
                }
              else
                {
                 listPtr=new list<itemSet<NODETYPE>* >();
                 //cout<<"creo nuova LISTA "<<listPtr<<endl;
                 listPtr->push_back(prttNew);
		 riferimento=listPtr->end();
	         riferimento--;
                 prttNew->setRefList(riferimento);
       	         //cout<<"FOOOT: addr ptr list"<<*riferimento<<endl;
                 ptrList.vect.push_back(listPtr);
                }
            }
           //else cout<<"ho gia' la ItemSet"<<endl;

          //cout<<"Inserico il link da "<<prec<<" a "<<item<<endl;
          cout<<"ancestor"<<ancestor<<endl;
          levelPast=prttNew->mergeItemSetHelper(nToken,in,item,true,ptrList,ancestor);
	  //cout<<"Ancestor dopo ricorsione :"<<ancestor<<" inpart"<<getAncestor()<<endl;
	  ancestor=getAncestor();
          //cout<<"Qui avevo item "<<prec<<endl;
          //cout<<"Uscita dalla ricorsione levelPast "<<levelPast<<endl;
          //cout<<"Uscita dalla ricorsione level "<<level<<endl;
          diff=levelPast-getLevel();


          }
        else
        {
          //cout<<"diff=0"<<endl;

          StringTokenizer mytoken2((String&)nToken,":::\r\n");

          if (mytoken2.hasMoreTokens())
          {
            level=atoi(mytoken2.nextToken().c_str());
            item=(NODETYPE)mytoken2.nextToken();
          }
          //cout<<"SetitemSet Item in stesso livello "<<getLevel()<<" "<<item<<endl;
          setItemSet(item);
          //printItemSetRecursive(1);
          levelPast=mergeItemSetHelper(nToken,in,item,true,ptrList,ancestor);
	  //cout<<"Ancestor dopo ricorsione :"<<ancestor;
          //cout<<"Uscita dalla ricorsione levelPast "<<levelPast<<endl;
          //cout<<"Uscita dalla ricorsione level "<<level<<endl;
          diff=levelPast-getLevel();
          //cout<<"=0 differenza "<<diff<<endl;
        }

      }
     //cout<<"finePROCEDURA"<<levelPast<<endl;
     return levelPast;

}



template< class NODETYPE >
boolean itemSet< NODETYPE >::gen_final_count(itemSetList<NODETYPE>& ptrList,double nSup,double totGroups,int lev,itemSetHashMap<NODETYPE>& pHashMap)
{
   typename list<itemSet<NODETYPE>* >::iterator i;
   list<itemSet<NODETYPE>* >* listPtr;
   int rLev;
   boolean toContinue,okNext;
   //cout<<"creazione phashMapNew"<<endl;
   itemSetHashMap<NODETYPE> pHashMapNew;


   toContinue=true;
   cout<<"EstimateLevel: "<<lev<<endl;

   rLev=lev-1;
   if ((ptrList.vect.size()-1)>=rLev)
        listPtr=ptrList.getList(rLev);
   else toContinue=false;

//   if (toContinue) cout<<"Size lista: "<<listPtr->empty()<<endl;
//pHashMap.print();
   if (toContinue)
   {
        if (lev!=2)
        {
                toContinue=false;
                for ( i=listPtr->begin(); i!=listPtr->end(); i++)
                {
                        cout<<"lista address: "<<"*i -"<<*i<<endl;

                        okNext=((itemSet*)(*i))->gen_final_countHelper(ptrList,nSup,totGroups,lev,pHashMap,pHashMapNew);
                       // cout<<" okNext "<< okNext<<endl;
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
    list<NODETYPE> tList,t2,tListNew;
    itemSet<NODETYPE>* prttNew;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
    typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator j;
    typename list<NODETYPE>::iterator i2;
    boolean oneOK,doApriori;
    list<itemSet<NODETYPE>* >*  listPtr;
    String myAncestor;
    int level;
    map <NODETYPE,itemSetEntry< NODETYPE > > tmp;
    typename list<itemSet<NODETYPE>* >::iterator riferimento;
    NODETYPE elemToDelete;
    boolean isDelete;



    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
       cout<<"PADRE  "<<i->first<<endl;

       prttNew=i->second.getItemSet();
       cout<<"indirizzo ItemSet figlia : "<<prttNew<<endl;
       tList=i->second.getGidList();

       oneOK=false;
       // il controllo su tList non dovrebbe più servire.
       if ( (prttNew!=NULL) && (!tList.empty()) )
       {
         myAncestor=prttNew->getAncestor();
         level=prttNew->getLevel();
         //cout<<"cosa sto per analizzare"<<endl;
         //prttNew->printItemSet();
         // cout<<"fine cosa sto per analizzare"<<endl;

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

           cout<<"figlio in esame"<<j->first<<endl;
           doApriori=pHashMap.findTies(myAncestor,j->first,level);

           if (doApriori)
             {
              cout<<"tutti gli antenati ci sono dei fare merge lista"<<endl;
              t2=(prtt.find(j->first))->second.getGidList();
              //t2=j->second.getGidList();
              set_intersection(t2.begin(), t2.end(),tList.begin(), tList.end(), back_inserter(tListNew));
              //cout<<"size della lista "<<tListNew.size()<<endl;
              if (  (tListNew.size()>=nSup) && (!tListNew.empty()) )
        	{
                   prttNew->setItemSet(j->first,tListNew,totGroups);
                   String newElemHash=myAncestor+(j->first).c_str();
                   const char* it=newElemHash.c_str();
                   cout<<"Supporto Sufficiente nuovo elemnto in pHashMapNew"<<newElemHash<<endl;
                   //cout<<"elemento in hash"<<it<<"----"<<endl;
                   pHashMapNew.add(it,newElemHash);
                   oneOK=true;
                   //pHashMapNew.print();
                }
                else
                 {
                    cout<<"devo rimuovere tutto sotto "<<j->first<<" ItemSet "<<prttNew<<endl;

                    //ptrList.printAddrItemSet();
                    cout<<"ma non questo livello"<<endl;
                    if (j->second.getItemSet()!=NULL)
                       {
                        cout<<"address ItemSet figlia "<<j->second.getItemSet();
                        j->second.getItemSet()->remove(ptrList);
                        int nextLev;
                        nextLev= j->second.getItemSet()->getLevel();
                        listPtr=ptrList.getList(nextLev);
  	                cout<<"delete ItemSet dalla lista"<<j->second.getItemSet()<<endl;
  	                riferimento= j->second.getItemSet()->getRefList();
                        listPtr->erase(riferimento);


                        delete(j->second.getItemSet());
                       }
                       elemToDelete=j->first;
                       isDelete=true;

                 //cout<<"elemento  che sto a cancella "<<i->first<<endl;
                 //elemToDelete=i->first;
                 //isDelete=true;
                }
                tListNew.clear();
             }
           else
             {
               cout<<"posso fare aPriori"<<myAncestor<<" "<<j->first<<endl;
                 if (j->second.getItemSet()!=NULL)
                       {
                        cout<<"address ItemSet figlia "<<j->second.getItemSet();
                        j->second.getItemSet()->remove(ptrList);
                        int nextLev;
                        nextLev= j->second.getItemSet()->getLevel();
                        listPtr=ptrList.getList(nextLev);
  	                cout<<"delete ItemSet dalla lista"<<j->second.getItemSet()<<endl;
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
             cout<<"ultimo elem da cancellare "<<elemToDelete<<endl;
             prttNew->removeEntry(elemToDelete);
             isDelete=false;
          }

      //if (!oneOK) cout<<"posso cancellare questo livello non ho niente addrx :"<<prttNew<<endl;
      cout<<"ho finito entry "<<i->first<<" oneOK "<<oneOK<<" ItemSet a cui punto "<<prttNew<<endl;
      if (!oneOK)
         {
            cout<<"cancello prttNew"<<prttNew<<" pttrC di i "<<i->second.getItemSet()<<endl;
 	    i->second.getItemSet()->remove(ptrList);

            int nLev;
            nLev=(i->second.getItemSet()->getLevel())+1;
	    cout<<"nlev "<<nLev<<endl;
            listPtr=ptrList.getList(nLev);

            riferimento= i->second.getItemSet()->getRefList();
            listPtr->erase(riferimento);
            delete(i->second.getItemSet());
            i->second.setItemSetChild(NULL);


         }
   }
   else cout<<"non ho una ItemSet figlia"<<endl;
   i->second.gidListRemove();
  }



  cout<<"finito gen_final_count"<<endl;
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
   typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   String rule;

   rule=regola;

   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
        //cout<<"Address "<<i<<endl;
        regola=rule+(String)i->first+",";
       //cout<<"Item "<<i->first<<" GidList: ";
       //gList=i->second.getGidList();
       //for (i2=gList.begin(); i2!=gList.end();i2++)
       //    cout<<*i2<<"  ";
       //cout<<"level"<<getLevel()<<endl;
       if (lev!=1) combina2(regola);
       //  cout<<"regola"<<regola<<endl;
       if (i->second.getItemSet()!=NULL)
       {
         //cout<<"Address "<<i->second.getItemSet()<<endl;

         i->second.getItemSet()->extractRule(regola,lev+1);
         cout<<endl;
       }
     regola="";
    }

 }



template< class NODETYPE >
void itemSet< NODETYPE >::remove(itemSetList<NODETYPE>& ptrList)
 {
   typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   itemSet<NODETYPE>* p;
   list<itemSet<NODETYPE>* >* listPtr;
   int lev;
   typename list<itemSet<NODETYPE>* >::iterator riferimento;

   lev=getLevel();
   cout<<"Remove tutto lev"<<lev<<endl;
   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
         cout<<"Dentro REmove da taglio "<<i->first<<endl;
         p=i->second.getItemSet();
         if (p!=NULL)
  	    {
  	       p->remove(ptrList);
  	       listPtr=ptrList.getList(lev);
  	       cout<<"delete ItemSet dalla lista"<<p<<endl;
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
   typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   itemSet<NODETYPE>* p;
   int lev;


  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ )
     {
         p=i->second.getItemSet();
         cout<<"item"<<i->first<<endl;

  	 if (p!=NULL)
  	    {
  	       //p->removeHelper(ptrList);
  	       lev=getLevel();
     	       //ptrList.remove(lev+1,p);
  	       delete p;
            }
        prtt.erase(i);
     }
  cout<<"fine removeHelper"<<endl;
}
*/





//------------------------- disegno GRAFICAMENTE la struttura : WinGraph //
template< class NODETYPE >
void itemSet< NODETYPE >::printToDesign(int isPart)
 {

   String files,label1,values,vStruct,nod,address,idList;
   typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   typename list<NODETYPE>::iterator i2;
   list<NODETYPE> tList;
   char filename[16];



   sprintf(filename,"design/L%d.txt",isPart);

   ofstream f1(filename);

   f1<<"digraph minerule { "<<endl;
   f1<<"graph[fontsize=8]; edge  [fontsize=8]; node  [fontsize=8]; ranksep = .30; nodesep = .25;"<<endl;
   f1<<"node [fontname=\"Courier\"]; "<<endl;
   f1<<"node [shape=record];"<<endl;

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
   f1<<label1<<endl;
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

  f1<<"}"<<endl;

 }

// Segue la sintassi per creare un file testo per legare i vari nodi.
// Vedi WinGraph e programma vizuale.exe
template< class NODETYPE >
void itemSet< NODETYPE >::printToDesignHelper(String here,String padre,ofstream& f1)
 {
   typename map <NODETYPE,itemSetEntry< NODETYPE > >::iterator i;
   String files,label1,values,vStruct,nod,address,idList;
   typename list<NODETYPE>::iterator i2;
   list<NODETYPE> tList;


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
   f1<<label1<<endl;
   f1<<here<<"->"<<"struct"+padre<<endl;
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


/* ITEMSET.H */

#ifndef ITEMSET_H
#define ITEMSET_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <list>
#include <deque>
#include <algorithm>

#include "itemSetEntry.h"
#include "itemSetList.h"
#include "newItemSetHashMap.h"
#include "sqlCoreConn.h"

#include "utils.h"

using namespace std;
using namespace minerule;


template< class NODETYPE > class itemSetEntry;
template< class NODETYPE > class itemSet;

template< class NODETYPE >
class itemSet
{
  public:
  typedef typename newItemSetHashMap<NODETYPE>::HASHTYPE HASHTYPE;
  typedef map <NODETYPE,itemSetEntry< NODETYPE >, NODETYPE > MapType;


private:
  // Inner Class LevelInfoStack
  class LevelInfoStack {
    HASHTYPE ancestor;
    vector<itemSet<NODETYPE>*> levels;
    
  public:
    size_t getCurLevel() {
      // there may be up to 1 ancestor less than
      // the current level size (the first level may
      // not have any ancestor
      assert( ancestor.size() == levels.size() ||
	      ancestor.size() == levels.size()-1 );

      return levels.size();
    }
    
    void
      push_back( NODETYPE newAncestor, itemSet<NODETYPE>* newISC ) {
      ancestor.push_back(newAncestor);
      levels.push_back(newISC);
    }

    // This method adds a new ISC without adding the conresponding ancestor
    // it is intended to be used only for the first level, i.e. when there
    // is no a valid ancestor conresponding to the level being inserted.
    void
      push_back( itemSet<NODETYPE>* newISC ) {
      levels.push_back(newISC);
    }
    
    const HASHTYPE& 
      getAncestors() {
      return ancestor;
    }
    
    itemSet<NODETYPE>*
      getCurLevelInfo() {
      if(getCurLevel()==0)
	return NULL;
      else
	return levels.back();
    }
    
    void
      pop_back() {
      levels.pop_back();
      if( ancestor.size()>0 )
	ancestor.pop_back();
    }
  }; // fine class LevelInfoStack

 public:

  
// Costruttore
    itemSet() {   numLevel=1; }
    itemSet(int lev) { numLevel=lev; }

    void setLevel(int lev) { numLevel=lev;}

    int getLevel()  { return numLevel;}
    void setAncestor(const HASHTYPE& anc) {myAncestor=anc;}
    const HASHTYPE& getAncestor() {return myAncestor;}

    void setRefList(typename list<itemSet<NODETYPE>* >::iterator riferimento) {refList=riferimento;}
    typename list<itemSet<NODETYPE>* >::iterator getRefList()  {return refList;}
    void setItemSet(NODETYPE value, int gidList);
    void setItemSetIfExist(NODETYPE value,int gidList);
    void setItemSet(NODETYPE value,const set<int>& gidList);
    void setItemSet(NODETYPE value,const set<int>& gidList,double totGroups);
    void setItemSet(NODETYPE value);
    void printItemSet();
    void printToDesign(int isPart);
    void printItemSetRecursive(int lev);
    void save_Large_ItemSet( int lev, int isPart,sqlCoreConn& coreConn);


    void cancellaRef(list<itemSet<NODETYPE>* >* listPtr);
// Ricorsiva che scende gi� in modo ricorsivo.
// Non si possono cancallare le gid immediatamente
    void createNewItemSet(double,double,int);

    bool 
      gen_Large_ItemSet(itemSetList<NODETYPE>& ptrList,double nSup,int lev);
    bool 
      gen_final_count(itemSetList<NODETYPE>& ,double,int,newItemSetHashMap<NODETYPE>&);

    void gen_Large_ItemSet_Base(double);

    void checkSupportBaseDef(double, newItemSetHashMap<NODETYPE>&,itemSetList<NODETYPE>& );
    void mergeItemSet(sqlCoreConn& coreConn,
		      itemSetList<NODETYPE>& ptrList,
		      const SourceRowDescriptor& srd);
    void remove(itemSetList<NODETYPE>& ptrList);
    bool existEntry(NODETYPE item);
    void extractRule(sqlCoreConn& coreConn, double totGroups, itemSet<NODETYPE>& root);

    void removeEntry(NODETYPE elem);
    MapType& getMap() {return prtt;}

  private:
    int numLevel;
// Root
    MapType prtt;
    HASHTYPE myAncestor;
    typename list<itemSet<NODETYPE>* >::iterator refList;

//  vector<list<itemSet<String>* > > vectLevel;

    void deleteElemsFromPrtt(vector<NODETYPE>& elVec);
    void printToDesignHelper(string here,string padre,ofstream& f1);
    void saveIn_DB_Body( int lev, int isPart, NODETYPE node,sqlCoreConn& coreConn);
    void save_Large_ItemSetHelper(int lev,int isPart,sqlCoreConn& coreConn);

    void mergeItemSetHelper(
			    itemSetList<NODETYPE>& ptrList, 
			    odbc::ResultSet* resultAllBody,
			    // The following parameters describe the data set
			    const HeadBodySourceRowDescription& srDescription,
			    size_t lastElem,
			    // The following parameters will be passed to the
			    // mergeItemSetHead function
			    sqlCoreConn& coreConn,
			    const SourceRowDescriptor& srd);

    void updatePtrList(				     
		     itemSetList<NODETYPE>& ptrList,
		     unsigned int level,
		     itemSet<NODETYPE>* newLevel);

    bool gen_Large_ItemSetHelper(itemSetList<NODETYPE>& ptrList,double nSup,unsigned int lev);

    bool gen_final_count_Helper(
				   newItemSetHashMap<NODETYPE>& pHashMap,
				   newItemSetHashMap<NODETYPE>& pHashMapNew,
				   itemSetList<NODETYPE>& ptrList,
				   double nSup);

    bool prune_children(itemSetEntry<NODETYPE>& toBePruned,
			   newItemSetHashMap<NODETYPE>& pHashMap,
			   newItemSetHashMap<NODETYPE>& pHashMapNew,
			   double nSup,
			   itemSetList<NODETYPE>& ptrList );

    void 
      removeAllReferences(itemSetEntry<NODETYPE>& delElem,
			  itemSetList<NODETYPE>& ptrList );
    double 
      getSupportForItemSet(deque<NODETYPE>& itset);


    void extractRuleHelper( sqlCoreConn& coreConn, 
			    HASHTYPE& curElem,
			    double myGroups,
			    double totGroups,
			    itemSet<NODETYPE>& root);
    typename itemSet<NODETYPE>::MapType::iterator 
      getLink(NODETYPE item );

//void removeHelper(itemSetList<NODETYPE>& ptrList);
};

/* IMPLEMENTAZIONE DEI METODI DI PARTITION.H */

template< class NODETYPE >
bool itemSet< NODETYPE >::existEntry(NODETYPE item ) {

// cout<<"gidList "<<gidList<<" Item "<<value<<endl;
  if (prtt.find(item)==prtt.end())
    return false;
  else return true;

}


template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,int gidList ) {
  itemSetEntry< NODETYPE > entry;
  typename MapType::iterator i=prtt.find(value);
  if (i==prtt.end()) {
    entry.setData(value);
    entry.setGidList(gidList);
    prtt[value]=entry;
  }
  else {
    i->second.setGidList(gidList);
  }
}


template< class NODETYPE >
void itemSet< NODETYPE >::setItemSetIfExist(NODETYPE value,int gidList ) {
  itemSetEntry< NODETYPE > entry;
  typename MapType::iterator i=prtt.find(value);
  if (i!=prtt.end()) {
    i->second.setGidList(gidList);
  }
}


template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,const set<int>& gidList ) {
  itemSetEntry< NODETYPE > entry;
  typename MapType::iterator i=prtt.find(value);
  if (i==prtt.end()) {
    entry.setData(value);
    entry.setGidList(gidList);
    prtt[value]=entry;
  }
  else {
    i->second.setGidList(gidList);
  }
}




template< class NODETYPE >
void itemSet< NODETYPE >::setItemSet(NODETYPE value,const set<int>& gidList,double totGroups ) {
  itemSetEntry< NODETYPE > entry;
  typename MapType::iterator i=prtt.find(value);

  if (i==prtt.end()) {
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
void itemSet< NODETYPE >::setItemSet(NODETYPE value ) {
  itemSetEntry< NODETYPE > entry;
  typename MapType::iterator i=prtt.find(value);

  if (i==prtt.end()) {
    entry.setData(value);
    prtt[value]=entry;
  }
}


template< class NODETYPE >
void itemSet< NODETYPE >::printItemSet() {
  set<int> gList;
  typename MapType::iterator i;
  typename set<int>::iterator i2;
  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
        cout<<"Item "<<i->first<<"GidList: ";
    gList=i->second.getGidList();
    for (i2=gList.begin(); i2!=gList.end();i2++)
      cout<<*i2<<"  ";

    cout<<endl;
  }

}


template< class NODETYPE >
void itemSet< NODETYPE >::printItemSetRecursive(int lev) {
  set<int> gList;
  typename MapType::iterator i;
  typename set<int>::iterator i2;

//cout<<"dentro print"<<endl;
  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
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
    if (i->second.getItemSet()!=NULL) {
//cout<<"Address "<<i->second.getItemSet()<<endl;
      i->second.getItemSet()->printItemSetRecursive(lev+1);
    }
  }
//cout<<"fine print"<<endl;
}


template< class NODETYPE >
void itemSet< NODETYPE >::createNewItemSet(double nSup,double
totGroups,int lev) {
  set<int> tList,t2,tListNew;
  itemSet<NODETYPE>* prttNew;
  typename MapType::iterator i;
  typename MapType::iterator j;
  typename set<int>::iterator i2;
  bool toLink;

  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
// cout<<"PADRE!!!!  "<<i->first<<endl;
    j=i;
    j++;
    prttNew=new itemSet(lev);
    toLink=false;
    tList=i->second.getGidList();

    while (j!=prtt.end()) {
// cout<<"figlio generato "<<j->first;
      t2=j->second.getGidList();

      set_intersection(t2.begin(), t2.end(),tList.begin(), tList.end(), inserter(tListNew,tListNew.begin()));
// cout<<"intersezione... "<<endl;
// for (i2=tListNew.begin(); i2!=tListNew.end();i2++)
//     cout<<*i2<<"  ";
// cout<<endl;
// cout<<"ListNEW!!! "<<tListNew<<endl;

//cout<<"size list"<<tListNew.size()<<endl;
      if (  (tListNew.size()>=nSup) && (!tListNew.empty()) ) {
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
    if (toLink) {
      i->second.setItemSetChild(prttNew);

      prttNew->createNewItemSet(nSup,totGroups,lev+1);
    }
    else delete prttNew;
// cout<<".."<<endl;
  }

}


// Crea un livello

template< class NODETYPE >
bool itemSet< NODETYPE >::gen_Large_ItemSet(
			      itemSetList<NODETYPE>& ptrList,
			      double nSup,
			      int lev) {
  //  itemSet<NODETYPE>* prttNew;
  typename list<itemSet<NODETYPE>* >::iterator i;
  list<itemSet<NODETYPE>* >* listPtr;
  unsigned int rLev;
  bool otherItemSet,toContinue;


  toContinue=false;
  if (lev!=2) {
    rLev=lev-1;
    if ((ptrList.vect.size()-1)<=rLev) {
      listPtr=ptrList.getList(rLev);
      
      for ( i=listPtr->begin(); i!=listPtr->end(); i++) {
	otherItemSet=(*i)->gen_Large_ItemSetHelper(ptrList,nSup,lev);
	if (otherItemSet) 
	  toContinue=true;
      }
    }
  }
  else toContinue=gen_Large_ItemSetHelper(ptrList,nSup,lev);

  return toContinue;
}


template< class NODETYPE >
bool itemSet< NODETYPE >::gen_Large_ItemSetHelper(itemSetList<NODETYPE>& ptrList,double nSup,unsigned int lev) {
  set<int> tList,t2,tListNew;
  itemSet<NODETYPE>* prttNew;
  typename MapType::iterator i;
  typename MapType::iterator j;
  //  typename list<NODETYPE>::iterator i2;
  bool toLink,otherPart;
  list<itemSet<NODETYPE>* >*  listPtr;

  otherPart=false;

  if ((ptrList.vect.size()-1)>=lev)
    listPtr=ptrList.getList(lev);
  else 
    listPtr=new list<itemSet<NODETYPE>* >();

  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
    j=i;
    j++;
    prttNew=new itemSet(lev);
    toLink=false;
    tList=i->second.getGidList();

    while (j!=prtt.end()) {
      t2=j->second.getGidList();

      set_intersection(t2.begin(), 
		       t2.end(),
		       tList.begin(), 
		       tList.end(), 
		       inserter(tListNew,tListNew.begin())
		       );
      if(tListNew.size()>=nSup) {
        prttNew->setItemSet(j->first,tListNew);
        toLink=true;
      }
      tListNew.clear();
      j++;
    } // end while

    i->second.gidListRemove();
    if (toLink) {
      i->second.setItemSetChild(prttNew);
      listPtr->push_back(prttNew);
    }
    else delete prttNew;
  } // end for


  if (listPtr->size()>0) {
    otherPart=true;
    if ((ptrList.vect.size()-1)>=lev)
      ptrList.vect[lev]=listPtr;
    else ptrList.vect.push_back(listPtr);

  }

  return otherPart;
}


template<class NODETYPE>
void itemSet< NODETYPE >::deleteElemsFromPrtt(vector<NODETYPE>& elVec) {
  typename vector<NODETYPE>::iterator it;
  for(it=elVec.begin(); it!=elVec.end(); it++) {
    prtt.erase(*it);
  }
}

template< class NODETYPE >
void itemSet< NODETYPE >::gen_Large_ItemSet_Base(double nSup) {
  typename MapType::iterator i;
  NODETYPE elemToDelete;
  vector<NODETYPE> elemsToBeDeleted;

  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
    if (i->second.getGidList().size() < nSup) {
      elemsToBeDeleted.push_back(i->first);
    }
  }

  deleteElemsFromPrtt(elemsToBeDeleted);
}


template< class NODETYPE >
void itemSet< NODETYPE >::cancellaRef( list<itemSet<NODETYPE>* >* listPtr) {
  typename list<itemSet<NODETYPE>* >::iterator riferimento;
  //  cout<<"opa"<<endl;

  riferimento=getRefList();
  //  cout<<"refList"<<*riferimento<<endl;
  listPtr->erase(riferimento);
}


template< class NODETYPE >
void 
itemSet<NODETYPE>::removeAllReferences(
		itemSetEntry<NODETYPE>& delElem,
		itemSetList<NODETYPE>& ptrList
		) {
  itemSet<NODETYPE>* prttChild = delElem.getItemSet();
  if( prttChild!=NULL ) {
    int nlev = prttChild->getLevel();  
    list<itemSet<NODETYPE>* >* listPtr=ptrList.getList(nlev);;
    assert( listPtr!=NULL );

    prttChild->remove(ptrList);
    prttChild->cancellaRef(listPtr);
    delete (prttChild);
    delElem.setItemSetChild(NULL);
  }
 }

template< class NODETYPE >
void itemSet< NODETYPE >::checkSupportBaseDef(
					      double nSup,
					      newItemSetHashMap<NODETYPE>& pHashMap,
					      itemSetList<NODETYPE>& ptrList) {

  //  itemSet<NODETYPE>* prttNew;
  typename MapType::iterator i;
  //  int nlev;
  //  list<itemSet<NODETYPE>* >* listPtr;
  vector<NODETYPE> elemsToBeDeleted;

  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
    if (i->second.getGidList().size() < nSup) {
      removeAllReferences( i->second, ptrList );
      elemsToBeDeleted.push_back(i->first);    
    }
    else {
      i->second.setCountGid(i->second.numDistinctGID());
      vector<NODETYPE> tmp;
      tmp.push_back(i->first);
      pHashMap.add(tmp);
    }
  }

  deleteElemsFromPrtt(elemsToBeDeleted);
}


template< class NODETYPE >
void itemSet< NODETYPE >::save_Large_ItemSet(
				 int lev,
				 int isPart,
				 sqlCoreConn& coreConn)  {
    typename MapType::iterator i;

    for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
       saveIn_DB_Body(lev,isPart,i->first,coreConn);

       if (i->second.getItemSet()!=NULL) {
	 i->second.getItemSet()->save_Large_ItemSetHelper(lev+1,isPart,coreConn);
	 i->second.releaseItemSet();
       }
    }

    prtt.clear();
 }

template< class NODETYPE >
void itemSet< NODETYPE >::save_Large_ItemSetHelper(int lev,int isPart,sqlCoreConn& coreConn)
 {
    typename MapType::iterator i;
    
     for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
       saveIn_DB_Body(lev,isPart,i->first,coreConn);
       if (i->second.getItemSet()!=NULL)
	 {
	   i->second.getItemSet()->save_Large_ItemSetHelper(lev+1,isPart,coreConn);
	   //Quando esco dealloco il puntatore
	   i->second.releaseItemSet();
	 }
     }
 }

template< class NODETYPE >
void itemSet< NODETYPE >::saveIn_DB_Body(
			     int lev,
			     int isPart,
			     NODETYPE node,
			     sqlCoreConn& coreConn)
 {
    char inter[10],intPart[10];
    //const char* bid;
    std::string Qry;
    std::string QryDef;


    Qry="INSERT INTO tmp_Rule_Base VALUES (0,";
    sprintf(inter,"%d",lev);
    sprintf(intPart,"%d",isPart);
    QryDef=Qry+inter+","+node.getSQLData()+");";
    const char * dains=QryDef.c_str();
    coreConn.insert_DB(dains);
    //cout << ".";
}

template< class NODETYPE >
void itemSet< NODETYPE >::mergeItemSet(
				     sqlCoreConn& coreConn,
				     itemSetList<NODETYPE>& ptrList,
				     const SourceRowDescriptor& srd) {
   std::string Qry;
   HASHTYPE anc;
   NODETYPE temp;
   int level;
   odbc::Statement* statement=coreConn.getConnection()->createStatement();
   odbc::ResultSet* resultAllBody;
   level=0;
   size_t lastElem;

   // Il db del minerule � connesso!

   Qry="select level,"
     +srd.getBody().getSQLColumnNames()+" from tmp_Rule_Base order by id;";

   //   cerr << endl << "QRY:" << Qry << endl;

   HeadBodySourceRowDescription srDescription;
   lastElem = srDescription.setBodyElems(2,srd.getBody().getColumnsCount());

   resultAllBody=statement->executeQuery(Qry.c_str());
   //cout<<"dopo resultAllBody"<<endl;
   if (resultAllBody!=NULL) {
     mergeItemSetHelper(ptrList,
			resultAllBody,
			srDescription,
			lastElem,
			coreConn,
			srd);
     delete resultAllBody;
   } else cout<<"ResultAllBody == NULL"<<endl;
   delete statement;
}

template< class NODETYPE >
typename itemSet<NODETYPE>::MapType::iterator 
itemSet< NODETYPE >::getLink(NODETYPE item )
 {
    return prtt.find(item);
 }

template< class NODETYPE >
void itemSet< NODETYPE >::mergeItemSetHelper(
			itemSetList<NODETYPE>& ptrList, 
			odbc::ResultSet* resultAllBody,
			// The following parameters describe the data set
			const HeadBodySourceRowDescription& srDescription,
			size_t lastElem,
			// The following parameters will be passed to the
			// mergeItemSetHead function
			sqlCoreConn& coreConn,
			const SourceRowDescriptor& srd) {
  LevelInfoStack levelStack;
  levelStack.push_back(this);

  int lastLevel = 0;
  NODETYPE lastItem; 

  unsigned int level;
  NODETYPE item;
  
  while( resultAllBody->next() ) {
    
    // lettura dal db
    HeadBodySourceRow srow(resultAllBody, srDescription);

    lastLevel = level;
    level = atoi(resultAllBody->getString(1).c_str());

    lastItem = item;
    item = srow.getBody();

    while( level<levelStack.getCurLevel() ) {
      // torno indietro
      levelStack.pop_back();
    }
    
    if( level > levelStack.getCurLevel() ) {
      assert( level == levelStack.getCurLevel()+1 );
      typename MapType::iterator parentItem;
      //      assert( levelStack.getAncestors().size() > 0 );
            
      parentItem=levelStack.getCurLevelInfo()->getLink(lastItem);
      assert(parentItem!=levelStack.getCurLevelInfo()->getMap().end());
      itemSet<NODETYPE>* newLevel = parentItem->second.getItemSet();
      if( newLevel == NULL ) {
	// there are no child items for parentItem... I have to build up
	// the new level
	
	newLevel=new itemSet();
	levelStack.push_back(lastItem, newLevel);
	newLevel->setAncestor(levelStack.getAncestors());
	newLevel->setLevel(level);
	parentItem->second.setItemSetChild(newLevel);

	// updating the ptrList ...
	updatePtrList(ptrList, level, newLevel);
      } else {
	levelStack.push_back(lastItem, newLevel);
      }
    } // level > levelStack...
    
    assert(level==levelStack.getCurLevel());
    levelStack.getCurLevelInfo()->setItemSet(item);
  }
}


template< class NODETYPE >
void itemSet< NODETYPE >::updatePtrList(				     
				     itemSetList<NODETYPE>& ptrList,
				     unsigned int level,
				     itemSet<NODETYPE>* newLevel) {
  list<itemSet<NODETYPE>* >*  listPtr;
  typename list<itemSet<NODETYPE>* >::iterator riferimento;

  if ((ptrList.vect.size()-1)>=level) {
    // esiste la lista di livello level
    listPtr=ptrList.getList(level);
    listPtr->push_back(newLevel);
    ptrList.vect[level]=listPtr;
  } else {
    // Non c'e' una lista di livello level!
    listPtr=new list<itemSet<NODETYPE>* >();
    listPtr->push_back(newLevel);
    ptrList.vect.push_back(listPtr);
  }

  riferimento=listPtr->end();
  riferimento--;
  newLevel->setRefList(riferimento);
}


template< class NODETYPE >
bool itemSet< NODETYPE >::gen_final_count(
                itemSetList<NODETYPE>& ptrList,
		double nSup,
		int lev,
		newItemSetHashMap<NODETYPE>& pHashMap) {

   typename list<itemSet<NODETYPE>* >::iterator i;
   list<itemSet<NODETYPE>* >* listPtr;
   unsigned int rLev;
   newItemSetHashMap<NODETYPE> pHashMapNew;


   rLev=lev-1;
   if ((ptrList.vect.size()-1)>=rLev)
     listPtr=ptrList.getList(rLev);
   else 
     return false;

   if (lev!=2) {
     for ( i=listPtr->begin(); i!=listPtr->end(); i++) {
       (*i)->gen_final_count_Helper(  				
				    pHashMap,
				    pHashMapNew,
				    ptrList,
				    nSup);
     }
   } else 
     gen_final_count_Helper(
				       pHashMap,
				       pHashMapNew,
				       ptrList,
				       nSup);
   

   pHashMap=pHashMapNew;
   return true;
}


template< class NODETYPE >
bool itemSet< NODETYPE >::gen_final_count_Helper(
	      newItemSetHashMap<NODETYPE>& pHashMap,
	      newItemSetHashMap<NODETYPE>& pHashMapNew,
              itemSetList<NODETYPE>& ptrList,
	      double nSup) {
   typename MapType::iterator i;

   vector<NODETYPE> elemsToBeDeleted;

   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
     prune_children(i->second, pHashMap, pHashMapNew, nSup, ptrList );
   }

   return true;
 }

template< class NODETYPE >
bool itemSet< NODETYPE >::prune_children(
						itemSetEntry<NODETYPE>& toBePruned,
						newItemSetHashMap<NODETYPE>& pHashMap,
						newItemSetHashMap<NODETYPE>& pHashMapNew,
						double nSup,
						itemSetList<NODETYPE>& ptrList
				   ) {
  set<int>& tList = toBePruned.getGidList();
  if( toBePruned.getItemSet()==NULL ) {
    return false;
  }

  itemSet<NODETYPE>* prttNew  = toBePruned.getItemSet();
  HASHTYPE myAncestor=prttNew->getAncestor();
  int level=prttNew->getLevel();
  MapType& childMap=prttNew->getMap(); // 
  bool oneOK = false;
  bool doApriori;

  vector<NODETYPE> elemsToBeDeleted;
  typename MapType::iterator j;
  for( j=childMap.begin(); j !=childMap.end( ) ; j++ )  {
      doApriori=pHashMap.findTies(myAncestor,j->first,level);

      if (doApriori) {
	  set<int> tListNew;

	  assert( prtt.find(j->first)!=prtt.end() );	  
	  set<int>& tParentList=(prtt.find(j->first))->second.getGidList();

	  set_intersection(tList.begin(),tList.end(),
			   tParentList.begin(), tParentList.end(),
			   inserter(tListNew,tListNew.begin()));

	  if (  tListNew.size() >= nSup  ) { 
	    // tutti i miei antenati hanno supporto sufficiente (infatti doApriori
	    // e' true) ed io ho supporto sufficiente (infatti tListNew.isBigger(nSup))
	    // :-)
	    prttNew->setItemSet(j->first,tListNew);
	    HASHTYPE ancestorCopy = myAncestor;
	    ancestorCopy.push_back(j->first);
	    pHashMapNew.add(ancestorCopy);
	    oneOK=true;
	  } else {
	    // io non  ho supporto sufficiente sebbene i miei antenati lo abbiano
	    //ptrList.printAddrItemSet();
	    removeAllReferences(j->second, ptrList);
	    elemsToBeDeleted.push_back(j->first);
	    }
	} else 	{
	  // Non e' vero che tutti i miei ancestors hanno supporto sufficiente.
	  // non c'e' speranza che io possa averlo 
	  //	  cout << "CANCELLO:" << j->first << endl;
	  removeAllReferences(j->second, ptrList);
	  elemsToBeDeleted.push_back(j->first);
	}
    } // fine for

  // rimuoviamo fisicamente gli elementi dalla mappa.
  // N.B. Non puo' essere fatto prima perche' altrimenti l'iteratore "i"
  // verrebbe invalidato.

  typename vector<NODETYPE>::iterator it;
  for(it=elemsToBeDeleted.begin(); it!=elemsToBeDeleted.end(); it++) {
    childMap.erase(*it);
  }

  // posso cancellare questo livello, non ci sono item a supporto sufficiente
  if (!oneOK) {
    // Il codice seguente corrisponde a:
    // removeAllReferences(i->second, ptrList,false)
    // tranne per il fatto che removeAllReferences rimuove un riferimento
    // a livello  nLev=(toBePruned.getItemSet()->getLevel()), mentre il
    // codice sottostante lo fa per nLev=(i->second.getItemSet()->getLevel())+1
    
    assert( toBePruned.getItemSet()!=NULL );
    
    
    int nLev=(toBePruned.getItemSet()->getLevel())+1;
    list< itemSet<NODETYPE>* >* listPtr=ptrList.getList(nLev);
    
    toBePruned.getItemSet()->remove(ptrList);
    toBePruned.getItemSet()->cancellaRef(listPtr);
    delete(toBePruned.getItemSet());
    toBePruned.setItemSetChild(NULL);
  }

  return !oneOK;
}


template< class NODETYPE >
void itemSet< NODETYPE >::removeEntry(NODETYPE elem) {
  prtt.erase(elem);
}


template< class NODETYPE >
void itemSet< NODETYPE >::extractRule(sqlCoreConn& coreConn, double totGroups, itemSet<NODETYPE>& root)
 {
   typename MapType::iterator i;

   for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
     HASHTYPE curElems = getAncestor();
     curElems.push_back(i->first);

     extractRuleHelper(coreConn, curElems, i->second.getCountGid(), totGroups,root);

     if (i->second.getItemSet()!=NULL){
       i->second.getItemSet()->extractRule(coreConn, totGroups, root);
     }
   }
 }


// N.B.: For perfomance reasons this procedure modifies its parameter
//  and should therefore be called only on copies of the actual parameter
//  Assumptions: 
// 1) itset contain of NODETYPE which can be reached from the
//    current itemSet<>. 
// 2) The sorting of those element is the natural one, i.e.:
//    elements that come first are the ones nearest to the root.
// 3) The element represents a valid path in the current tree, i.e.,
//    the first element must appear in the root object (i.e. prtt->find(),
//    must not fail), the following one must appear in the object found
//    by prtt->find(previous_elem)->prtt->find(curElem), and so on. 

template< class NODETYPE >
double 
itemSet< NODETYPE >::getSupportForItemSet(deque<NODETYPE>& itset) {
  NODETYPE cur = itset.front();
  itset.pop_front();
  typename MapType::iterator curIt = prtt.find(cur);
  assert(curIt!=prtt.end());

  if( itset.size()==0 ) {
    return curIt->second.numDistinctGID();
  } else {
    return curIt->second.getItemSet()->getSupportForItemSet(itset);
  }
}

template< class NODETYPE > 
void itemSet< NODETYPE >::extractRuleHelper( sqlCoreConn& coreConn, 
					     HASHTYPE& curElem,
					     double myGroups,
					     double totGroups,
					     itemSet<NODETYPE>& root) {
  float cSupp = myGroups / totGroups;
  
  assert( curElem.size()<=32 );
  // N.B. : maxVal = 2^curElem.size() -1
  unsigned int maxVal = (1 << curElem.size())-1;

#if 0 // debug
 // stampa ancestor
  string values;
  typename vector<NODETYPE>::const_iterator it=curElem.begin();
  for(;it!=curElem.end(); it++) {
    values += std::string(it->c_str())+"-";
  }
  cout << "extract..Helper: curElem:" << values << endl;
  cout << "                 maxVal:" << maxVal << endl;
  //end stampa ancestor
#endif

  for(unsigned int i=1; i<maxVal; i++ ) {
    HASHTYPE body;
    deque<NODETYPE> bodyCopy;
    HASHTYPE head;

    // Here we build the current rule's head and body elements
    for(unsigned int j=0; j<curElem.size(); j++) {
      if(i&(1<<j)) {
	body.push_back(curElem[j]);
	bodyCopy.push_back(curElem[j]); 
      }
      else {
	head.push_back(curElem[j]);
      }
    }

#if 0 // debug
#warning debug
    cout << "    i==" <<  i << " body: ";
    string values;
    typename vector<NODETYPE>::const_iterator it2=body.begin();
    for(;it2!=body.end(); it2++) {
      values += std::string(it2->c_str())+"-";
    }
    cout << values << endl;

    cout << "    i==" <<  i << " head: ";
    values="";
    it2=head.begin();
    for(;it2!=head.end(); it2++) {
      values += std::string(it2->c_str())+"-";
    }
    cout << values << endl;
// end warning
#endif
  
    float  cConf = myGroups/root.getSupportForItemSet(bodyCopy); 
    coreConn.insert_DB(body,head,cSupp,cConf);

    /*
    // Building up the string representation of the generated rule...
    // Body part...
    typename vector<NODETYPE>::iterator it;
    string regola;

    it=body.begin();
    if(it!=body.end()) {
      regola=string("<")+it->c_str()+">";
      it++;
    }

    for(; it!=body.end(); it++ ) {
      regola+=string(",")+"<"+it->c_str()+">";
    }

    // a nice looking -> ;-)
    regola+=" => ";

    // Head part...
    it=head.begin();
    if(it!=head.end()) {
      regola+=string("<")+it->c_str()+">";
      it++;
    }

    for(; it!=head.end(); it++ ) {
      regola+=string(",")+"<"+it->c_str()+">";
    }

    // Inserting the new rule into the db.

    stringstream strbuf;
    strbuf << "INSERT INTO "+coreConn.getOutTableName()+" VALUES ('"
	   <<regola<<"',"
	   <<cSupp<<","
	   <<cConf<<");";
    const char * dains=strbuf.str().c_str();
    coreConn.insert_DB(dains);*/
  }
}


template< class NODETYPE >
void itemSet< NODETYPE >::remove(itemSetList<NODETYPE>& ptrList) {
  typename MapType::iterator i;
  itemSet<NODETYPE>* p;
  list<itemSet<NODETYPE>* >* listPtr;
  int lev;
  typename list<itemSet<NODETYPE>* >::iterator riferimento;

  lev=getLevel();
  //  cout<<"Remove tutto lev"<<lev<<endl;
  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
    //    cout<<"Dentro REmove da taglio "<<i->first<<endl;
    p=i->second.getItemSet();
    if (p!=NULL) {
      p->remove(ptrList);
      listPtr=ptrList.getList(lev);
      //      cout<<"delete ItemSet dalla lista"<<p<<endl;
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
   typename MapType::iterator i;
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
void itemSet< NODETYPE >::printToDesign(int isPart) {

  string files,label1,values,vStruct,nod,address,idList;
  typename MapType::iterator i;
  typename set<int>::iterator i2;
  set<int> tList;
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
  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
    idList="";
    values=values+"<"+i->first.c_str()+">"+i->first.c_str();

    // #define PRINTGID

    #ifdef PRINTGID
    tList=i->second.getGidList();
    for (i2=tList.begin(); i2!=tList.end();i2++) {
      std::stringstream o2;
      o2 << i2->c_str();
      address=o2.str();
      idList=idList+address+"-";
    }
    #endif

    #ifndef PRINTGID
    // stampa ancestor
    typename vector<NODETYPE>::const_iterator it=getAncestor().begin();
    for(;it!=getAncestor().end(); it++) {
      values += std::string(it->c_str())+"-";
    }
    //end stampa ancestor
    #endif 
    
    values=values+":  "+idList+"|";
  }

  int siz=values.size();
  siz=siz-1;
  if (siz>0)  values.resize(siz);
  label1=label1+values+"\"];";
  f1<<label1<<endl;
  values="";

  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {

    nod=i->first.c_str();

    if ((i->second.getItemSet())!=NULL) {

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
void itemSet< NODETYPE >::printToDesignHelper(string here,string padre,ofstream& f1) {
  typename MapType::iterator i;
  string files,label1,values,vStruct,nod,address,idList;
  typename set<int>::iterator i2;
  set<int> tList;

  label1="struct"+padre+"[shape=record,label=\""+padre+"|";
  values="";

  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {
    idList="";
    values=values+"<"+i->first.c_str()+">"+i->first.c_str();
    
    #ifdef PRINTGID
    tList=i->second.getGidList();
    for (i2=tList.begin(); i2!=tList.end();i2++) {
      std::stringstream o2;
      o2 << i2;
      address=o2.str();
      idList=idList+address+"-";
    }
    #endif

    #ifndef PRINTGID
    // stampa ancestor
    typename vector<NODETYPE>::const_iterator it=getAncestor().begin();
    for(;it!=getAncestor().end(); it++) {
      values += std::string(it->c_str())+"-";
    }
    //end stampa ancestor
    #endif

//string myAncestor2=myAncestor;
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
  for( i=prtt.begin( ) ; i != prtt.end( ) ; i++ ) {

    nod=i->first.c_str();

    if (i->second.getItemSet()!=NULL) {
      std::stringstream o2;
      o2 <<  i->second.getItemSet();
      address=o2.str();
      nod="struct"+padre+":"+nod;

      i->second.getItemSet()->printToDesignHelper(nod,address,f1);
    }

  }

}
#endif
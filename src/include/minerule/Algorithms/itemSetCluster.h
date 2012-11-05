#ifndef ITEMSETCLUSTER_H
#define ITEMSETCLUSTER_H

#include <map>
#include <list>

#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

#include "itemSetEntryCluster.h"
#include "itemSetListCluster.h"
#include "newItemSetHashMap.h"
using namespace minerule;

#include "sqlCoreConn.h"


#include "MRDatabase/sourcerow.h"
#include "MRDatabase/sourcerowdescriptor.h"




#if 0

vector<string> DBGVEC;
#define DBG_PUSH(str)  DBGCLS dbgclsinstance(str)

class DBGCLS {
 public:
  DBGCLS(string str) {
    cout << "Inizio di:" << str<<endl;
    DBGVEC.push_back(str);
  }

  ~DBGCLS() {
    cout << "Fine di:" << DBGVEC.back()<<endl;
    DBGVEC.pop_back();
  }
};

inline void
DBG_PRINT_STACK() {
  cout << "stack trace:";
  copy( DBGVEC.begin(), DBGVEC.end(), ostream_iterator<string>(cout,"::"));
  cout << endl;
}
#endif



using namespace std;
using namespace minerule;


template< class NODETYPE > class itemSetClusterEntry;
template< class NODETYPE > class itemSetCluster;


template< class NODETYPE >
class itemSetCluster {
 public:
  typedef map <NODETYPE,itemSetEntryCluster< NODETYPE >, NODETYPE > MapType;
  typedef typename newItemSetHashMap<NODETYPE>::HASHTYPE HASHTYPE;

 private:
  // Inner Class LevelInfoStack
  class LevelInfoStack {
    HASHTYPE ancestor;
    vector<itemSetCluster<NODETYPE>*> levels;
    
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
      push_back( const NODETYPE& newAncestor, itemSetCluster<NODETYPE>* newISC ) {
      ancestor.push_back(newAncestor);
      levels.push_back(newISC);
    }

    // This method adds a new ISC without adding the conresponding ancestor
    // it is intended to be used only for the first level, i.e. when there
    // is no a valid ancestor conresponding to the level being inserted.
    void
      push_back( itemSetCluster<NODETYPE>* newISC ) {
      levels.push_back(newISC);
    }
    
    const HASHTYPE& 
      getAncestors() {
      return ancestor;
    }
    
    itemSetCluster<NODETYPE>*
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
  
  size_t size() const {
    return isc.size();
  }

  
  itemSetCluster() { numLevel=1;  }
  itemSetCluster(int lev) { numLevel=lev; }
  void setAncestor(const HASHTYPE& anc) {myAncestor=anc;}
  const HASHTYPE& getAncestor() {return myAncestor;}
  void setRefList(typename list<itemSetCluster<NODETYPE>* >::iterator riferimento) {refList=riferimento;}
  typename list<itemSetCluster<NODETYPE>* >::iterator getRefList()  {return refList;}
  void setLevel(int lev) { numLevel=lev;}
  int getLevel()  { return numLevel;}
  MapType& getMap() {return isc;}
  bool existEntry(const NODETYPE& item);
  void setItemSet(const NODETYPE& value,const int gid,const int cid);
  
  void setItemSet(const NODETYPE& valueB, 
		  const int gidB,
		  const int cidB, 
		  const NODETYPE& valueH, 
		  const int cidH);
  
  void setItemSet(const NODETYPE& value,const gidCidList<int>& gidCid);
  void setItemSet(const NODETYPE& value);
  void setItemSetIfExist(const NODETYPE& valueB,
			 const int gidB,
			 const int cidB, 
			 const NODETYPE& valueH, 
			 const int cidH);
  void save_Large_ItemSet(int lev,int isPart,sqlCoreConn&);
  // modificato da esposito@di.unito.it -- la procedura qui sotto
  // restituiva un const char* allocato localmente alla procedura stessa...
  // nell'uscire dalla procedura, la variabile da cui il const char* era
  // stato preso veniva deallocata con le ovvie conseguenze...
  //   const char* save_Large_ItemSet_Head(int lev,int isPart,sqlCoreConn&);
  std::string save_Large_ItemSet_Head(int lev,int isPart,sqlCoreConn&);
  void printItemSetRecursive(int lev);
  void printToDesign(string , int );
  void gen_Large_Rule_Base(double nSup, const MinMaxPair&);
  bool gen_Large_ItemSet(itemSetListCluster<NODETYPE>& ptrList,double nSup,double totGroups,int lev, const MinMaxPair&);
  
  void removeAllElem();
  itemSetCluster<NODETYPE>* gen_New_Head(typename MapType::iterator iterSon, double nSup);
  typename MapType::iterator getLink(const NODETYPE& item);
  void checkSupportBaseDef(double nSup,newItemSetHashMap<NODETYPE>& pHashMap,itemSetListCluster<NODETYPE>& ptrList);
  void printItemSetRecursiveHead(int lev);
  bool gen_final_count(itemSetListCluster<NODETYPE>& ptrList,double nSup,int lev,newItemSetHashMap<NODETYPE>& pHashMap);
  void extractRule(sqlCoreConn&,const vector<NODETYPE>&,double totGroups);

  void printToDesign_2(string nome,int isPart);
  
  void mergeItemSet(
		    sqlCoreConn& coreConn,
		    itemSetListCluster<NODETYPE>& ptrList,
		    const SourceRowDescriptor& srd);


  // --- PRIVATE MEMBERS ---
 private:
  void saveIn_DB_Body(int lev,int isPart,const NODETYPE& node,const char* id_head,sqlCoreConn& coreConn);
  void saveIn_DB_Head(int lev,const NODETYPE& node,const char* id_head,int isPart,sqlCoreConn& coreConn);
  bool gen_Large_Rule_Base_Head(double nSup);
  void save_Large_ItemSet_HeadHelper(int lev,int isPart,const char* id_head_father,sqlCoreConn& coreConn);
  bool gen_Large_ItemSetHelper(itemSetListCluster<NODETYPE>& ptrList,double nSup,double totGroups,unsigned int lev, const MinMaxPair&);
  void printToDesignHelper(string here,string padre,ofstream& f1,int isPart);
  void extend_Head(double, const MinMaxPair&);
  bool gen_Large_ItemSet_Head(itemSetListCluster<NODETYPE>& ptrListHead,double nSup,int lev);
  void save_Large_ItemSetHelper(int lev,int isPart,sqlCoreConn&);
  bool gen_Large_ItemSet_HeadHelper(itemSetListCluster<NODETYPE>& ptrList,double nSup,unsigned int lev);
  
  void updatePtrList(				     
		     itemSetListCluster<NODETYPE>& ptrList,
		     unsigned int level,
		     itemSetCluster<NODETYPE>* newLevel);
  
  void mergeItemSetHelper(
			  itemSetListCluster<NODETYPE>& ptrList, 
			  odbc::ResultSet* resultAllBody,
			  // The following parameters describe the data set
			  const HeadBodySourceRowDescription& srDescription,
			  size_t lastElem,
			  // The following parameters will be passed to the
			  // mergeItemSetHead function
			  sqlCoreConn& coreConn,
			  const SourceRowDescriptor& srd);
  
  void mergeItemSetHeadBase(
			    odbc::PreparedStatement * statement,
			    const string& fHead,
			    const NODETYPE& item,
			    const SourceRowDescriptor& srd);
  
  
  void mergeItemSetHead(
			 odbc::PreparedStatement * statement,
			 itemSetListCluster<NODETYPE>& ptrList,
			 const string& fHead,
			 const SourceRowDescriptor& srd);
  
  void mergeItemSetHeadHelper(
			       itemSetListCluster<NODETYPE>& ptrList, 
			       odbc::ResultSet* resultHead,
			       const HeadBodySourceRowDescription& srDescription);

   bool setItemSetIfExistHead(const NODETYPE& valueH, const int gidH, const int cidH);
   void remove(itemSetListCluster<NODETYPE>& ptrList);
   void cancellaRef( list<itemSetCluster<NODETYPE>* >* listPtr);
   void cancellaHead(itemSetListCluster <NODETYPE>& listPtrHead);
   bool check_and_gen(itemSetListCluster <NODETYPE>& listPtrHead,double nSup);
   //   bool check_and_gen_from_Body(itemSetListCluster <NODETYPE> listPtrHead, double nSup,itemSetCluster<NODETYPE>* uno, itemSetCluster<NODETYPE>* due);
   void removeEntry(const NODETYPE& elem);

   bool gen_final_count_Head(itemSetListCluster<NODETYPE>& ptrList,
				double nSup,
				int lev,
				newItemSetHashMap<NODETYPE>& pHashMap);

   void gen_final_count_HeadHelper(
	      newItemSetHashMap<NODETYPE>& pHashMap,
	      newItemSetHashMap<NODETYPE>& pHashMapNew,
	      double nSup,
              itemSetListCluster<NODETYPE>& ptrList);

   bool prune_children(
		       itemSetEntryCluster<NODETYPE>& toBePruned,
		       newItemSetHashMap<NODETYPE>& pHashMap,
		       newItemSetHashMap<NODETYPE>& pHashMapNew,
		       double nSup,
		       itemSetListCluster<NODETYPE>& ptrList
		       );

   bool gen_final_count_Helper(
	      newItemSetHashMap<NODETYPE>& pHashMap,
	      newItemSetHashMap<NODETYPE>& pHashMapNew,
              itemSetListCluster<NODETYPE>& ptrList,
	      double nSup);

   void printRule(sqlCoreConn& coreConn,
		  const vector<NODETYPE>& body,
		  double denomBody,
		  const vector<NODETYPE>& head,
		  double totGroups);

   void removeAllReferences(
		itemSetEntryCluster<NODETYPE>& delElem,
		itemSetListCluster<NODETYPE>& ptrList
		);

   bool existHead(
		itemSetCluster<NODETYPE>* head,
		double nSup);

   bool existHeadHavingSuffSupport(
				      MapType& myParentMap,
				      itemSetEntryCluster<NODETYPE>& parentElem,
				      const NODETYPE& myBodyId,
				      itemSetEntryCluster<NODETYPE>& myBodyElem,
				      gidCidList<int>& myBodyElemNewList,
				      double nSup);
   
   void fillGidCidList(
		       itemSetCluster<NODETYPE>* parentHead,
		       itemSetCluster<NODETYPE>* myBodyHeadInParentMap,
		       gidCidList<int>& newList,
		       double nSup)  ;

   #if 0
   #warning debug
   void
     saveTList(string tlistname, gidCidList<NODETYPE>  list);
   // debug
   #endif

 private:
   // Membri privati
   int numLevel;
   HASHTYPE myAncestor;
   MapType isc;
   typename list<itemSetCluster<NODETYPE>* >::iterator refList;
};


#if 0
/* DEBUG */
#warning DEBUG
template< class NODETYPE >
void
itemSetCluster< NODETYPE >::saveTList(string tlistname, gidCidList<NODETYPE>  list) {
  ofstream outfile(tlistname.c_str());

  outfile << "INSERT INTO " << tlistname << " VALUES " << endl;

  typename gidCidList<NODETYPE>::MapType::const_iterator it;
  typename map <NODETYPE,int,NODETYPE>::const_iterator it2;

  for(it=list.getMap().begin(); it!=list.getMap().end() ;it++ ) {
    it2=it->second.begin();

    if( it2!=it->second.end() ) {
      outfile << "(";
      outfile << it->first.getSQLData() << "," << it2->first.getSQLData();
      outfile << ")";
      it2++;
    }

    for(; it2!=it->second.end(); it2++) {
      outfile << "," << endl;
      outfile << "(";
      outfile << it->first.getSQLData() << "," << it2->first.getSQLData();
      outfile << ")";
    }
  }

  outfile << ";";
}
#endif

/* IMPLEMENTAZIONE DEI METODI DI PARTITION.H */

template< class NODETYPE >
bool itemSetCluster< NODETYPE >::existEntry(const NODETYPE& item )
 {
/*
    bool exist;
    typename MapType::iterator i;

    i=isc.find(item);
   // cout<<"gidList "<<gidList<<" Item "<<value<<endl;
    if (i==isc.end())
       exist=false;
    else exist=true;

    return exist;
*/
    return (isc.find(item) != isc.end());

 }


template< class NODETYPE >
bool itemSetCluster< NODETYPE >::setItemSetIfExistHead(const NODETYPE& valueH, 
						       const int gidH, 
						       const int cidH)
 {
    typename MapType::iterator i = isc.find(valueH);

   // cout<<"gidList "<<gidList<<" Item "<<value<<endl;
    if (i==isc.end())
     {
      return false;
      //cout<<"HEAD FALSE NO INS g"<<gidH<<" c"<<cidH<<" item "<<valueH<<endl;
     }
    else {
      i->second.getGidCidList().insertValue(gidH,cidH);
   // cout<<"inserisco valore nella gidLsit"<<endl;
      return true;
    }

 }


template< class NODETYPE >
void itemSetCluster< NODETYPE >::setItemSetIfExist(
						   const NODETYPE& valueB,
						   const int gidB,
						   const int cidB, 
						   const NODETYPE& valueH, 
						   const int cidH) {

    typename MapType::iterator i = isc.find(valueB);

   // cout<<"gidList "<<gidList<<" Item "<<value<<endl;
    if (i==isc.end())
     {
      //cout<<"NON ESISTE E NON LO INSERISCO! "<<valueB<<" g"<<gidB<<" c"<<cidB<<endl;
     }
    else {
    //cout<<"Elemento body esiste "<<valueB<<" g"<<gidB<<" c"<<cidB<<endl;
    //cout<<"controllo head e se ok inserisco"<<endl;
    if ((i->second.getHead())->setItemSetIfExistHead(valueH,gidB,cidH))
        i->second.getGidCidList().insertValue(gidB,cidB);
   // cout<<"inserisco valore nella gidLsit"<<endl;
    }


 }


template< class NODETYPE >
typename itemSetCluster<NODETYPE>::MapType::iterator itemSetCluster< NODETYPE >::getLink(const NODETYPE& item )
 {
    return isc.find(item);
 }
template< class NODETYPE >
void itemSetCluster< NODETYPE >::setItemSet(
					    const NODETYPE& value, 
					    const int gid,
					    const int cid )
 {
    itemSetEntryCluster < NODETYPE > entry;
    typename MapType::iterator i = isc.find(value);

    if (i==isc.end())
     {
     // inserisco un nuovo valore
     entry.setData(value);
     entry.getGidCidList().insertValue(gid,cid);
     isc[value]=entry;
     }
    else {
     // Il valore già esiste, aggiungo solo le info sulla lista
     i->second.getGidCidList().insertValue(gid,cid);

    }


 }



template< class NODETYPE >
void itemSetCluster< NODETYPE >::setItemSet(
					    const NODETYPE& valueB,
					    const int gidB,
					    const int cidB,
					    const NODETYPE& valueH, 
					    const int cidH )
 {
    itemSetEntryCluster < NODETYPE > entry;
    typename MapType::iterator i=isc.find(valueB);
    itemSetCluster < NODETYPE >* iscHead;

    if (i==isc.end())
     {
     //cout<<"new value"<<endl;
     entry.setData(valueB);
     entry.getGidCidList().insertValue(gidB,cidB);
     iscHead=new itemSetCluster();
     iscHead->setItemSet(valueH,gidB,cidH);
     entry.setHead(iscHead);
     isc[valueB]=entry;
     }
    else {
      i->second.getGidCidList().insertValue(gidB,cidB);
      // Non puo' essere la head null poiche' ho gia' inserito una coppia
      i->second.getHead()->setItemSet(valueH,gidB,cidH);

    }
 }


template< class NODETYPE >
void itemSetCluster< NODETYPE >::setItemSet(const NODETYPE& value,
					    const gidCidList<int>& gidCid)
 {
    itemSetEntryCluster< NODETYPE > entry;
    typename MapType::iterator i=isc.find(value);

    if (i==isc.end())
     {

     entry.setData(value);
     entry.setGidList(gidCid);
     //entry.setCountGid(gidCid.numDistinctGID());

     isc[value]=entry;
     }
    else {
    i->second.setGidList(gidCid);
    i->second.setCountGid(gidCid.numDistinctGID());
    }
 }

template< class NODETYPE >
void itemSetCluster< NODETYPE >::setItemSet(const NODETYPE& value )
 {
    itemSetEntryCluster< NODETYPE > entry;
    typename MapType::iterator i=isc.find(value);

    if (i==isc.end()) {
      entry.setData(value);
      isc[value]=entry;
    }
 }

// Bisogna fare attenzione. Se elimino tutti gli elementi della head devo segnalarlo.
// Se la head è vuota il body non può esistere.
template< class NODETYPE >
bool itemSetCluster< NODETYPE >::gen_Large_Rule_Base_Head(double nSup)
 {
    typename MapType::iterator i;
    NODETYPE elemToDelete;
    bool isDelete,isOK;

    isDelete=false;
    isOK=false;

    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
          if (isDelete)
          {
          isc.erase(elemToDelete);
          isDelete=false;
          }

          if ( (i->second.getGidCidList().numDistinctGID()) < nSup )
             {
               elemToDelete=i->first;
               isDelete=true;
             }
          else isOK=true;
     }

     // se fosse l'ultimo elemento della MAP!
     if (isDelete)
      {
        isc.erase(elemToDelete);
        isDelete=false;
      }
 return isOK;
 }


// controllo che nell'insieme AR11 gli item con support suff.
// Prima controllo che il body sia a supp ok.
// Nel caso in cui body è ok allora controllo la head. Vedi
// gen_Large_Rule_base_Head per spiegazioni ulteriori.
template< class NODETYPE >
void itemSetCluster< NODETYPE >::gen_Large_Rule_Base(double nSup, const MinMaxPair& hcards)
 {
    typename MapType::iterator i;
    NODETYPE elemToDelete;
    bool isDelete,headOK;

    isDelete=false;

    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
          if (isDelete)
          {
          isc.erase(elemToDelete);
          isDelete=false;
          }

          if ( (i->second.getGidCidList().numDistinctGID()) < nSup )
             {
	      // cout<<"devo togliere questo elemento con la relativa HEAD"<<endl;

               (i->second.getHead())->removeAllElem();
               delete (i->second.getHead());

               elemToDelete=i->first;
               isDelete=true;
             }
          else
            {
              headOK=(i->second.getHead())->gen_Large_Rule_Base_Head(nSup);
              //tutti gli elementi della head sono stati cancellati.
              // -> non c'è head -> no body!
              //cout<<"ptr head "<<(i->second.getHead())<<endl;
              if (!headOK)
              {
               delete (i->second.getHead());

               elemToDelete=i->first;
               isDelete=true;
              }
              else (i->second.getHead())->extend_Head(nSup, hcards);

            }

     }

     // se fosse l'ultimo elemento della MAP!
     if (isDelete)
      {
        isc.erase(elemToDelete);
        isDelete=false;
      }



 }

template< class NODETYPE >
void itemSetCluster< NODETYPE >::removeAllElem()
 {
    isc.clear();
 }

template< class NODETYPE >
void itemSetCluster< NODETYPE >::printItemSetRecursive(int lev)
 {

    typename MapType::iterator i;


    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {

       for (int j=0; j<lev;j++)
            cout<<"  ";

       cout<<"Item "<<i->first<<" H" << endl ;//<<i->second.getHead()<<" ";

       if (i->second.getItemSet()!=NULL)
       {
         //cout<<"Address "<<i->second.getItemSet()<<endl;
	          i->second.getItemSet()->printItemSetRecursive(lev+1);
       }       

    }

    cout << "size:" << isc.size() << endl;
 }



// Crea un livello
template< class NODETYPE >
bool itemSetCluster< NODETYPE >::gen_Large_ItemSet(
				 itemSetListCluster<NODETYPE>& ptrList,
				 double nSup,
				 double totGroups,
				 int lev,
				 const MinMaxPair& hcards)
{
  //   itemSetCluster<NODETYPE>* iscNew;
   typename list<itemSetCluster<NODETYPE>* >::iterator i;
   list<itemSetCluster<NODETYPE>* >* listPtr;
   unsigned int rLev;
   bool otherItemSet,toContinue;

   //cout<<"Dentro gen_Large_ItemSet "<<lev<<endl;
   toContinue=false;
   if (lev!=2)
   {
    rLev=lev-1;
    //cout<<"IF di lev!=2 size"<<ptrList.vect.size()<<"level "<<lev<<" RLEV"<<rLev<<endl;
    if ((ptrList.vect.size()-1)<=rLev) {
        listPtr=ptrList.getList(rLev);

	//cout<<"OK2 "<<ptrList.vect[rLev]<<endl;
	for ( i=listPtr->begin(); i!=listPtr->end(); i++) {
	    //cout<<"*i -"<<*i<<endl;
	    //  otherItemSet=false;
	    otherItemSet=(*i)->gen_Large_ItemSetHelper(ptrList,nSup,totGroups,lev,hcards);
	    if (otherItemSet) toContinue=true;
	    //  cout<<"altre partizioni create "<<otherItemSet<<endl;
	  }
    }
   }
   else toContinue=gen_Large_ItemSetHelper(ptrList,nSup,totGroups,lev,hcards);
   //ptrList.print();


   return toContinue;
}

template< class NODETYPE >
bool itemSetCluster< NODETYPE >::gen_Large_ItemSetHelper(
				       itemSetListCluster<NODETYPE>& ptrList,
				       double nSup,
				       double totGroups,
				       unsigned int lev,
				       const MinMaxPair& hcards)
 {

    gidCidList <int> tListNew;
    itemSetCluster<NODETYPE>* iscNew;
    typename MapType::iterator i;
    typename MapType::iterator j;
    //typename list<int>::iterator i2;
    bool toLink,otherPart;
    list<itemSetCluster<NODETYPE>* >*  listPtr;
    typename MapType::iterator k;

    otherPart=false;

    //cout<<"create New level"<<ptrList.vect.size()<<"level "<<lev<<endl;
    if ((ptrList.vect.size()-1)>=lev)
        listPtr=ptrList.getList(lev);
    else listPtr=new list<itemSetCluster<NODETYPE>* >();

    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
       //cout<<"PADRE!!!!  "<<i->first<<endl;
       j=i;
       j++;
       iscNew=new itemSetCluster(lev);
       toLink=false;

       gidCidList<int>& tList=i->second.getGidCidList();
       while (j!=isc.end())
         {
         // cout<<"figlio generato "<<j->first<<endl;
          gidCidList<int>& t2=j->second.getGidCidList();

	  tListNew.clear();
          t2.set_intersection(tList,tListNew);

          if ( tListNew.isBigger(nSup) )
          {
            //cout<<"Supporto sufficiente"<<i->first<<endl;
            // Okay body con supp suff. controllo se ha una HEAD associata ok
            itemSetCluster <NODETYPE>* iscHead=(i->second.getHead())->gen_New_Head(j,nSup);
            if ( iscHead !=NULL )
            {
               //cout<<"head diverso di null"<<endl;

               iscNew->setItemSet(j->first,tListNew);
               k=iscNew->getLink(j->first);
               iscHead->extend_Head(nSup, hcards);
               k->second.setHead(iscHead);

               toLink=true;
            }
            //else cout<<"head NULL!"<<endl;
          }
          j++;
         }
    // i->second.gidListRemove();
        // cout<<"stampa prttNew"<<endl;
        // prttNew->printItemSet();

         if (toLink)
          {
             i->second.setItemSetChild(iscNew);
             listPtr->push_back(iscNew);

             //cout<<"creato parti ptr "<<prttNew<<endl;
          }
         else delete iscNew;
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

     return otherPart;

 }



template< class NODETYPE >
itemSetCluster<NODETYPE>* itemSetCluster< NODETYPE >::gen_New_Head(typename MapType::iterator iterSon,double nSup)
 {

    gidCidList<int> tListNew;
    itemSetCluster<NODETYPE>* iscNew;
    typename MapType::iterator i;
    //typename MapType::iterator j;
    //typename list<int>::iterator i2;
    bool existPtr;
   // iscHead=NULL;

    iscNew=NULL;
    existPtr=false;
    for (i=isc.begin();i!=isc.end();i++)
    {
        gidCidList<int>& tList=i->second.getGidCidList();
        if ( (iterSon->second.getHead())->existEntry(i->first) )
         {
            //cout<<"elemento esiste nella head."<<i->first<<endl;
            gidCidList<int>& t2=((iterSon->second.getHead())->getLink(i->first))->second.getGidCidList();
	    tListNew.clear();
            t2.set_intersection(tList,tListNew);
            if ( tListNew.isBigger(nSup) )
             {
                if (!existPtr)
                {
                    iscNew=new itemSetCluster();
                    existPtr=true;
                }
                iscNew->setItemSet(i->first,tListNew);
             }

         }
    }

   return iscNew;
 }




// Extend le head  - Cardinalità ancora da fissare... :-)
template< class NODETYPE >
void itemSetCluster< NODETYPE >::extend_Head(double nSup, const MinMaxPair& hcards)
{

   //typename list<itemSetCluster<NODETYPE>* >::iterator i;
   itemSetListCluster <NODETYPE> ptrListHead;
   bool toContinue;
   int levelIn;

   //cout<<"Dentro gen_Large_ItemSet "<<lev<<endl;
   ptrListHead.init();
   toContinue=true;
   levelIn=1;
   while ( (toContinue) && levelIn<hcards.getMax() )
   {
     levelIn=levelIn+1;
     toContinue=gen_Large_ItemSet_Head(ptrListHead,nSup,levelIn);
   }

}

template< class NODETYPE >
bool itemSetCluster< NODETYPE >::gen_Large_ItemSet_Head(itemSetListCluster<NODETYPE>& ptrListHead,double nSup,int lev)
{
   typename list<itemSetCluster<NODETYPE>* >::iterator i;
   list<itemSetCluster<NODETYPE>* >* listPtr;
   unsigned int rLev;
   bool otherItemSet,toContinue;

   //cout<<"Dentro gen_Large_ItemSet "<<lev<<endl;
   toContinue=false;
   if (lev!=2) {
     rLev=lev-1;
     //cout<<"IF di lev!=2 size"<<ptrList.vect.size()<<"level "<<lev<<" RLEV"<<rLev<<endl;
     if ((ptrListHead.vect.size()-1)<=rLev) {
       listPtr=ptrListHead.getList(rLev);

       //cout<<"OK2 "<<ptrList.vect[rLev]<<endl;
       for ( i=listPtr->begin(); i!=listPtr->end(); i++)
	 {
	   //cout<<"*i -"<<*i<<endl;
	   //  otherItemSet=false;
	   otherItemSet=(*i)->gen_Large_ItemSet_HeadHelper(ptrListHead,nSup,lev);
	   if (otherItemSet) toContinue=true;
	   //  cout<<"altre partizioni create "<<otherItemSet<<endl;
	 }
     }
   }
   else toContinue=gen_Large_ItemSet_HeadHelper(ptrListHead,nSup,lev);
   return toContinue;
}


template< class NODETYPE >
bool itemSetCluster< NODETYPE >::gen_Large_ItemSet_HeadHelper(itemSetListCluster<NODETYPE>& ptrList,double nSup,unsigned int lev)
 {

    gidCidList<int> tListNew;
    itemSetCluster<NODETYPE>* iscNew;
    typename MapType::iterator i;
    typename MapType::iterator j;
    //typename list<int>::iterator i2;
    bool toLink,otherPart;
    list<itemSetCluster<NODETYPE>* >*  listPtr;

    otherPart=false;

    //cout<<"create New level"<<ptrList.vect.size()<<"level "<<lev<<endl;
    if ((ptrList.vect.size()-1)>=lev)
        listPtr=ptrList.getList(lev);
    else listPtr=new list<itemSetCluster<NODETYPE>* >();

    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
       //cout<<"PADRE!!!!  "<<i->first<<endl;
       j=i;
       j++;
       iscNew=new itemSetCluster(lev);
       toLink=false;

       gidCidList<int>& tList=i->second.getGidCidList();
       while (j!=isc.end())
         {
         // cout<<"figlio generato "<<j->first<<endl;
          gidCidList<int>& t2=j->second.getGidCidList();

	  tListNew.clear();
          t2.set_intersection(tList,tListNew);


          if ( tListNew.isBigger(nSup) )
          {
          //  cout<<"Supporto Head sufficiente"<<i->first<<endl;
            // Okay head con supp suff.

               iscNew->setItemSet(j->first,tListNew);
               toLink=true;
          }
         j++;
        }
    // i->second.gidListRemove();
        // cout<<"stampa prttNew"<<endl;
        // prttNew->printItemSet();

         if (toLink)
          {
             i->second.setItemSetChild(iscNew);
             listPtr->push_back(iscNew);

             //cout<<"creato parti ptr "<<prttNew<<endl;
          }
         else delete iscNew;
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

     return otherPart;

 }




template< class NODETYPE >
void itemSetCluster< NODETYPE >::save_Large_ItemSet(
				 int lev,
				 int isPart,
				 sqlCoreConn& coreConn)  {
    typename MapType::iterator i;
    std::string id_head;

    //cout<<"itemSetCluter.h _ Dentro alla procedura"<<endl;
    for( i=isc.begin( ) ; i != isc.end( ) ; i++ ) {

       id_head=i->second.getHead()->save_Large_ItemSet_Head(1,isPart,coreConn);
       // !!! bobo 030603
       i->second.releaseHead();
       // !!! end

       saveIn_DB_Body(lev,isPart,i->first,id_head.c_str(),coreConn);
       

       if (i->second.getItemSet()!=NULL) {
	 i->second.getItemSet()->save_Large_ItemSetHelper(lev+1,isPart,coreConn);
	 // !!! bobo 030603
	 i->second.releaseItemSet();
	 // !!! end
	 }
    }

    isc.clear();
 }


template< class NODETYPE >
std::string itemSetCluster< NODETYPE >::save_Large_ItemSet_Head(int lev,int isPart,sqlCoreConn& coreConn)
 {
    typename MapType::iterator i;
    std::string id_head;
    std::stringstream oHead;

    oHead << this;
    //    string nomeH=oHead.str();
    id_head=oHead.str();

    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {

      //cout<<lev<<":::"<<i->first<<endl;
      saveIn_DB_Head(lev,i->first,id_head.c_str(),isPart,coreConn);
      if (i->second.getItemSet()!=NULL) {
           i->second.getItemSet()->save_Large_ItemSet_HeadHelper(lev+1,isPart,id_head.c_str(),coreConn);
	   // !!! bobo 030603
	   i->second.releaseItemSet();
	   // !!! end
      }
      //prtt.erase(i);
    }

   isc.clear();
   //out_Head.close();
   //cout<<"Finito la creazione del file head"<<endl;
   return id_head;
  }

template< class NODETYPE >
void itemSetCluster< NODETYPE >::save_Large_ItemSetHelper(int lev,int isPart,sqlCoreConn& coreConn)
 {
    typename MapType::iterator i;
    std::string id_head;

    
     for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {

      id_head=i->second.getHead()->save_Large_ItemSet_Head(1,isPart,coreConn);
      // !!! bobo 030603
      i->second.releaseHead();
      // !!! end

      saveIn_DB_Body(lev,isPart,i->first,id_head.c_str(),coreConn);
      if (i->second.getItemSet()!=NULL)
      {
         i->second.getItemSet()->save_Large_ItemSetHelper(lev+1,isPart,coreConn);
         //Quando esco dealloco il puntatore
         i->second.releaseItemSet();
      }
    }

 }

template< class NODETYPE >
void itemSetCluster< NODETYPE >::save_Large_ItemSet_HeadHelper(int lev,int isPart,const char* id_head_father,sqlCoreConn& coreConn)
 {
    typename MapType::iterator i;
    //    const char* id_head;

    
     for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
      saveIn_DB_Head(lev,i->first,id_head_father,isPart,coreConn);

      if (i->second.getItemSet()!=NULL)
      {
         i->second.getItemSet()->save_Large_ItemSet_HeadHelper(lev+1,isPart,id_head_father,coreConn);
         //Quando esco dealloco il puntatore
         i->second.releaseItemSet();
      }
    }

 }
template< class NODETYPE >
void itemSetCluster< NODETYPE >::saveIn_DB_Body(
			     int lev,
			     int isPart,
			     const NODETYPE& node,
			     const char* id_head,
			     sqlCoreConn& coreConn)
 {
    char inter[10],intPart[10];
    //const char* bid;
    std::string Qry;
    std::string QryDef;

    Qry="INSERT INTO tmp_Rule_Ext VALUES (0,";
    sprintf(inter,"%d",lev);
    sprintf(intPart,"%d",isPart);
    //    bid=node.c_str();

    //cout<<lev<<":::"<<i->first<<":::"<<id_head<<isPart<<endl;
    //cout << node.getSQLData() << endl;
    //cout << "!!! "  << id_head << " " << intPart <<  endl;

    QryDef=Qry+inter+","+node.getSQLData()+",'"+id_head+intPart+"');";
    //    cout<<QryDef<<endl;
    const char * dains=QryDef.c_str();
    coreConn.insert_DB(dains);

    //cout << ".";
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::saveIn_DB_Head(int lev,const NODETYPE& node,const char* id_head,int isPart,sqlCoreConn& coreConn)
 {
    char inter[10],intPart[10];
    //    const char* hid;
    std::string Qry;
    std::string QryDef;

    Qry="INSERT INTO tmp_Rule_Head_Ext VALUES (0,'";

    sprintf(inter,"%d",lev);
    sprintf(intPart,"%d",isPart);
    // hid=node.c_str();

    QryDef=Qry+id_head+intPart+"',"+inter+","+node.getSQLData()+");";
    //cout<<QryDef<<endl;
    const char * dains=QryDef.c_str();
    coreConn.insert_DB(dains);
}


/***************************************
   FASE DI MERGE
 ***************************************/

template< class NODETYPE >
void itemSetCluster< NODETYPE >::updatePtrList(				     
				     itemSetListCluster<NODETYPE>& ptrList,
				     unsigned int level,
				     itemSetCluster<NODETYPE>* newLevel) {
  list<itemSetCluster<NODETYPE>* >*  listPtr;
  typename list<itemSetCluster<NODETYPE>* >::iterator riferimento;

  if ((ptrList.vect.size()-1)>=level) {
    // esiste la lista di livello level
    listPtr=ptrList.getList(level);
    listPtr->push_back(newLevel);
    ptrList.vect[level]=listPtr;
  } else {
    // Non c'e' una lista di livello level!
    listPtr=new list<itemSetCluster<NODETYPE>* >();
    listPtr->push_back(newLevel);
    ptrList.vect.push_back(listPtr);
  }

  riferimento=listPtr->end();
  riferimento--;
  newLevel->setRefList(riferimento);
}

template< class NODETYPE >
void itemSetCluster< NODETYPE >::mergeItemSet(
				     sqlCoreConn& coreConn,
				     itemSetListCluster<NODETYPE>& ptrList,
				     const SourceRowDescriptor& srd) {
   std::string Qry;
   HASHTYPE anc;
   NODETYPE temp;
   int level;
   odbc::Statement* statement=coreConn.getConnection()->createStatement();
   odbc::ResultSet* resultAllBody;
   level=0;
   size_t lastElem;

   string idHead("");

   // Il db del minerule è connesso!

   Qry = "create index id_head on tmp_Rule_Head_Ext (id_head);";
   statement->execute(Qry);

   Qry="select level,"
     +srd.getBody().getSQLColumnNames()+",id_head from tmp_Rule_Ext order by id;";

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
void itemSetCluster< NODETYPE >::mergeItemSetHelper(
			itemSetListCluster<NODETYPE>& ptrList, 
			odbc::ResultSet* resultAllBody,
			// The following parameters describe the data set
			const HeadBodySourceRowDescription& srDescription,
			size_t lastElem,
			// The following parameters will be passed to the
			// mergeItemSetHead function
			sqlCoreConn& coreConn,
			const SourceRowDescriptor& srd) {
  LevelInfoStack levelStack;
  //  levelStack.push_back(NODETYPE(), this);
  levelStack.push_back(this);

  NODETYPE lastItem; 
  string lastHead;

  unsigned int level;
  NODETYPE item;
  string head; 
  string QrySt="select level,"+srd.getHead().getSQLColumnNames()+
     " from tmp_Rule_Head_Ext where id_head=? order by id;";
  odbc::PreparedStatement* statement=coreConn.getConnection()->prepareStatement(QrySt);
  
  while( resultAllBody->next() ) {
    

    // lettura dal db
    HeadBodySourceRow srow(resultAllBody, srDescription);

    level = atoi(resultAllBody->getString(1).c_str());

    lastItem = item;
    item = srow.getBody();

    lastHead = head;
    head = resultAllBody->getString(lastElem+1);

    //    cerr << "level: " << level << " item:" << item << endl;

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
      itemSetCluster<NODETYPE>* newLevel = parentItem->second.getItemSet();
      if( newLevel == NULL ) {
	// there are no child items for parentItem... I have to build up
	// the new level
	
	newLevel=new itemSetCluster();
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
    levelStack.getCurLevelInfo()->mergeItemSetHeadBase(statement,head,item, srd);
  }
  delete statement;
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::mergeItemSetHeadBase(
				odbc::PreparedStatement * statement,
				const string& fHead,
				const NODETYPE& item,
				const SourceRowDescriptor& srd)
 {
   itemSetListCluster <NODETYPE> prtListH;
   typename MapType::iterator k;
   itemSetCluster<NODETYPE>* iscHead;

   k=getLink(item);
   prtListH=k->second.getListPtrHead();
   if (prtListH.vect.size()==0)
       prtListH.init();

   if (k->second.getHead()!=NULL)
       iscHead=k->second.getHead();
   else
       iscHead=new itemSetCluster();

   iscHead->mergeItemSetHead(statement,prtListH,fHead,srd);
   k->second.setHead(iscHead);
   k->second.setListPtrHead(prtListH);
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::mergeItemSetHead(
			    odbc::PreparedStatement * statement,
			    itemSetListCluster<NODETYPE>& ptrList,
			    const string& fHead,
			    const SourceRowDescriptor& srd)
 {

   odbc::ResultSet* resultHead;
   size_t lastElem;

   // Il db del minerule è connesso!

   statement->setString(1,fHead);
   
   HeadBodySourceRowDescription srDescription;
   lastElem=srDescription.setHeadElems(2,srd.getHead().getColumnsCount());

   resultHead=statement->executeQuery();
   
   if (resultHead!=NULL) {
     mergeItemSetHeadHelper(ptrList,
			    resultHead,
			    srDescription);
     delete resultHead;
   } else cout<<"ResultHead == NULL"<<endl;
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::mergeItemSetHeadHelper(
			itemSetListCluster<NODETYPE>& ptrList, 
			odbc::ResultSet* resultHead,
			const HeadBodySourceRowDescription& srDescription) {
  LevelInfoStack levelStack;
  //levelStack.push_back(NODETYPE(), this);
  levelStack.push_back(this);

  NODETYPE lastItem; 

  unsigned int level = -1;
  NODETYPE item;
  string head; 
  
  while( resultHead->next() ) {
    // lettura dal db
    HeadBodySourceRow srow(resultHead, srDescription);

    level = atoi(resultHead->getString(1).c_str());

    lastItem = item;
    item = srow.getHead();

    //    cerr << "head - level: " << level << " item:" << item << endl;

    while( level<levelStack.getCurLevel() ) {
      // torno indietro
      levelStack.pop_back();
    }
    
    if( level > levelStack.getCurLevel() ) {
      assert( level == levelStack.getCurLevel()+1 );
      typename MapType::iterator parentItem;
      //      assert( levelStack.getAncestors().size() > 0 );
            
      parentItem=levelStack.getCurLevelInfo()->getLink(lastItem);

      //cerr << "lastItem:" << lastItem << ":fineLastItem" << endl;
      // cerr << "mappa:" << endl;
      //levelStack.getCurLevelInfo()->printItemSetRecursive(1);
      // cerr << "finemappa" << endl;


      assert(parentItem!=levelStack.getCurLevelInfo()->getMap().end());
      itemSetCluster<NODETYPE>* newLevel = parentItem->second.getItemSet();
      if( newLevel == NULL ) {
	// there are no child items for parentItem... I have to build up
	// the new level
	
	newLevel=new itemSetCluster();
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


/***************************************
   FASE DUE (RICALCOLO DEI SUPPORTI)
 ***************************************/

template< class NODETYPE >
void 
itemSetCluster<NODETYPE>::removeAllReferences(
		itemSetEntryCluster<NODETYPE>& delElem,
		itemSetListCluster<NODETYPE>& ptrList
		) {
  itemSetCluster<NODETYPE>* iscChild = delElem.getItemSet();
  itemSetCluster<NODETYPE>* iscHead = delElem.getHead();


  if( iscHead!=NULL ) {
    iscHead->cancellaHead(delElem.getListPtrHead());
    delete (iscHead);
    delElem.setHead(NULL);
  }

  if( iscChild!=NULL ) {
    int nlev = iscChild->getLevel();  
    list<itemSetCluster<NODETYPE>* >* listPtr=ptrList.getList(nlev);
    assert( listPtr!=NULL );

    iscChild->remove(ptrList);
    iscChild->cancellaRef(listPtr);
    delete (iscChild);
    delElem.setItemSetChild(NULL);
  }
 }


template< class NODETYPE >
void itemSetCluster< NODETYPE >::checkSupportBaseDef(
				  double nSup,
				  newItemSetHashMap<NODETYPE>& pHashMap,
				  itemSetListCluster<NODETYPE>& ptrList)
 {

   //    itemSetCluster<NODETYPE>* iscNew;
    typename MapType::iterator i;

    
    MapType tmpMap;
    NODETYPE elemToDelete;

    vector<NODETYPE> elemsToBeDeleted;


    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
      {
	if (i->second.getGidCidList().numDistinctGID()< nSup)
	  {
	    removeAllReferences( i->second, ptrList );
	    elemsToBeDeleted.push_back(i->first);    
	  }
	else
	  {
	    
	    // Qui devo controllare il supp. della head. Se ho almeno una head... ok!
	    // e mentre controllo faccio che calcolare le liste dei sottolivelli della
	    // head.
	    //cout<<"first element of map"<<i->first<<endl;
	    
	    if ( i->second.getHead()->check_and_gen(i->second.getListPtrHead(), 
						    nSup) )
	      {
		i->second.setCountGid(i->second.getGidCidList().numDistinctGID());
		vector<NODETYPE> tmp;
		tmp.push_back(i->first);
		pHashMap.add(tmp);
	      }
	    else
	      {
		removeAllReferences(i->second, ptrList);
		elemsToBeDeleted.push_back(i->first);
	      }
	  }
      } // FINE FOR
      
    // rimuoviamo fisicamente gli elementi dalla mappa.
    // N.B. Non puo' essere fatto prima perche' altrimenti l'iteratore "i"
    // verrebbe invalidato.

    cout << "Number of Items without sufficient support: " << elemsToBeDeleted.size() << endl;
    typename vector<NODETYPE>::iterator it;
    for(it=elemsToBeDeleted.begin(); it!=elemsToBeDeleted.end(); it++) {
      isc.erase(*it);
    }
 }



// Nota : non può esistere body senza head. Perciò non controllo se Head!=null
template< class NODETYPE >
void itemSetCluster< NODETYPE >::remove(itemSetListCluster<NODETYPE>& ptrList)
 {
   typename MapType::iterator i;
   itemSetCluster<NODETYPE>* p;
   list<itemSetCluster<NODETYPE>* >* listPtr;
   int lev;
   typename list<itemSetCluster<NODETYPE>* >::iterator riferimento;

   lev=getLevel();
   //cout<<"Remove tutto lev"<<lev<<endl;
   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
       //cancello tutta la head. Dalla lista... :-)
       i->second.getHead()->cancellaHead(i->second.getListPtrHead());
       delete (i->second.getHead());

       //    cout<<"Dentro REmove da taglio "<<i->first<<endl;
       p=i->second.getItemSet();
       if (p!=NULL)
	 {
	   p->remove(ptrList);
	   riferimento=p->getRefList();
	   listPtr=ptrList.getList(lev);
	   listPtr->erase(riferimento);
	   delete p;
	 }
     }
   isc.clear();
 }


template< class NODETYPE >
void itemSetCluster< NODETYPE >::cancellaRef( list<itemSetCluster<NODETYPE>* >* listPtr)
 {
     typename list<itemSetCluster<NODETYPE>* >::iterator riferimento;

     riferimento=getRefList();
     //cout<<"refList"<<*riferimento<<endl;
     listPtr->erase(riferimento);
 }


// Ho la lista di tutti i livelli e dei relativi ptr.
// Scorro la lista cancello tutti i ptr e poi cancello la lista.
template< class NODETYPE >
void itemSetCluster< NODETYPE >::cancellaHead(
			    itemSetListCluster <NODETYPE>& listPtrHead)
 {
  typename vector<list<itemSetCluster<NODETYPE>* >* >::iterator i;
  typename list<itemSetCluster<NODETYPE>* >::iterator j;

  for ( i=listPtrHead.vect.begin(); i!=listPtrHead.vect.end(); i++)
    {
      if ((*i)!=NULL)
	{
	  for ( j=(*i)->begin(); j!=(*i)->end(); j++)
            {
	      delete(*j);
	      //cout<<"FOOT"<<*i<<endl;
            }
	}
    }
  
  listPtrHead.removeAll();
 }


// Controlla la lista delle head e se non esiste nessuna head possibile ritorna il valore false.
// Se ritorna false devo cancellare anche il body. Se ritorna true, ha controllato e generato

template< class NODETYPE >
bool itemSetCluster< NODETYPE >::check_and_gen(
			  itemSetListCluster <NODETYPE>& listPtrHead, 
			  double nSup)
 {
   //    itemSetCluster<NODETYPE>* iscNew;
    typename MapType::iterator i;
    //    int nlev;
    //    list<itemSetCluster<NODETYPE>* >* listPtr;
    //    NODETYPE elemToDelete;
    
    newItemSetHashMap <NODETYPE> pHashMapHead;
    vector<NODETYPE> elemsToBeDeleted;
    bool existOneHead = false;
    

    for( i=isc.begin( ) ; i != isc.end( ) ; i++ ) {
       if(!i->second.getGidCidList().isBigger(nSup) ) {
	   removeAllReferences(i->second, listPtrHead);
	   elemsToBeDeleted.push_back(i->first);
       } else {
	   i->second.setCountGid(i->second.getGidCidList().numDistinctGID());
	   vector<NODETYPE> tmp;
	   tmp.push_back(i->first);
           pHashMapHead.add(tmp);
	   existOneHead = true;
       }
    }

   

    // rimuoviamo fisicamente gli elementi dalla mappa.
    // N.B. Non puo' essere fatto prima perche' altrimenti l'iteratore "i"
    // verrebbe invalidato.

    typename vector<NODETYPE>::iterator it;
    for(it=elemsToBeDeleted.begin(); it!=elemsToBeDeleted.end(); it++) {
      isc.erase(*it);
    }
    


    if (existOneHead)
      {
	bool toContinue=true;
	int levelIn=1;
	while  (toContinue)
	  {
	    levelIn=levelIn+1;
	    //cout<<"Address livello head "<<this<<endl;
	    toContinue=this->gen_final_count_Head(listPtrHead,nSup,levelIn,pHashMapHead);
	  }
      }
    
    return existOneHead;
 }


template< class NODETYPE >
bool itemSetCluster< NODETYPE >::gen_final_count_Head(
			       itemSetListCluster<NODETYPE>& ptrList,
			       double nSup,
			       int lev,
			       newItemSetHashMap<NODETYPE>& pHashMap)
{
   typename list<itemSetCluster<NODETYPE>* >::iterator i;
   list<itemSetCluster<NODETYPE>* >* listPtr;
   unsigned int rLev;
   //cout<<"creazione phashMapNew"<<endl;
   newItemSetHashMap<NODETYPE> pHashMapNew;


   //printItemSetRecursiveHead(1);
   //cout<<"EstimateLevel_HEAD: "<<lev<<endl;

   rLev=lev-1;

   if ((ptrList.vect.size()-1)>=rLev)
     listPtr=ptrList.getList(rLev);
   else 
     return false;


   if (lev!=2) {
     for ( i=listPtr->begin(); i!=listPtr->end(); i++) {
       (*i)->gen_final_count_HeadHelper(pHashMap,
					pHashMapNew,
					nSup,
					ptrList);
     }
   } else {
     gen_final_count_HeadHelper(pHashMap,
				pHashMapNew,
				nSup,
				ptrList);

   }
   
   pHashMap=pHashMapNew;
   return true;
}

/**
 * Controlla itemSetEntry "toBePruned", per ogni suo figlio "f" controlla se "f" ha
 * supporto sufficiente e lo rasa via in caso contrario. Nel controllare se "f" ha
 * supporto sufficiente si controlla:
 *    1) se esistono tutti gli ancestor di "f" e se sono a supporto sufficiente.
 *    2) se "f" e' a supporto sufficiente
 *    3) se esistono head agganciate al nodo, nel caso esistano si controlla se ne
 *       esiste almeno una a supporto sufficiente. Si rasano via le head con supporto
 *       non sufficiente man mano che vengono trovate.
 * La procedura puo' venire chiamata sia da nodi nella struttura contenente i body che 
 * da quella conenente le head. Infatti quando si controlla se esistono le head a supporto
 * sufficiente, si restituisce immediatamente true nel caso nessuna head esista.
 *
 * return value: true se toBePruned deve essere rimosso dalla mappa che lo contiene (cioe'
 *   se toBePruned non ha piu' nessun elemento valido al suo interno.
 */

template< class NODETYPE >
bool itemSetCluster< NODETYPE >::prune_children(
						itemSetEntryCluster<NODETYPE>& toBePruned,
						newItemSetHashMap<NODETYPE>& pHashMap,
						newItemSetHashMap<NODETYPE>& pHashMapNew,
						double nSup,
						itemSetListCluster<NODETYPE>& ptrList
				   ) {
  gidCidList<int>& tList = toBePruned.getGidCidList();
  if( toBePruned.getItemSet()==NULL ) {
    return false;
  }

  itemSetCluster<NODETYPE>* iscNew  = toBePruned.getItemSet();
  HASHTYPE myAncestor=iscNew->getAncestor();
  int level=iscNew->getLevel();
  MapType& childMap=iscNew->getMap(); // 
  bool oneOK = false;
  bool doApriori;

  vector<NODETYPE> elemsToBeDeleted;
  typename MapType::iterator j;
  for( j=childMap.begin(); j !=childMap.end( ) ; j++ )  {

      doApriori=pHashMap.findTies(myAncestor,j->first,level);

      if (doApriori) {
	  gidCidList<int> tListNew;

	  assert( isc.find(j->first)!=isc.end() );	  
	  gidCidList<int>& tParentList=(isc.find(j->first))->second.getGidCidList();
	  tParentList.set_intersection(tList,tListNew);

	  if (  tListNew.isBigger(nSup) && 
		existHeadHavingSuffSupport(
					   isc,
					   toBePruned,	   
					   j->first, 
					   j->second, 
					   tListNew,
					   nSup) ) { 
	    // tutti i miei antenati hanno supporto sufficiente (infatti doApriori
	    // e' true) ed io ho supporto sufficiente (infatti tListNew.isBigger(nSup))
	    // :-)
	    iscNew->setItemSet(j->first,tListNew);
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
	  //cout << "CANCELLO:" << j->first << endl;
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
    list< itemSetCluster<NODETYPE>* >* listPtr=ptrList.getList(nLev);
    
    toBePruned.getItemSet()->remove(ptrList);
    toBePruned.getItemSet()->cancellaRef(listPtr);
    delete(toBePruned.getItemSet());
    toBePruned.setItemSetChild(NULL);
  }

  return !oneOK;
}

template< class NODETYPE >
void itemSetCluster< NODETYPE >::gen_final_count_HeadHelper(
	      newItemSetHashMap<NODETYPE>& pHashMap,
	      newItemSetHashMap<NODETYPE>& pHashMapNew,
	      double nSup,
              itemSetListCluster<NODETYPE>& ptrList) {
  typename MapType::iterator i;
  
  //  cout << "DEBUG HEAD" << endl;
  //  cout << "ISC SIZE:" << isc.size() << endl;

  vector<NODETYPE> elemsToBeDeleted;

  for( i=isc.begin( ) ; i != isc.end( ) ; i++ ) {
    if(prune_children(i->second, pHashMap, pHashMapNew, nSup, ptrList )
       && !i->second.getGidCidList().isBigger(nSup)) 
      elemsToBeDeleted.push_back(i->first);
  }

  typename vector<NODETYPE>::iterator it = elemsToBeDeleted.begin();
  for( ; it!=elemsToBeDeleted.end(); it++ ) {
    isc.erase(*it);
  }
  //  cout << "end ISC SIZE:" << isc.size() << endl;
  //  cout << "end DEBUG HEAD" << endl;
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::removeEntry(const NODETYPE& elem)
 {
    isc.erase(elem);
 }


template< class NODETYPE >
bool itemSetCluster< NODETYPE >::gen_final_count(
                itemSetListCluster<NODETYPE>& ptrList,
		double nSup,
		int lev,
		newItemSetHashMap<NODETYPE>& pHashMap) {

   typename list<itemSetCluster<NODETYPE>* >::iterator i;
   list<itemSetCluster<NODETYPE>* >* listPtr;
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
bool itemSetCluster< NODETYPE >::gen_final_count_Helper(
	      newItemSetHashMap<NODETYPE>& pHashMap,
	      newItemSetHashMap<NODETYPE>& pHashMapNew,
              itemSetListCluster<NODETYPE>& ptrList,
	      double nSup) {
   typename MapType::iterator i;

   vector<NODETYPE> elemsToBeDeleted;

   for( i=isc.begin( ) ; i != isc.end( ) ; i++ ) {
     if( prune_children(i->second, pHashMap, pHashMapNew, nSup, ptrList ) 
	 && !existHead(i->second.getHead(),nSup) ) {
       elemsToBeDeleted.push_back(i->first);
     } 
   }

   typename vector<NODETYPE>::iterator it;
   for(it=elemsToBeDeleted.begin(); it!=elemsToBeDeleted.end(); it++ ) {
     isc.erase(*it);
   }

   return true;
 }

template< class NODETYPE >
bool itemSetCluster< NODETYPE >::existHead(
				itemSetCluster<NODETYPE>* head,
				double nSup)
{
  bool exist = false;
  typename MapType::iterator i;
  for( i=head->isc.begin( ) ; i != head->isc.end() && !exist ; i++ )
    exist = i->second.getGidCidList().isBigger(nSup);
  return exist;
}

template< class NODETYPE >
bool itemSetCluster< NODETYPE >::existHeadHavingSuffSupport(
			       MapType& myParentMap,
			       itemSetEntryCluster<NODETYPE>& parentElem,
			       const NODETYPE& myBodyId,
			       itemSetEntryCluster<NODETYPE>& myBodyElem,
			       gidCidList<int>& myBodyElemNewList,
			       // the above parameter is the gidCidList that will be
			       // set for myBodyElem once this procedure returns true
			       double nSup) {
  itemSetCluster<NODETYPE>* parentHead = parentElem.getHead();
  if( parentHead==NULL ) {
    return true;
  }

  typename MapType::iterator it = myParentMap.find(myBodyId);
  assert(it!=myParentMap.end());

  itemSetCluster<NODETYPE>* myBodyHeadInParentMap = it->second.getHead();
  assert(myBodyHeadInParentMap!=NULL);

  itemSetCluster<NODETYPE>* myBodyHead = myBodyElem.getHead();
  assert( myBodyHead!=NULL );
  
  myBodyHead->fillGidCidList(parentHead, 
			     myBodyHeadInParentMap, 
			     myBodyElemNewList, 
			     nSup);

  return  myBodyHead->check_and_gen(myBodyElem.getListPtrHead(),nSup);
}

/**
 * fillGidCidList -- 
 *    newList: e' la gidCidList a supporto suff. che verra' assegnata al corrente body,
 *             in caso ci sia una regola a supp. suff  (vedi istr. setItemSet in prune_children)
 *
 *   Consideriamo un nuovo itemset A,B. Per ogni elemento K della head ad esso associata 
 *   consideriamo le mappe head hA e hB associate agli ItemSet A e B (quindi al livello precedente),
 *   fillGidCidList calcola la nuova gidCidList associata a K per l'ItemSet A,B (cioe'
 *   al livello corrente) intersecando:
 *     1) le gidCidList associate a k in hA e hB (se esistono) (l'intersezione avviene tenendo conto
 *        sia del campo Gid che del campo Cid)
 *     2) il risultato dell'intersezione al punto 1) con newList.
 */

template< class NODETYPE >
void itemSetCluster< NODETYPE >::fillGidCidList(
			   itemSetCluster<NODETYPE>* parentHead,
			   itemSetCluster<NODETYPE>* myBodyHeadInParentMap,
			   gidCidList<int>& newList,
			   double nSup) {

  typename MapType::const_iterator it = isc.begin();

  for(; it!=isc.end(); it++ ) {
    typename MapType::iterator 
      headListIt = parentHead->getMap().find(it->first);

    typename MapType::iterator 
      headInParentListIt = myBodyHeadInParentMap->getMap().find(it->first);


    if( headListIt!=parentHead->getMap().end() &&
	headInParentListIt!=myBodyHeadInParentMap->getMap().end() ) {

      gidCidList<int> tlistNew;
      headListIt->second.getGidCidList().set_intersection( 
			    headInParentListIt->second.getGidCidList(), tlistNew );

      if( tlistNew.numDistinctGID()>0 ) {
	tlistNew.set_gid_intersection(newList);
      }

      setItemSet( it->first, tlistNew  );
    }
  }
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::printRule(
			 sqlCoreConn& coreConn,
			 const vector<NODETYPE>& body,
			 double denomBody,
			 const vector<NODETYPE>& head,
			 double totGroups) {
   typename MapType::iterator i;
   double sup,conf;


   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
       vector<NODETYPE> curHead = head;

       curHead.push_back(i->first);
       double numHead= i->second.getCountGid();
       sup=(numHead/totGroups);
       conf=(numHead/denomBody);

       coreConn.insert_DB(body,curHead,sup,conf);

       if (i->second.getItemSet()!=NULL) {
         i->second.getItemSet()->printRule(coreConn,body,denomBody,curHead,totGroups);
       }
     }
}




template< class NODETYPE >
void itemSetCluster< NODETYPE >::extractRule(sqlCoreConn& coreConn,const vector<NODETYPE>& body, double totGroups)
 {
   typename MapType::iterator i;

   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
       vector<NODETYPE> curBody = body;
       curBody.push_back(i->first);

       double denom=i->second.getCountGid();
       
       i->second.getHead()->printRule(coreConn,curBody,denom,vector<NODETYPE>(),totGroups);

       if (i->second.getItemSet()!=NULL) {
         i->second.getItemSet()->extractRule(coreConn,curBody,totGroups);
       }
    }
 }



#if 0 // deprecated
template< class NODETYPE >
void itemSetCluster< NODETYPE >::extractRule(sqlCoreConn& coreConn,string regola,int lev,double totGroups)
 {
   typename MapType::iterator i;
   string rule;
   
   rule=regola;


   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
        //cout<<"Address "<<i<<endl;
	if (rule!="")
	  regola=rule+","+i->first.c_str();
	else
	  regola=i->first.c_str();
       //cout<<"Item "<<i->first<<" GidList: ";
       //gList=i->second.getGidList();
       //for (i2=gList.begin(); i2!=gList.end();i2++)
       //    cout<<*i2<<"  ";
       //cout<<"level"<<getLevel()<<endl;
       //if (lev!=1) combina2(regola);
       //cout<<"BODY : "<<lev<<endl;
       double denom=i->second.getCountGid();
       i->second.getHead()->printRule(coreConn,regola,lev,denom,"",1,totGroups);
       if (i->second.getItemSet()!=NULL)
       {
         //cout<<"Address "<<i->second.getItemSet()<<endl;

         i->second.getItemSet()->extractRule(coreConn,regola,lev+1,totGroups);
	 //         cout<<endl;
       }
     regola="";
    }

 }
#endif





//------------------------- disegno GRAFICAMENTE la struttura : WinGraph //
template< class NODETYPE >
void itemSetCluster< NODETYPE >::printToDesign(string nome,int isPart)
 {

   /*string files,label1,values,nod,vStruct,address ,idList;*/
   typename MapType::iterator i;
   //typename list<int>::iterator i2;
   //list<int> tList;
   char filename[255];
   const char* nomeSt;
   //string myAncestor;

   //cout<<"Sono in disegno grafico della struttura"<<endl;
   nomeSt=nome.c_str();

   sprintf(filename,"design/LClust%s%d.txt",nomeSt,isPart);

   ofstream f1(filename);

   f1<<"digraph minerule { "<<endl;
   f1<<"graph[fontsize=8]; edge  [fontsize=8]; node  [fontsize=8]; ranksep = .30; nodesep = .25;"<<endl;
   f1<<"node [fontname=\"Courier\"]; "<<endl;
   f1<<"node [shape=record];"<<endl;

   string vStruct="struct0";
   string label1="struct0[shape=record,label=\"";
   string values;
   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
      string idList;
      values=values+"<"+
	i->first.c_str()+">"+
	i->first.c_str();
      if (i->second.getHead()!=NULL)
       {
        std::stringstream oHead;
        oHead <<i->second.getHead();
        string nomeH=oHead.str();
	//        values=values+" : H_"+nomeH;
	
     }

#if 1      //stampa degli ancestor
      typename vector<NODETYPE>::const_iterator it=getAncestor().begin();
      for(;it!=getAncestor().end(); it++) {
	values += std::string(it->c_str())+"-";
      }
#endif
      
      if (i->second.getGidCidList().Csize()>0) idList=(i->second.getGidCidList()).getListToDesign();
      values=values+":  "+idList+"|";
      
     }

   int siz=values.size();
   siz=siz-1;
   if (siz>0)  values.resize(siz);
   label1=label1+values+"\"];";
   f1<<label1<<endl;
   //values="";


   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
   {

    string nod=i->first.c_str();

    if (i->second.getHead()!=NULL)
     {
      std::stringstream oHead;
      oHead <<i->second.getHead();
      string nomeH=oHead.str();

       //cout<<"nome File head"<<nomeH<<endl;
       i->second.getHead()->printToDesign(nomeH,isPart);
     }


     if ((i->second.getItemSet())!=NULL)
     {

      std::stringstream o2;
      o2 <<i->second.getItemSet();
      string address=o2.str();
      nod=vStruct+":"+nod;

      i->second.getItemSet()->printToDesignHelper(nod,address,f1,isPart);
     }
   }

  f1<<"}"<<endl;
  //cout<<"Fine : Sono in disegno grafico della struttura"<<endl;

 }

// Segue la sintassi per creare un file testo per legare i vari nodi.
// Vedi WinGraph e programma vizuale.exe
template< class NODETYPE >
void itemSetCluster< NODETYPE >::printToDesignHelper(string here,string padre,ofstream& f1,int isPart)
 {
   typename MapType::iterator i;
   string files,label1,values,vStruct,nod,address,idList,myAncestor;
   //typename list<int>::iterator i2;
   //list<int> tList;


   label1="struct"+padre+"[shape=record,label=\""+padre+"|";
   values="";

   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
      idList="";
      values=values+"<"+i->first.c_str()+">"+i->first.c_str();

#if 1      // Stampa gli ancestor nel grafo generato
      typename vector<NODETYPE>::const_iterator it=getAncestor().begin();
      values += "A";
      for(;it!=getAncestor().end(); it++) {
	values += std::string(it->c_str())+"-";
	}
#endif

      idList=(i->second.getGidCidList()).getListToDesign();
       if (i->second.getHead()!=NULL)
        {
        std::stringstream oHead;
        oHead <<i->second.getHead();
        string nomeH=oHead.str();
         values=values+" : H_"+nomeH;

      }
       values += "|";
        values=values+":  "+idList+"|";
     }



    int siz=values.size();
    siz=siz-1;

   if (siz>0)    values.resize(siz);

   label1=label1+values+"\"];";
   f1<<label1<<endl;
   f1<<here<<"->"<<"struct"+padre<<endl;
   values="";
   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
   {

     nod=i->first.c_str();

     if (i->second.getHead()!=NULL)
     {
      std::stringstream oHead;
      oHead <<i->second.getHead();
      string nomeH=oHead.str();

       //cout<<"nome File head"<<nomeH<<endl;
       i->second.getHead()->printToDesign(nomeH,isPart);
     }

     if (i->second.getItemSet()!=NULL)
     {
      std::stringstream o2;
      o2 <<  i->second.getItemSet();
      address=o2.str();
      nod="struct"+padre+":"+nod;

      i->second.getItemSet()->printToDesignHelper(nod,address,f1,isPart);
     }

   }

 }


template< class NODETYPE >
void itemSetCluster< NODETYPE >::printItemSetRecursiveHead(int lev)
 {
   typename MapType::iterator i;

    //cout<<"dentro print"<<endl;
    for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
        //cout<<"Address "<<i<<endl;
       for (int j=0; j<lev;j++)
            cout<<"  ";
       cout<<getLevel()<<" Item "<<i->first<<" Ancestor "<<myAncestor<<" GidList: ";
       i->second.getGidCidList().print();


       if (i->second.getItemSet()!=NULL)
       {
        i->second.getItemSet()->printItemSetRecursiveHead(lev+1);
       }
    }
     //cout<<"fine print"<<endl;
 }


//------------------------- disegno GRAFICAMENTE la struttura : WinGraph //
template< class NODETYPE >
void itemSetCluster< NODETYPE >::printToDesign_2(string nome,int isPart)
 {

   /*string files,label1,values,nod,vStruct,address ,idList;*/
   typename MapType::iterator i;
   //typename list<int>::iterator i2;
   //list<int> tList;
   char filename[255];
   const char* nomeSt;
   //string myAncestor;

   //cout<<"Sono in disegno grafico della struttura"<<endl;
   nomeSt=nome.c_str();

   sprintf(filename,"design/LClust%s%d.txt",nomeSt,isPart);

   ofstream f1(filename);

   f1<<"digraph minerule { "<<endl;
   f1<<"graph[fontsize=8]; edge  [fontsize=8]; node  [fontsize=8]; ranksep = .30; nodesep = .25;"<<endl;
   f1<<"node [fontname=\"Courier\"]; "<<endl;
   f1<<"node [shape=record];"<<endl;

   string vStruct="struct0";
   string label1="struct0[shape=record,label=\"";
   string values;
   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
     {
      string idList;
      values=values+"<"+i->first+">"+i->first;
      if (i->second.getHead()!=NULL)
       {
        std::stringstream oHead;
        oHead <<i->second.getHead();
        string nomeH=oHead.str();
        values=values+" : H_"+nomeH;

     }
      if (i->second.getGidCidList().Csize()>0) idList=(i->second.getGidCidList()).getListToDesign();
      values=values+":  "+idList+"|";

     }

   int siz=values.size();
   siz=siz-1;
   if (siz>0)  values.resize(siz);
   label1=label1+values+"\"];";
   f1<<label1<<endl;
   //values="";


   for( i=isc.begin( ) ; i != isc.end( ) ; i++ )
   {

    string nod=i->first;
 /*     if (i->second.getHead()!=NULL)
     {
      std::stringstream oHead;
      oHead <<i->second.getHead();
      string nomeH=oHead.str();

       //cout<<"nome File head"<<nomeH<<endl;
       i->second.getHead()->printToDesign(nomeH,isPart);
     }
*/

     if ((i->second.getItemSet())!=NULL)
     {

      std::stringstream o2;
      o2 <<i->second.getItemSet();
      string address=o2.str();
      nod=vStruct+":"+nod;

      //i->second.getItemSet()->printToDesignHelper(nod,address,f1,isPart);
     }
   }

  f1<<"}"<<endl;
  //cout<<"Fine : Sono in disegno grafico della struttura"<<endl;
  //f1.close();
 }

#endif


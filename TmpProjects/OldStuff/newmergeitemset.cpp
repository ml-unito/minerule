class LevelInfoStack {
  HASHTYPE ancestor;
  vector<itemSetCluster<NODETYPE>*> levels;
  
public:
  size_t getCurLevel() {
    assert( ancestor.size() == levels.size() );
    return ancestor.size();
  }

  void
  push_back( NODETYPE newAncestor, itemSetCluster<NODETYPE>* newISC ) {
    ancestor.push_back(newAncestor);
    levels.push_back(newISC);
  }

  const HASHTYPE& 
  getAncestors() {
    return ancestor;
  }

  const itemSetCluster<NODETYPE>*
  getCurLevelInfo() {
    if(getCurLevel()==0)
      return NULL;
    else
      return levels.top();
  }

  void
  pop_back() {
    levels.pop_back();
    ancestor.pop_back();
  }
}

template< class NODETYPE >
void itemSetCluster< NODETYPE >::updatePtrList(				     
				     itemSetListCluster<NODETYPE>& ptrList,
				     int level,
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
   odbc::ResultSet* resultAllBody;
   level=0;
   String idHead("");

   // Il db del minerule è connesso!

   Qry="select level,"
     +srd.getBody().getSQLColumnNames()+",id_head from tmp_Rule_Ext order by id;";

   cerr << endl << "QRY:" << Qry << endl;

   HeadBodySourceRowDescription srDescription;
   srDescription.setBodyElems(2,srd.getBody().getColumnsCount());

   resultAllBody=coreConn.openQuery(Qry.c_str());
   //cout<<"dopo resultAllBody"<<endl;
   if (resultAllBody!=NULL)
     mergeItemSetHelper(ptrList,
			resultAllBody,
			srDescription,
			coreConn,
			srd);
   else cout<<"ResultAllBody == NULL"<<endl;
}

template< class NODETYPE >
int itemSetCluster< NODETYPE >::mergeItemSetHelper(
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
  levelStack.push_back(NODETYPE(), &isc);

  int lastLevel = 0;
  NODETYPE lastItem; 
  String lastHead;
  
  while( resultAllBody->next() ) {
    int level;
    NODETYPE item;
    String head; 
    

    // lettura dal db
    HeadBodySourceRow srow(resultAllBody, srDescription);

    lastLevel = level;
    level = atoi(resultAllBody->getString(1).c_str());

    lastItem = item;
    item = srow.getBody();

    lastHead = head;
    head = resultAllBody->getString(lastElem+1);

    while( level<levelStack.getCurLevel() ) {
      // torno indietro
      ancestorStack.pop_back();
    }
    
    if( level > levelStack.getCurLevel() ) {
      assert( level == levelStack.getCurLevel()+1 );
      typename MapType::iterator parentItem;
      assert( levelStack.getAncestor().size() > 0 );
            
      parentItem=levelStack.getLevelInfo().find(lastItem);
      assert(parentItem!=isc.end());
      itemSetCluster<NODETYPE>* newLevel = parentItem->second.getItemSet();
      if( newLevel == NULL ) {
	// there are no child items for parentItem... I have to build up
	// the new level
	
	newLevel=new itemSetCluster();
	levelStack.push_back(lastItem, newLevel);
	newLevel->setAncestor(levelStack.getAncestor());
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
    levelStack.getCurLevelInfo()->mergeItemSetHeadBase(coreConn,head,item, srd);
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::mergeItemSetHeadBase(
				sqlCoreConn& coreConn,
				String fHead,
				NODETYPE item,
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

   iscHead->mergeItemSetHead(coreConn,prtListH,fHead,srd);
   k->second.setHead(iscHead);
   k->second.setListPtrHead(prtListH);
}


template< class NODETYPE >
void itemSetCluster< NODETYPE >::mergeItemSetHead(
			    sqlCoreConn& coreConn,
			    itemSetListCluster<NODETYPE>& ptrList,
			    String fHead,
			    const SourceRowDescriptor& srd)
 {

   String QrySt;
   odbc::ResultSet* resultHead;
   size_t lastElem;

   // Il db del minerule è connesso!

   QrySt="select level,"+
     srd.getHead().getSQLColumnNames()+
     " from tmp_Rule_Head_Ext where id_head='"+fHead+"' order by id,id_head;";
   
   HeadBodySourceRowDescription srDescription;
   lastElem=srDescription.setHeadElems(2,srd.getHead().getColumnsCount());

   resultHead=coreConn.openQuery(QrySt.c_str());
   
   if (resultHead!=NULL)
     mergeItemSetHeadHelper(ptrList,
			    resultHead,
			    srDescription);

   else cout<<"ResultHead == NULL"<<endl;
}


template< class NODETYPE >
int itemSetCluster< NODETYPE >::mergeItemSetHeadHelper(
			itemSetListCluster<NODETYPE>& ptrList, 
			odbc::ResultSet* resultHead,
			const HeadBodySourceRowDescription& srDescription) {
  LevelInfoStack levelStack;
  levelStack.push_back(NODETYPE(), &isc);

  int lastLevel = 0;
  NODETYPE lastItem; 
  
  while( resultHead->next() ) {
    int level;
    NODETYPE item;
    String head; 
    

    // lettura dal db
    HeadBodySourceRow srow(resultHead, srDescription);

    lastLevel = level;
    level = atoi(resultHead->getString(1).c_str());

    lastItem = item;
    item = srow.getHead();

    while( level<levelStack.getCurLevel() ) {
      // torno indietro
      ancestorStack.pop_back();
    }
    
    if( level > levelStack.getCurLevel() ) {
      assert( level == levelStack.getCurLevel()+1 );
      typename MapType::iterator parentItem;
      assert( levelStack.getAncestor().size() > 0 );
            
      parentItem=levelStack.getLevelInfo().find(lastItem);
      assert(parentItem!=isc.end());
      itemSetCluster<NODETYPE>* newLevel = parentItem->second.getItemSet();
      if( newLevel == NULL ) {
	// there are no child items for parentItem... I have to build up
	// the new level
	
	newLevel=new itemSetCluster();
	levelStack.push_back(lastItem, newLevel);
	newLevel->setAncestor(levelStack.getAncestor());
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

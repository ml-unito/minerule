template< class NODETYPE >
void itemSetCluster< NODETYPE >::prune_children(
				   itemSetEntryClustr<NODETYPE>& toBePruned,
				   newItemSetHashMap<NODETYPE>& pHashMap,
				   newItemSetHashMap<NODETYPE>& pHashMapNew,
				   double nSup) {
  gidCidList<NODETYPE>& tList = toBePruned.getGidCidList();
  if( toBePruned.getItemSet()==NULL ) {
    return;
  }

  itemSetCluster<NODETYPE>* iscNew  = toBePruned.getItemSet();
  HASHTYPE myAncestor=iscNew->getAncestor();
  int level=iscNew->getLevel();
  MapType childMap=iscNew->getMap(); // 
  boolean oneOK = false;

  vector<NODETYPE> elemsToBeDeleted;
  typename MapType::iterator j;
  for( j=childMap.begin(); j !=childMap.end( ) ; j++ )
    {
      doApriori=pHashMap.findTies(myAncestor,j->first,level);

      if (doApriori) {
	  gidCidList<NODETYPE> tParentList,tListNew;

	  assert( isc.find(j->first)!=isc.end() );	  
	  tParentList=(isc.find(j->first))->second.getGidCidList();
	  tListNew=tParentList.set_intersection(tList);

	  if (  tListNew.isBigger(nSup) && 
		existHeadHavingSuffSupport(toBePruned, nSup) ) { 
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
	    //cout<<"ma non questo livello"<<endl;
	    eraseAllReferences(j->second, ptrList);
	    elemsToBeDeleted.push_back(j->first);
	    }
	} else 	{
	  // Non e' vero che tutti i miei ancestors hanno supporto sufficiente.
	  // non c'e' speranza che io possa averlo
	  eraseAllReferences(j->second, ptrList);
	  elemsToBeDeleted.push_back(j->first);
	}
    } // fine for

  // rimuoviamo fisicamente gli elementi dalla mappa.
  // N.B. Non puo' essere fatto prima perche' altrimenti l'iteratore "i"
  // verrebbe invalidato.

  vector<NODETYPE>::iterator it;
  for(it=elemsToBeDeleted.begin(); it!=elemsToBeDeleted.end(); it++) {
    isc.erase(*it);
  }
  
  // posso cancellare questo livello, non ci sono item a supporto sufficiente
  if (!oneOK) {
    // Il codice seguente corrisponde a:
    // removeAllReferences(i->second, ptrList,false)
    // tranne per il fatto che removeAllReferences rimuove un riferimento
    // a livello  nLev=(toBePruned.getItemSet()->getLevel()), mentre il
    // codice sottostante lo fa per nLev=(i->second.getItemSet()->getLevel())+1
    
    //cout<<"cancello iscNew"<<iscNew<<" pttrC di i "<<i->second.getItemSet()<<endl;
    
    assert( toBePruned.getItemSet()!=NULL );
    
    
    int nLev=(toBePruned.getItemSet()->getLevel())+1;
    listPtr=ptrList.getList(nLev);
    
    toBePruned.getItemSet()->remove(ptrList);
    toBePruned.cancellaRef(listPtr);
    delete(toBePruned.getItemSet());
    toBePruned.setItemSetChild(NULL);
  }
}

template< class NODETYPE >
boolean itemSetCluster< NODETYPE >::gen_final_count_HeadHelper(
              itemSetListCluster<NODETYPE>& ptrList,
	      double nSup,
	      int lev,
	      newItemSetHashMap<NODETYPE>& pHashMap,
	      newItemSetHashMap<NODETYPE>& pHashMapNew)
 {
   typename MapType::iterator i;

   for( i=isc.begin( ) ; i != isc.end( ) ; i++ ) {
     prune_head_children(i->second, pHashMap, pHashMapNew, nSup );
   }
   
   return true;
 }


// ---------------------------------------------------


template< class NODETYPE >
boolean itemSetCluster< NODETYPE >::gen_final_count_Helper(
              itemSetListCluster<NODETYPE>& ptrList,
	      double nSup,
	      int lev,
	      newItemSetHashMap<NODETYPE>& pHashMap,
	      newItemSetHashMap<NODETYPE>& pHashMapNew)
 {
   typename MapType::iterator i;

   for( i=isc.begin( ) ; i != isc.end( ) ; i++ ) {
     prune_children(i->second, pHashMap, pHashMapNew, nSup );
   }
   
   return true;
 }

template< class NODETYPE >
boolean itemSetCluster< NODETYPE >::existHeadHavingSuffSupport(
			            itemSetEntryClustr<NODETYPE>& bodyElem,
				    double nSup) {
  if( bodyElem.getHead()==NULL ) {
    return true;
  }

  return  bodyElem.getHead()->check_and_gen(bodyElem.getListPtrHead(),nSup);
}

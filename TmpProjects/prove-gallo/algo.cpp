#include "Optimizer/QueryResultIterator.h"
#include "classi.h"
#include "algo.h"
#include "MRDatabase/itemtype.h"
#include <iostream>
#include "Optimizer/OptimizerCatalogue.h"
#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Utils/MineruleOptions.h"
#include "MRDatabase/sourcerowelement.h"
#include "MRDatabase/MIndex.h"
#include "SubsetIterator.h"
#include <iterator>

using namespace std;
namespace minerule {


//per algo distruttivo: devo mettere nella struttura anche i supporti
Body* DestrTree::insertRulesInStructure(const string& queryname,int ngroups){
  QueryResultIterator qit;
  OptimizerCatalogue::getMRQueryResultIterator(queryname, qit);
  Head* newhead;
  while( qit.next()) {
    QueryResultIterator::Rule r;
    //crea body e head che sono due vettori ItemSetType
    //ovvero due vettori di ItemType
    //ogni ItemType contiene un puntatore ad un SourceRowElement
    qit.getRule(r);
    //body e head sono due puntatori a ItemSetType
    //moltiplico per il num dei gruppi
    //double suppr=r.support*9983;
    //double suppb=suppr*r.confidence;
    //aggiungi cast per int
    newhead=root->insertItemSetB(*r.body,round((r.support*ngroups)/r.confidence));
    newhead->insertItemSetH(*r.head,round(r.support*ngroups));
   }
 return root;
}


//per algo costruttivo: nella struttura non metto i supporti a zero
Body* ConstrTree::insertRulesInStructure(const string& queryname, int ng){
  QueryResultIterator qit;
  OptimizerCatalogue::getMRQueryResultIterator(queryname, qit);
  Head* newhead;
  ngroups=ng;
  while( qit.next()) {
    QueryResultIterator::Rule r;
    //crea body e head che sono due vettori ItemSetType
    //ovvero due vettori di ItemType
    //ogni ItemType contiene un puntatore ad un SourceRowElement
    qit.getRule(r);
    //insertRuleInStructure(r);
    //body e head sono due puntatori a ItemSetType
    newhead=root->insertItemSetB(*r.body,0.0);
    newhead->insertItemSetH(*r.head,0.0);
   }
 return root;
}

//creo le combinazioni di bodies e le cerco nell'albero
//se le trovo incremento il loro supporto
void ConstrTree::buildAndFindBodies(ItemSetType& body){
 SubsetIterator it(body);
 while(it.next()){
   ItemSetType curBody;
   it.getCurrentSet(curBody);
   NodeRowB* ris=root->findBodyInTree(&curBody);
   if (ris!=NULL) {ris->incremSupp();
     //cout<<"suppb incrementato"<<std::endl;
   }
 }
}

//creo le combinazioni di rules e le cerco nell'albero
//se le trovo incremento il loro supporto
void ConstrTree::buildAndFindRules(ItemSetType& body,ItemSetType& heads){
 ItemSetType curHeadSet;
 std::insert_iterator< ItemSetType > ins_it(curHeadSet, curHeadSet.begin());
 set_difference(heads.begin(), heads.end(),
                body.begin(),body.end(),
		ins_it);
 SubsetIterator it(curHeadSet);
 while(it.next()){
   ItemSetType curHead;
   it.getCurrentSet(curHead);
   //restituisce un punt all'ultimo item della head della regola (se l'ha trovata)
   NodeRow* ris=root->findRuleInTree(&body,&curHead);
   if (ris!=NULL){ ris->incremSupp();
   //cout<<"suppr incrementato"<<std::endl;
   }
 }
}



void ConstrTree::adjustSupp(MIndexIterator* b2,MIndexIterator* h2){
   //if (b2!=NULL)&&(h2!=NULL){//inserisci questo controlla prima della chiamata a qsta funz

     for (; b2->current() != b2->end(); ) {
       string g=b2->getCurrentGID();
       ItemSetType* body=new ItemSetType();
       cout<<"in adjustSupp:"<<std::endl;
       for(; b2->current() != b2->end() && b2->getCurrentGID()==g;(*b2)++){
         ItemType gid(*SourceRowElement::deserializeElementFromString("n "+b2->getCurrentGID()));
	 ItemType item(*SourceRowElement::deserializeElementFromString("n "+b2->getCurrentItem()));
	 cout<<item.asString()<<" "<<gid.asString()<<std::endl;
	 body->push_back(item);
       }
       buildAndFindBodies(*body);
       if (g==h2->getCurrentGID()){
         ItemSetType* head=new ItemSetType();
         for(;h2->current() != h2->end() && h2->getCurrentGID()==g; (*h2)++){
         ItemType item(*SourceRowElement::deserializeElementFromString("n "+h2->getCurrentItem()));
         head->push_back(item);
         }
         SubsetIterator si(*body);
	 while(si.next()){
	   ItemSetType result;
	   si.getCurrentSet(result);
           buildAndFindRules(result,*head);
	 }delete head;
      }

     delete body;
     }
 //}

}



#if 0

//creo le combinazioni di bodies e le cerco nell'albero
//se le trovo incremento il loro supporto
void DestrTree::buildAndFindBodies(ItemSetType& body){
 ItemSetType curBodySet;
 SubsetIterator it(curBodySet);
 while(it.next()){
   ItemSetType curBody;
   it.getCurrentSet(curBody);
   NodeRowB* ris=root->findBodyInTree(&curBody);
   if (ris!=NULL) ris->decremSupp();
 }
}

//creo le combinazioni di rules e le cerco nell'albero
//se le trovo incremento il loro supporto
void DestrTree::buildAndFindRules(ItemSetType& body,ItemSetType& heads){
 ItemSetType curHeadSet;
 std::insert_iterator< ItemSetType > ins_it(curHeadSet, curHeadSet.begin());
 set_difference(heads.begin(), heads.end(),
                body.begin(),body.end(),
		ins_it);
 SubsetIterator it(curHeadSet);
 while(it.next()){
   ItemSetType curHead;
   it.getCurrentSet(curHead);
   //restituisce un punt all'ultimo item della head della regola (se l'ha trovata)
   NodeRow* ris=root->findRuleInTree(&body,&curHead);
   if (ris!=NULL) ris->incremSupp();
 }
}






void DestrTree::adjustSupp(MIndexIterator* b1notb2,MIndexIterator* h1noth2,
                           MIndexIterator* b1,MIndexIterator* h1){
   //if (b2!=NULL)&&(h2!=NULL){//inserisci questo controlla prima della chiamata a qsta funz
     for (; b1notb2->current() != b1notb2->end(); (*b1notb2)++) {
       string g=b1notb2->getCurrentGID();
       //createItemset crea tutte le possibili combinazioni di itemset in g
       ItemSetType* body=new ItemSetType();
       while(b1notb2->getCurrentGID()==g){
         ItemType item(*SourceRowElement::deserializeElementFromString("g @[ "+b2->getCurrentItem()+" @]"));
         body->push_back(item);
	 b1notb2++;
       }
       buildAndFindBodies(*body);
       if (g==h2->getCurrentGID()){
         ItemSetType* head=new ItemSetType();
         while(h2->getCurrentGID()==g){
         ItemType item(*SourceRowElement::deserializeElementFromString("g @[ "+h2->getCurrentItem()+" @]"));
         head->push_back(item);
	 h2++;
         }
         SubsetIterator si(*body);
	 while(si.next()){
	   ItemSetType result;
	   si.getCurrentSet(result);
           buildAndFindRules(result,*head);
	 }
      }
   }
 //}

}
#endif



}//namespace



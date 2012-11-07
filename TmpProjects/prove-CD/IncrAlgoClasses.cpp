#include "IncrAlgoClasses.h"
#include "MRDatabase/itemtype.h"
#include <iostream>
#include <algorithm>
#include "Optimizer/OptimizerCatalogue.h"
#include "Algorithms/sqlCoreConn.h"
#include <iterator>


namespace minerule {

 size_t Body::countb=0;
 size_t Head::counth=0;

 Body::Body(){
  NRB=new RowBContainer;
  ItemType* tmp=new ItemType();
  ancestor=*tmp;
  countb++;
  delete tmp;
 }

 Body::~Body(){
    countb--;
    RowBContainer::iterator it;
    it=NRB->begin();
    while(it!=NRB->end()){
      delete (it->second);
      it++;
    }
    delete NRB;
  }

 Head::Head(){
  NR=new RowContainer;
  counth++;
 }

 Head::~Head(){
    counth--;
    RowContainer::iterator it;
    it=NR->begin();
    while(it!=NR->end()){
      delete (it->second);
      it++;
    }
    delete NR;
  }


 //INSERIMENTO DI ItemSet BODY

 //funzione ricorsiva
 Head* Body::insertItemSetB(ItemSetType:: iterator ite,
                           ItemSetType:: iterator itend, double s ){
  RowBContainer::iterator itr;
  itr=NRB->find(*ite);
  Head* hr=NULL;
  //se non c'e' lo inserisco
  if(itr==NRB->end()){
   (*NRB)[*ite]=new NodeRowB();
   itr=NRB->find(*ite);
   assert(itr!=NRB->end());
   //cout<<"body "<<itr->first<<std::endl;

   ite++;
   if(ite!=itend){
     Body* child=itr->second->setChild();
     child->setAncestor(itr->first);
     hr=child->insertItemSetB(ite,itend,s);
     return hr;
    }else itr->second->setSupp(s);
  }
  //se l'ho trovato
  else{
   //cout<<"body "<<itr->first<<std::endl;
   ite++;
   //se ci sono ancora figli da inserire
   if(ite!=itend){
    Body* child=itr->second->getChild();
    //se ha gia' figli
    if(child!=NULL){
     hr=child->insertItemSetB(ite,itend,s);
     return hr;}
    //se non ha figli per Rob:es.se ho inserito A->B e poi voglio inserire AC->F A non ha figli
    else{
     Body* child=itr->second->setChild();
     hr=child->insertItemSetB(ite,itend,s);
     return hr;
     }
   }else itr->second->setSupp(s);
  }
  if(ite==itend) {
   Head* h=itr->second->setHead();
   h->setAncestor(itr->first);
   return h;
  }
  return hr;
 }


 Head* Body::insertItemSetB(ItemSetType& SREV,double supp){
  ItemSetType:: iterator ite=SREV.begin();
  ItemSetType:: iterator itend=SREV.end();
  Head* h;
  h=insertItemSetB(ite, itend, supp);
  return h;
 }


 //INSERIMENTO DI ItemSet HEAD

 //funzione ricorsiva
 void Head::insertItemSetH(ItemSetType:: iterator ite,
                           ItemSetType:: iterator itend, double supp){
  RowContainer::iterator itr;
  itr=NR->find(*ite);

  //se non c'e' lo inserisco
  if(itr==NR->end()){
   (*NR)[*ite]=new NodeRow();
   itr=NR->find(*ite);
   //cout<<"                head "<<itr->first<<std::endl;
   //se ci sono ancora elementi nel vettore, vuol dire che devo inserire i figli
   ite++;
   if(ite!=itend){
     Head* tmph=itr->second-> setChild();
     tmph->setAncestor(itr->first);
     tmph->insertItemSetH(ite,itend,supp);
     }
  }

  //se l'ho trovato
  else{
   //cout<<"                head "<<itr->first<<std::endl;
   //se ci sono ancora figli da inserire
   ite++;
   if(ite!=itend){
    Head* child=itr->second->getChild();
     //se ha gia' figli
    if(child!=NULL)
     child->insertItemSetH(ite,itend,supp);
    //se non ha figli
    else{
     Head* child=itr->second->setChild();
     child->insertItemSetH(ite,itend,supp);
     }
   }
  }
  itr->second->setSupp(supp);
}

 void Head::insertItemSetH(ItemSetType& SREV,double supp){
  ItemSetType:: iterator ite=SREV.begin();
  ItemSetType:: iterator itend=SREV.end();
  insertItemSetH(ite,itend,supp);
 }



 //funzione ricorsiva
 void Body::findBodiesInTree(ItemSetType* body){

  RowBContainer::iterator ite=NRB->begin();
  ItemSetType::iterator itfind;
  while(ite!=NRB->end()){
    itfind=find(body->begin(),body->end(),ite->first);
    //se l'ho trovato
    if (itfind!=body->end()) {
      ite->second->incremSupp();
      cout<<"ho increm supp di body"<<std::endl;
      Body* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) c->findBodiesInTree(body);
    }
  ite++;
  }
 }



  //funzione ricorsiva
 void Head::findHead(ItemSetType* head){
  RowContainer::iterator ite=NR->begin();
  ItemSetType::iterator itfind;
  //  ItemSetType::iterator prova=head->begin();
  /*  cout<<"elementi in cui faccio ricerca:"<<std::endl;
  while(prova!=head->end()){
  cout<<(*prova)<<", ";
  prova++;
  }
  cout<<std::endl;*/

  while(ite!=NR->end()){
    //    cout<<"ora cerco "<<ite->first<<std::endl;
    itfind=find(head->begin(),head->end(),ite->first);
    if(itfind!=head->end()) {//se l'ho trovato
      ite->second->incremSupp();
      //cout<<"ho increm supp di rule"<<std::endl;
      Head* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) {
	c->findHead(head);
      }
    }//else cout<<"non trovato"<<std::endl;
  ite++;
  }
 }


 void Body::findRulesInTree(ItemSetType* body,ItemSetType* head){
  

   //ItemSetType:: iterator itb=body->begin();
   //ItemSetType:: iterator ity=body->end();

  RowBContainer::iterator ite=NRB->begin();
  ItemSetType::iterator itfind;
  while(ite!=NRB->end()){
    itfind=find(body->begin(),body->end(),ite->first);
    if(itfind!=body->end()) {//se l'ho trovato
      ite->second->incremSupp();
      //cout<<"ho increm suppb da findRules.."<<std::endl;
      Head* h=ite->second->getHead();

      /*      cout << "heads:";
      copy( head->begin(),
	    head->end(),
	    std::ostream_iterator<ItemType>(cout, " "));
	    cout << "end";*/
	    

      //cout<<h<<std::endl;
      if (h!=NULL) {
	h->findHead(head);
      }
      Body* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) {
	c->findRulesInTree(body,head);
      }
    }
  ite++;
  }
 }



void Head::extractRules(const vector<ItemType>& body,
			vector<ItemType>& head,
			double thrR, double thrB, double suppB, int ngroups, 
			sqlCoreConn* pcoreConn){
 typedef std::map<ItemType,NodeRow*> RowContainer;

 RowContainer::iterator it;
 Head* htmp;
 double s;
 double tmp;
 it=this->NR->begin();
 while (it!=this->NR->end()){
   htmp=it->second->getChild();
   //se ha figli richiamo la funzione
   if (htmp!=NULL){
      head.push_back(it->first);
      htmp->extractRules(body,head,thrR,thrB,suppB,ngroups,pcoreConn);
      s=it->second->getSupp();
      double sup=s/ngroups;
      tmp=(s/suppB);
      if((sup>=thrR)&&(tmp>=thrB)){
	/*        cout<<"body: "<<std::endl;
        vector<ItemType>::const_iterator itv=body.begin();
        while (itv!=body.end()){
          cout<<" "<<(*itv)<<std::endl;
	  itv++;
	  }
        cout<<"suppb: "<<tmp<<std::endl;
        cout<<"       head: "<<std::endl;
        itv=head.begin();
        while (itv!=head.end()){
          cout<<"               "<<(*itv)<<std::endl;
	  itv++;
	  }
        cout<<"suppr: "<<sup<<std::endl;
	*/
	pcoreConn->insert_DB(body,head,sup,tmp);
      }
   } else { //se invece non ne ha stampo la regola se conf e supp sono sufficienti
     head.push_back(it->first);
     s=it->second->getSupp();
     double sup=s/ngroups;
     tmp=(s/suppB);
     if((sup>=thrR)&&(tmp>thrB)){
       /* cout<<"body: "<<std::endl;
       vector<ItemType>::const_iterator itv=body.begin();
       while (itv!=body.end()){
	 cout<<" "<<(*itv)<<std::endl;
	 itv++;
       }
       cout<<"suppb: "<<tmp<<std::endl;
       cout<<"       head: "<<std::endl;
       itv=head.begin();
       while (itv!=head.end()){
	 cout<<"               "<<(*itv)<<std::endl;
	 itv++;
       }
       cout<<"suppr: "<<sup<<std::endl;*/
       pcoreConn->insert_DB(body,head,sup,tmp);
     }
   }
   head.pop_back();
   it++;
 }

}



void Body::extractRules(vector<ItemType>& body,
			double thrR, double thrB, int ngroups,
			sqlCoreConn* pcoreConn){
 typedef std::map<ItemType,NodeRowB*> RowBContainer;

 RowBContainer::iterator itB;
 Body* btmp;
 itB=this->NRB->begin();
 vector<ItemType> head;
 while (itB!=this->NRB->end()){
   double suppb=itB->second->getSupp();
   body.push_back(itB->first);
   Head* h=itB->second->getHead();
   if (h!=NULL){
	itB->second->getHead()->extractRules(body,head,thrR,thrB,suppb,ngroups,pcoreConn);
   }
   btmp=itB->second->getChild();
   if (btmp!=NULL){
     btmp->extractRules(body,thrR,thrB,ngroups,pcoreConn);
   }
   body.pop_back();
   itB++;
 }
}

void Body::provaStampaLiv1(){
 RowBContainer::iterator it=NRB->begin();
 while(it!=NRB->end()){
   cout<<"elemento "<<it->first<<" "<<std::endl;
   cout<<"child: "<<it->second->getChild()<<std::endl;
   cout<<"head: "<<it->second->getHead()<<std::endl;
   it++;
 }

}





}//namespace











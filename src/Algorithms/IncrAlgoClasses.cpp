#include "Algorithms/IncrAlgoClasses.h"
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
   //cout<<"body "<<itr->first<<endl;

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
   //cout<<"body "<<itr->first<<endl;
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
   itr->second->setSupp(supp);
   //cout<<"                head "<<itr->first<<endl;
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
   //cout<<"                head "<<itr->first<<endl;
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
   }else itr->second->setSupp(supp);
  }
  //itr->second->setSupp(supp);
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
      //cout<<"ho increm supp di body"<<endl;
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
  /*  cout<<"elementi in cui faccio ricerca:"<<endl;
  while(prova!=head->end()){
  cout<<(*prova)<<", ";
  prova++;
  }
  cout<<endl;*/

  while(ite!=NR->end()){
    //    cout<<"ora cerco "<<ite->first<<endl;
    itfind=find(head->begin(),head->end(),ite->first);
    if(itfind!=head->end()) {//se l'ho trovato
      ite->second->incremSupp();
      //cout<<"ho increm supp di rule"<<endl;
      Head* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) {
	c->findHead(head);
      }
    }//else cout<<"non trovato"<<endl;
  ite++;
  }
 }

//questa serve per l'algo incrementale distruttivo
//arrivo qui quando il body della regola non soddisfa B2, quindi non mi interessa piu' che
//H2 sia soddisfatto
 void Head::findHead2(ItemSetType* h1){
  RowContainer::iterator ite=NR->begin();
  ItemSetType::iterator itfind;
  while(ite!=NR->end()){
    //cout<<"esamino head "<<ite->first<<endl;
    itfind=find(h1->begin(),h1->end(),ite->first);
    if(itfind!=h1->end()) {//se l'ho trovato
      ite->second->decremSupp();
      //cout<<"decremento head "<<ite->first<<endl;
      Head* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) {
	c->findHead2(h1);
      }
    }//else cout<<"non trovato"<<endl;
  ite++;
  }
 }

//questa serve per l'algo incrementale distruttivo
//arrivo qui quando il body soddisfa la regola
 void Head::findHead2bis(ItemSetType* h1nh2,ItemSetType* h1){
  RowContainer::iterator ite=NR->begin();
  ItemSetType::iterator itfind;
  while(ite!=NR->end()){
    //cout<<"esamino head "<<ite->first<<endl;
    itfind=find(h1nh2->begin(),h1nh2->end(),ite->first);
    if(itfind!=h1nh2->end()) {//se l'ho trovato
      ite->second->decremSupp();
      //cout<<"decremento head "<<ite->first<<endl;
      Head* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) {
	c->findHead2(h1);
      }
    }else {
      itfind=find(h1->begin(),h1->end(),ite->first);
      if(itfind!=h1->end()) {//se l'ho trovato
       Head* c=ite->second->getChild();
       //se ha figli
       if (c!=NULL) {
	 c->findHead2bis(h1nh2,h1);
       }
      }
     }
  ite++;
  }
 }



//questa serve per l'algo incrementale distruttivo
//funzione ricorsiva
 void Body::findChildInTree(ItemSetType* b1, ItemSetType* h1){

  RowBContainer::iterator ite=NRB->begin();
  ItemSetType::iterator itfind;
  //cout<<"esamino body "<<ite->first<<endl;
  while(ite!=NRB->end()){
    itfind=find(b1->begin(),b1->end(),ite->first);
    //se l'ho trovato
    if (itfind!=b1->end()) {
      ite->second->decremSupp();
      //cout<<"                      decremento body "<<ite->first<<endl;
      //cout<<"ho decrem supp di body"<<endl;
      Head* h=ite->second->getHead();
      if (h!=NULL) {
	h->findHead2(h1);
      }
      Body* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) c->findChildInTree(b1,h1);
    }
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
      //cout<<"ho decrem suppb da findRules.."<<endl;
      Head* h=ite->second->getHead();

      /*      cout << "heads:";
      copy( head->begin(),
	    head->end(),
	    ostream_iterator<ItemType>(cout, " "));
	    cout << "end";*/


      //cout<<h<<endl;
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


//questa serve per l'algo incrementale distruttivo
 void Body::findRulesInTree(ItemSetType* b1,ItemSetType* b1nb2,
                            ItemSetType* h1,ItemSetType* h1nh2){
  RowBContainer::iterator ite=NRB->begin();
  ItemSetType::iterator itfind;
  while(ite!=NRB->end()){
    //cout<<"esamino body "<<ite->first<<endl;
    itfind=find(b1nb2->begin(),b1nb2->end(),ite->first);

    if(itfind!=b1nb2->end()) {//se l'ho trovato
      ite->second->decremSupp();
      //cout<<"                      decremento body "<<ite->first<<endl;
      Head* h=ite->second->getHead();
      if (h!=NULL) {
	h->findHead2(h1);
      }
      Body* c=ite->second->getChild();
      //se ha figli
      if (c!=NULL) {
	c->findChildInTree(b1,h1);
      }

    }else{//non l'ho trovato
      itfind=find(b1->begin(),b1->end(),ite->first);
      if(itfind!=b1->end()) {//se non soddisfa B1andnotB2 e soddisfa B1 allora soddisfa B2
        Head* h=ite->second->getHead();
        if (h!=NULL) {
	  h->findHead2bis(h1nh2,h1);
        }
        Body* c=ite->second->getChild();
        //se ha figli
        if (c!=NULL) {

	  c->findRulesInTree(b1,b1nb2,h1,h1nh2);
        }
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
      if((sup>=thrR)/*&&(tmp>=thrB)*/){
	/*        cout<<"body: "<<endl;
        vector<ItemType>::const_iterator itv=body.begin();
        while (itv!=body.end()){
          cout<<" "<<(*itv)<<endl;
	  itv++;
	  }
        cout<<"suppb: "<<tmp<<endl;
        cout<<"       head: "<<endl;
        itv=head.begin();
        while (itv!=head.end()){
          cout<<"               "<<(*itv)<<endl;
	  itv++;
	  }
        cout<<"suppr: "<<sup<<endl;
	*/
	pcoreConn->insert_DB(body,head,sup,tmp);
      }
   } else { //se invece non ne ha stampo la regola se conf e supp sono sufficienti
     head.push_back(it->first);
     s=it->second->getSupp();
     double sup=s/ngroups;
     tmp=(s/suppB);
     if((sup>=thrR)/*&&(tmp>thrB)*/){
       // cout<<"body: "<<endl;
       //vector<ItemType>::const_iterator itv=body.begin();
       //while (itv!=body.end()){
	// cout<<" "<<(*itv)<<endl;
	// itv++;
      // }
      // cout<<"suppb: "<<tmp<<endl;
      // cout<<"       head: "<<endl;
      // itv=head.begin();
     //  while (itv!=head.end()){
	// cout<<"               "<<(*itv)<<endl;
	// itv++;
      // }
      // cout<<"suppr: "<<sup<<endl;
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
   cout<<"elemento "<<it->first<<" "<<endl;
   cout<<"child: "<<it->second->getChild()<<endl;
   cout<<"head: "<<it->second->getHead()<<endl;
   it++;
 }

}





}//namespace










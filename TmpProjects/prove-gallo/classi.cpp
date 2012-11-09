#include "classi.h"
#include "Database/ItemType.h"
#include <iostream>

#include "Optimizer/OptimizerCatalogue.h"
#include "Database/Connection.h"



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
   //se ci sono ancora elementi nel vettore, vuol dire che devo inserire i figli
   //sicuramente il NodeRow appena inserito non ha figli quindi non faccio controllo su child==NULL
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
 NodeRowB* Body::findBody(ItemSetType:: iterator ite,
                          ItemSetType:: iterator itend){
  RowBContainer::iterator itr;
  //stampa di prova
  /*itr=NRB->begin();
  cout<<"stampa di prova"<<std::endl;
  std::string str;
  while(itr!=NRB->end()){
    SourceRowElement::serializeElementToString(itr->first.getElement(),str);
    cout << str <<std::endl;
    itr++;
  }*/

  //cout<<"ora cerco in body "<<(*ite)<<std::endl;
  itr=NRB->find(*ite);
  NodeRowB* nrb;
  //se non c'e' ritorno NULL
  if(itr==NRB->end()){
   return NULL;
  }
  //se l'ho trovato
  else{
   ite++;
   //se ci sono ancora item da cercare
   if(ite!=itend){
    Body* child=itr->second->getChild();
    //se ha figli
    if(child!=NULL){
     nrb=child->findBody(ite,itend);
     return nrb;}
    //se non ha figli ritorno NULL
    else{
     return NULL;
     }
   }else return itr->second;
  }
 }

 NodeRowB* Body::findBodyInTree(ItemSetType* itemset_body){
  ItemSetType:: iterator ite=itemset_body->begin();
  ItemSetType:: iterator itend=itemset_body->end();
  NodeRowB* nrb;
  nrb=this->findBody(ite, itend);
  return nrb;
 }



  //funzione ricorsiva
 NodeRow* Head::findHead(ItemSetType:: iterator ite,
                         ItemSetType:: iterator itend){
  RowContainer::iterator itr;
  //cout<<"ora cerco in head "<<(*ite)<<std::endl;
  itr=NR->find(*ite);
  NodeRow* nrb;
  //se non c'e' ritorno NULL
  if(itr==NR->end()){
   return NULL;
  }
  //se l'ho trovato
  else{
   ite++;
   //se ci sono ancora item da cercare
   if(ite!=itend){
    Head* child=itr->second->getChild();
    //se ha figli
    if(child!=NULL){
     nrb=child->findHead(ite,itend);
     return nrb;}
    //se non ha figli ritorno NULL
    else{
     return NULL;
     }
   }else return itr->second;
  }
 }

 NodeRow* Body::findRuleInTree(ItemSetType* itemset_body,ItemSetType* itemset_head){
  ItemSetType:: iterator ite;
  ItemSetType:: iterator itend;
  NodeRowB* nrb=NULL;
  NodeRow* nr=NULL;
  nrb=this->findBodyInTree(itemset_body);
  if (nrb!=NULL){
     Head* h=nrb->getHead();
     if (h!=NULL) {
        ite=itemset_head->begin();
        itend=itemset_head->end();
        nr=h->findHead(ite,itend);
     }
  }else return nrb;
  return nr;
 }


void Head::extractRules(const vector<ItemType>& body,
                  vector<ItemType>& head,
		  double thrR, double thrB, double suppB, int ngroups,sqlCoreConn* pcoreConn){
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
      if((sup>=thrR)&&(tmp>thrB)){
        //cout<<"body: "<<std::endl;
        /*vector<ItemType>::const_iterator itv=body.begin();
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

	pcoreConn->insert_DB(body,head,tmp,sup);
      }
   }//se invece non ne ha stampo la regola se conf e supp sono sufficienti
   else {
      head.push_back(it->first);
      s=it->second->getSupp();
      double sup=s/ngroups;
      tmp=(s/suppB);
      if((sup>=thrR)&&(tmp>thrB)){
        /*cout<<"body: "<<std::endl;
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
	pcoreConn->insert_DB(body,head,tmp,sup);
      }
    }
   head.pop_back();
   it++;
 }

}



void Body::extractRules(vector<ItemType>& body,
	       double thrR, double thrB, int ngroups,sqlCoreConn* pcoreConn){
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

}//namespace











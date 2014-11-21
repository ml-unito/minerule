


#ifndef CLASSI_H
#define CLASSI_H
#include "minerule/Database/ItemType.hpp"
#include <vector>
#include <iostream>
#include <map>
#include "minerule/Database/Connection.hpp"

namespace minerule {

 class NodeRow;
 class NodeRowB;
 class Head;

 /*******************class Body******/
 class Body
 {public:
  typedef std::map<ItemType,NodeRowB*> RowBContainer;
  
  private:
   ItemType ancestor;
   NodeRowB* findBody(ItemSet:: iterator ite,
                      ItemSet:: iterator itend);
   NodeRowB* findHead(ItemSet:: iterator ite,
                          ItemSet:: iterator itend);
   Head* insertItemSetB(ItemSet::iterator ite,
                       ItemSet::iterator itend,
		       double d);
  public:
  static size_t countb;
  Body();
  ~Body();
  
  RowBContainer* NRB;

  void setAncestor(ItemType b){ancestor=b;}
  ItemType getAncestor(){return ancestor;}
  Head* insertItemSetB(ItemSet& SREV, double supp);
  NodeRowB* findBodyInTree(ItemSet* itemset_body);
  NodeRow* findRuleInTree(ItemSet* itemset_body,ItemSet* itemset_head);
  void extractRules(std::vector<ItemType>& body,
	       double thrR, double thrB,int ngroups,Connection* pconnection);

 };

 /*******************class Head******/
 class Head
 {public:
  typedef std::map<ItemType,NodeRow*> RowContainer;
  
  private:
   ItemType ancestor;

  public:
   static size_t counth;
   Head();
   ~Head();

   RowContainer* NR;
   void setAncestor(ItemType h){ancestor=h;}
   ItemType getAncestor(){return ancestor;}
   void insertItemSetH(ItemSet& SREV, double supp);
   void insertItemSetH(ItemSet:: iterator ite,
                      ItemSet:: iterator itend,
		      double s);
   void extractRules(const std::vector<ItemType>& body,
                  std::vector<ItemType>& head,
		  double thrR, double thrB, double suppB, int ngroups,Connection* pconnection);
   NodeRow* findHead(ItemSet:: iterator ite,
                     ItemSet:: iterator itend);
 };


 /*******************class NodeRow******/

 class NodeRow{

  private:
  Head* child;
  double supp;
  //marca per algo distr
  bool marked;
  
  public:
  NodeRow() : child(NULL),supp(0) {marked=0;}
  NodeRow(const NodeRow& nr) : supp(nr.supp) {}
  ~NodeRow(){delete child;}

  double getSupp(){return supp;}
  void setSupp(double& s){supp=s;}
  void incremSupp(){supp++;}
  void decremSupp(){supp--;}
  Head* getChild() {return child;}
  Head* setChild() {child = new Head(); return child;}
  void mark(){marked=1;}
  void demark(){marked=0;}
  bool isMarked(){return marked;}
 };



 /*******************class NodeRowB******/

 //classe per le righe del Body
 //ogni riga del Body potrebbe avere un puntatore ad un Node Head
 //nota che una riga potrebbe anche non avere un puntatore head.In questo caso
 //avrebbe almeno un figlio...(caso con |body|>1)

 class NodeRowB : public NodeRow
 {
  private:
  Head* head;
  Body* child;
  public:
  NodeRowB(): head(NULL),child(NULL){ }
  NodeRowB(const NodeRowB& nr){}
  ~NodeRowB(){delete head; delete child;}
  Body* getChild() {return child;}
  Body* setChild() {child = new Body(); return child;}
  Head* setHead() {
    if (head==NULL)
      head = new Head();
    return head;}
  Head* getHead() {return head;}

 };



}
#endif

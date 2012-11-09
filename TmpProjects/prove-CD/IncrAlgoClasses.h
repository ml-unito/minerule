


#ifndef INCRALGOCLASSI_H
#define INCRALGOCLASSI_H
#include "Database/ItemType.h"
#include <vector>
#include <iostream>
#include <map>
#include "Algorithms/sqlCoreConn.h"

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
   NodeRowB* findBody(ItemSetType:: iterator ite,
                      ItemSetType:: iterator itend);
   NodeRowB* findHead(ItemSetType:: iterator ite,
                          ItemSetType:: iterator itend);
   Head* insertItemSetB(ItemSetType::iterator ite,
                       ItemSetType::iterator itend,
		       double d);
  public:
  static size_t countb;
  Body();
  ~Body();
  
  RowBContainer* NRB;

  void setAncestor(ItemType b){ancestor=b;}
  ItemType getAncestor(){return ancestor;}
  Head* insertItemSetB(ItemSetType& SREV, double supp);
  void findBodiesInTree(ItemSetType* body);
  void findRulesInTree(ItemSetType* itemset_body,ItemSetType* itemset_head);
  void extractRules(std::vector<ItemType>& body,
	       double thrR, double thrB,int ngroups,sqlCoreConn* pcoreConn);
  void provaStampaLiv1();
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
   void insertItemSetH(ItemSetType& SREV, double supp);
   void insertItemSetH(ItemSetType:: iterator ite,
                      ItemSetType:: iterator itend,
		      double s);
   void extractRules(const std::vector<ItemType>& body,
                  std::vector<ItemType>& head,
		  double thrR, double thrB, double suppB, int ngroups,sqlCoreConn* pcoreConn);
   void findHead(ItemSetType* head);
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

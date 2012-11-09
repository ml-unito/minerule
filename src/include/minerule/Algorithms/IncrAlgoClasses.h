


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
  void findChildInTree(ItemSetType* b1, ItemSetType* h1);
  void findRulesInTree(ItemSetType* itemset_body,ItemSetType* itemset_head);
  void findRulesInTree(ItemSetType* b1,ItemSetType* b1nb2,
                       ItemSetType* h1,ItemSetType* h1nh2);
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
   void findHead2bis(ItemSetType* h1nh2,ItemSetType* h1);
   void findHead2(ItemSetType* h1);
 };


 /*******************class NodeRow******/

 class NodeRow{

  private:
  Head* child;
  double support;
  //marca per algo distr
  bool marked;
  
  public:
  NodeRow() : child(NULL),support(0) {marked=0;}
  NodeRow(const NodeRow& nr) : support(nr.support) {}
  ~NodeRow(){delete child;}

  double getSupp(){return support;}
  void setSupp(double& s){support=s;/*cout<<"ho fissato il supp a "<<support<<"  ";*/}
  void incremSupp(){support++;/*cout<<"ora il supp e' "<<support<<"  ";*/}
  void decremSupp(){support--;/*cout<<"ora il supp e' "<<support<<"  ";*/}
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

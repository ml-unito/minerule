// ParsedMinerule.h.h
//#include"Ottimizzatore/ottimizzatore.h"
//#include<stdio>

#ifndef _ParsedMinerule_
#define _ParsedMinerule_

#include<string>
#include<vector>
#include"Utils/MinMaxPair.h"
#include"Utils/MineruleLogs.h"
#include"Utils/MineruleException.h"
#include"Utils/AlgorithmTypes.h"




typedef struct __tagsimple_pred simple_pred;
typedef struct __taglist_AND_node list_AND_node;
typedef struct __taglist_OR_node list_OR_node;

typedef struct __tagminerule mineruletag;

struct __tagsimple_pred
{
  char* val1;
  char* op;
  char*   val2;
};

struct __taglist_AND_node
{
  simple_pred*  sp;
  list_AND_node*  next;
};

struct __taglist_OR_node
{
  list_AND_node*  l_and;
  list_OR_node* next;
};

struct __tagminerule
{
  struct att_list*  ga;
  struct att_list*  ca;
  struct att_list*  ra;
  list_OR_node* mc;
  list_OR_node* gc;
  list_OR_node* cc;
};


list_OR_node* clone_l_OR(list_OR_node* l);
list_AND_node* clone_l_AND(list_AND_node* l);

class Attribute
{
  public:
   std::string  name;
    Attribute(const char* s);
    bool operator<(const Attribute& a) const;
};

namespace minerule {




class ParsedMinerule
{
 public:
  typedef std::vector<std::string> ListType;


 private:
  void fillAttrList(ListType& dest, struct att_list* source);

 std::string getAttrText(const ListType& l) const;
 std::string getSimplePredText(const simple_pred* pred) const;
 std::string getAndListText(const list_AND_node* cond)const;
 std::string getCondText(const list_OR_node* cond)const;
 std::string getCardsText(const MinMaxPair& mm) const;

 public:

  ListType  ga;  // group attr list 
  ListType  oa;  // ordering attr list (useful for mining sequences).
  ListType  ca;  // cluster attr list
  ListType  ra;  // rule attr list
  
  ListType  ba;  // body attr list
  ListType  ha;  // head attr list

  /* le seguenti liste dovrebbero essere sostituite
     da oggetti c++ (come quelle sopra per intenderci)*/
  list_OR_node* mc;  // mining condition
  list_OR_node* gc;  // group condition
  list_OR_node* cc;  // cluster condition
  
  ListType  c_aggr_list; // cluster aggregate list

  float     sup;
  float     conf;
  MinMaxPair bodyCardinalities;
  MinMaxPair headCardinalities;
 std::string    tab_source;
 std::string    tab_result;

  bool tautologies;
  bool body_coincident_head;

 /* Attributes used only by the sequence mining algorithm */
  MinMaxPair sequenceAllowedGaps;

  MiningTasks miningTask;

    
  ParsedMinerule() : 
    mc(NULL), 
    gc(NULL), 
    cc(NULL), 
    sup(0.0), 
    conf(0.0),
    bodyCardinalities( MinMaxPair(1,1000)),
    headCardinalities( MinMaxPair(1,1000)),
    sequenceAllowedGaps( MinMaxPair(0,0)) {
  };

  explicit ParsedMinerule(const std::string& minerule_text ):  
    mc(NULL), 
    gc(NULL), 
    cc(NULL), 
    sup(0.0), 
    conf(0.0),
    bodyCardinalities(1,1000),
    headCardinalities(1,1000),
	sequenceAllowedGaps( MinMaxPair(0,0)) {
    init(minerule_text);
  }
  
  ~ParsedMinerule() {
#ifdef MRUSERWARNING
#warning CAPIRE PERCHE'' DA SEGMENTATION FAULT NEL CASO LE SEGUENTI FREE SIANO DECOMMENTATE!
#endif
    //free_l_OR(mc);
    //free_l_OR(gc);
    //free_l_OR(cc);
  }
  
  ParsedMinerule( const ParsedMinerule& mr ) :
    ga(mr.ga),
    ca(mr.ca),
    ra(mr.ra),
    ba(mr.ba),
    ha(mr.ha),
    c_aggr_list(mr.c_aggr_list),
    sup(mr.sup),
    conf(mr.conf),
    bodyCardinalities( mr.bodyCardinalities ),
    headCardinalities( mr.headCardinalities ),
    tab_source(mr.tab_source),
    tab_result(mr.tab_result),
    tautologies(mr.tautologies),
    body_coincident_head(mr.body_coincident_head),
    sequenceAllowedGaps(mr.sequenceAllowedGaps),
 	miningTask(mr.miningTask) {
    mc=clone_l_OR(mr.mc);
    gc=clone_l_OR(mr.gc);
    cc=clone_l_OR(mr.cc);
  }
  
  void init(const std::string& minerule_text);

  

  /**
   * The method returns true if the constraints in the query
   * are item dependent. Notice that, cross predicates are
   * implicitly context dependent, even if the attributes depends
   * on the item.
   */
  bool hasIDConstraints() const throw(MineruleException);

  bool requiresClusters() const {
    return !ca.empty();
    //  return !(body_coincident_head && ca.empty());
  }

  bool hasCrossConditions(const list_OR_node* cond) const;

  bool hasCrossConditions() const {
    return hasCrossConditions(mc) || hasCrossConditions(gc) || hasCrossConditions(cc);
  }

  bool hasDisjuctionsInMC() const {
    return mc!=NULL && mc->next!=NULL;
  }
  
  bool hasSameBodyHead() const {
	  return ba == ha;
  }

  std::string getMinesequenceText() const;
  std::string getMineruleText() const;
  std::string getMineitemsetsText() const;
  std::string getText() const;
};

  std::ostream& operator<<(std::ostream& os, const ParsedMinerule& mr);
} // namespace minerule

#endif

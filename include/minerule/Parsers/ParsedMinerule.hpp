// ParsedMinerule.h.h
//#include"Ottimizzatore/ottimizzatore.h"
//#include<stdio>

#ifndef _ParsedMinerule_
#define _ParsedMinerule_

#include<string>
#include<vector>
#include"minerule/Utils/MinMaxPair.hpp"
#include"minerule/Utils/MineruleLogs.hpp"
#include"minerule/Utils/MineruleException.hpp"
#include"minerule/Utils/AlgorithmTypes.hpp"

#include <limits>


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

class Bem_cond{
public:
    std::string type;
    std::string attr;
    std::string op;
    std::string val;
    int count_min;
    int count_max;
    Bem_cond* and_c;

    Bem_cond(){
        type="";
        attr="";
        op="";
        val="";
        count_min=1;
        count_max=std::numeric_limits<int>::max();
        and_c=NULL;
    }

    Bem_cond(const minerule::Bem_cond& copy_me){
        type=copy_me.type;
        attr=copy_me.attr;
        op=copy_me.op;
        val=copy_me.val;
        count_min=copy_me.count_min;
        count_max=copy_me.count_max;
        if(copy_me.and_c!=NULL)
            and_c= new Bem_cond(*(copy_me.and_c));
        else
            and_c=NULL;
    }

    static std::vector<Bem_cond*> copyBemCond(std::vector<Bem_cond*> copy_me) {
        std::vector<Bem_cond*> out;
        for(int i=0; i<copy_me.size(); ++i)
            out.push_back(new Bem_cond(*copy_me[i]));
        return out;
    }
};


class Dist_cond {
public:
    std::string function;
    std::vector<std::string> attr;
    std::string range;

    Dist_cond(){
        function="";
        range="";
    }

    Dist_cond(const Dist_cond& copy_me){
        function=copy_me.function;
        range=copy_me.range;
        for(int i=0; i<copy_me.attr.size(); ++i) {
            attr.push_back(copy_me.attr[i]);
        }
    }

    static std::vector<Dist_cond*> copyDistCond(std::vector<Dist_cond*> copy_me) {
        std::vector<Dist_cond*> out;
        for(int i=0; i<copy_me.size(); ++i)
            out.push_back(new Dist_cond(*copy_me[i]));
        return out;
    }
};

class ParsedMinerule
{
 public:
  typedef std::vector<std::string> AttrVector;
  typedef Bem_cond* bem_c;

 private:
  void fillAttrList(AttrVector& dest, struct att_list* source);

 std::string getAttrText(const AttrVector& l) const;
 std::string getSimplePredText(const simple_pred* pred) const;
 std::string getAndListText(const list_AND_node* cond)const;
 std::string getCondText(const list_OR_node* cond)const;
 std::string getCardsText(const MinMaxPair& mm) const;

 public:

  AttrVector  ga;  // group attr list
  AttrVector  oa;  // ordering attr list (useful for mining sequences).
  AttrVector  ca;  // cluster attr list
  AttrVector  ra;  // rule attr list

  AttrVector  ba;  // body attr list
  AttrVector  ha;  // head attr list

  /* le seguenti liste dovrebbero essere sostituite
     da oggetti c++ (come quelle sopra per intenderci)*/
  list_OR_node* mc;  // mining condition
  list_OR_node* gc;  // group condition
  list_OR_node* cc;  // cluster condition

  AttrVector  c_aggr_list; // cluster aggregate list

  float     sup;
  float     conf;
  MinMaxPair bodyCardinalities;
  MinMaxPair headCardinalities;
 std::string    tab_source;
 std::string    tab_result;

  bool tautologies;
  bool body_coincident_head;
  bool distinct;

 /* Attributes used only by the sequence mining algorithm */
  MinMaxPair sequenceAllowedGaps;
  MinMaxPair length;
  std::vector<Dist_cond*> seq_dist_vect;
  std::vector<Bem_cond*> seq_bem_vect;
  std::string filter_condition;
  //std::string getFilterText(const list_OR_node* cond)const;

  MiningTasks miningTask;


  ParsedMinerule() :
    mc(NULL),
    gc(NULL),
    cc(NULL),
    sup(0.0),
    conf(0.0),
    bodyCardinalities( MinMaxPair(1,1000)),
    headCardinalities( MinMaxPair(1,1000)),
    sequenceAllowedGaps( MinMaxPair(0,0)),
    length(MinMaxPair(1,std::numeric_limits<int>::max())) {
  };

  explicit ParsedMinerule(const std::string& minerule_text ):
    mc(NULL),
    gc(NULL),
    cc(NULL),
    sup(0.0),
    conf(0.0),
    bodyCardinalities(1,1000),
    headCardinalities(1,1000),
    sequenceAllowedGaps( MinMaxPair(0,0)),
    length(MinMaxPair(1,std::numeric_limits<int>::max())) {
    init(minerule_text);
  }

  ~ParsedMinerule() {
#ifdef MRUSERWARNING
#warning CAPIRE PERCHE'' DA SEGMENTATION FAULT NEL CASO LE SEGUENTI FREE SIANO DECOMMENTATE!
	  // possible explanation: not all char* pointers in simple predicates have
	  // been dynamically allocated...
#endif
    //free_l_OR(mc);
    //free_l_OR(gc);
    //free_l_OR(cc);
  }

  ParsedMinerule( const ParsedMinerule& mr ) :
      oa(mr.oa),
      filter_condition(mr.filter_condition),
      distinct(mr.distinct),
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
    miningTask(mr.miningTask),
    length(mr.length){
    mc=clone_l_OR(mr.mc);
    gc=clone_l_OR(mr.gc);
    cc=clone_l_OR(mr.cc);
    seq_bem_vect= Bem_cond::copyBemCond(mr.seq_bem_vect);
    seq_dist_vect= Dist_cond::copyDistCond(mr.seq_dist_vect);
  }

  void init(const std::string& minerule_text);

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
  std::string getBEMText() const;

};

  std::ostream& operator<<(std::ostream& os, const ParsedMinerule& mr);
} // namespace minerule

#endif

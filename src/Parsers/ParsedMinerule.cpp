#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>

/*
  #ifndef String
  #  include<wxstrgnu.h>
  typedef wxString String;
 #endif
*/

#include "Utils/MineruleException.h"
#include "PredicateUtils/PredicateUtils.h"
#include <sstream>
#include "Utils/SQLUtils.h"
//#include"Ottimizzatore/ottimizzatore.h"


#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>
#include<iterator>

#include "Parsers/ParsedMinerule.h"
#include "Utils/MineruleOptions.h"
#include "Parsers/ParserLibrary.h"

/*#define yyerrror predicateerror

extern void yyerror(char*); */

using namespace std;

bool find_in_list(string s,minerule::ParsedMinerule::ListType l);

//int init_analisi_minerule(int ,char **);
int init_analisi_minerule(const string&);

void
print_AND_list(ostream& os, list_AND_node* andnode) {
  if( andnode==NULL )
    return;

  if( andnode->sp!=NULL ) {
    assert( andnode->sp->val1!=NULL &&
	    andnode->sp->op!=NULL &&
	    andnode->sp->val2 != NULL );

    cout << andnode->sp->val1;
    cout << " " << andnode->sp->op;
    cout << " " << andnode->sp->val2;
  }

  if( andnode->next!=NULL ) {
    os << " AND ";
    print_AND_list(os, andnode->next);
  }
}

void
print_OR_list(ostream& os, list_OR_node* ornode) {
  if( ornode==NULL ) 
    return;

  if( ornode->l_and!=NULL ) {
    os << "(";
    print_AND_list(os, ornode->l_and);
    os  << ")";
  }
  
  if( ornode->next!=NULL ) {
    os << " OR ";
    print_OR_list(os, ornode->next);
  }
}

extern void mrerror(char*);

list_AND_node* clone_l_AND(list_AND_node* l)
{
list_AND_node* result;
simple_pred*   p;

 if (l==NULL) return NULL;
 result=(list_AND_node*)malloc(sizeof(list_AND_node));

 if (result==NULL) 
   mrerror("Cannot allocate memory for a new list_AND_node");

 p=(simple_pred*)malloc(sizeof(simple_pred));

 if (p==NULL) 
   mrerror("simple_pred");

 p->val1=strdup(l->sp->val1);
 p->op=strdup(l->sp->op);
 p->val2=strdup(l->sp->val2);

 result->sp=p;
 result->next=clone_l_AND(l->next);

 return result;
}


list_OR_node* clone_l_OR(list_OR_node* l)
{
  list_OR_node* result;

  if (l==NULL) return NULL;
  
  result=(list_OR_node*)malloc(sizeof(list_OR_node));
  if (result==NULL) 
    mrerror("Cannot allocate memory for a new list_OR_node");
  
  result->l_and=clone_l_AND(l->l_and);
  result->next=clone_l_OR(l->next);
  return result;
}                                                        

namespace minerule {

  ostream& operator<<(ostream& os, const ParsedMinerule& mr) {
    os << "ParsedMinerule" << endl;
    os << " - ba:";
    copy( mr.ba.begin(), mr.ba.end(), ostream_iterator<string>(os, " "));
    os << endl;

    os << " - ha:";
    copy( mr.ha.begin(), mr.ha.end(), ostream_iterator<string>(os, " "));
    os << endl;

    os << " - ga:";
    copy( mr.ga.begin(), mr.ga.end(), ostream_iterator<string>(os, " "));
    os << endl 
       << " - ca:";
    copy( mr.ca.begin(), mr.ca.end(), ostream_iterator<string>(os, " "));
    os << endl 
       << " - ra:";
    copy( mr.ra.begin(), mr.ra.end(), ostream_iterator<string>(os, " "));
    os << endl
       << " = mc:";
    print_OR_list(os,mr.mc);
    os << endl 
       << " = gc:";
    print_OR_list(os,mr.gc);
    os << endl
       << " = cc:";
    print_OR_list(os,mr.cc);
    os << endl;
  
    os << " ! clust. agg. list:";
    copy( mr.c_aggr_list.begin(), 
	  mr.c_aggr_list.end(), 
	  ostream_iterator<string>(os, " "));
    os << endl;

    os << " * sup:" << mr.sup << endl;
    os << " * conf:" << mr.conf << endl;
    os << " & tautologies: "<<mr.tautologies << endl;
    os << " & body coinc head:"<<mr.body_coincident_head << endl;
    os << " tab_source:" << mr.tab_source << endl;
    os << " tab_result:" << mr.tab_result << endl;
    os << "ParsedMinerule - end" << endl;

    return os;
  }


  /*
  void
  clean_parser() {
    //  extern struct node *par_tree[4];
    //  extern void parser_free_all_memory();
    //  parser_free_all_memory();

    for( int i=0; i<4; i++ ) {
      par_tree[i]=NULL;
    }

    for(int i=0; i<6 ; i++ ) {
      par_sqlcode[i]="";
    }
  
    for(int i=0; i<10; i++) {
      deall_att_list(par_attlist[i]);
      par_attlist[i]=NULL;
    }
  }
  */

  float 
  floatFromText(char* text) {
    char* endptr;
    float tmp = strtof(text, &endptr);

    assert( endptr!=text );
    assert( errno!=ERANGE );

    return tmp;
  }

  string
  trim( string text ) {
    size_t begSpaces = text.find_first_not_of(" ");
    if( begSpaces == text.npos ) // all elements are spaces
      return "";

    text.erase( 0, begSpaces );

    size_t endSpaces = text.find_last_not_of(" ");
    if( endSpaces == text.npos || endSpaces+1 > text.length() )
      return text;

    text.erase( endSpaces+1, text.length()-endSpaces );
    return text;
  }

  /*
  void
  ParsedMinerule::fillAttrList(ListType& dest, struct att_list* source) {
    struct att_list* it=source;
    while( it!=NULL ) {
      dest.push_back(it->attrname);
      it=it->next;
    }
    } */

  void
  ParsedMinerule::init(const string& minerule_text)
  {
    pars_minerule( minerule_text, *this );
#if 0
    //  cout << "STO CREANDO LA MINERULE:" << endl << minerule_text << endl;
    MRLogPush("Parsing Algorithm...");
    MRDebugPush("Parsing Algorithm...");
    MRDebug() << "mr text:" << minerule_text << endl;


    //workaround di compatibilita'
    //    char* argv[2];
    //    argv[0]="unused";
    //    argv[1]="minerule.src";

    //    ofstream outmr(argv[1]);
    //    assert(outmr);
    //    outmr << minerule_text;
    //    outmr.close();

    MRLog() << "analyzing minerule text..." <<endl;
    //    int error=init_analisi_minerule(2,argv);
    int error=init_analisi_minerule(minerule_text);

    if (error!=0) {
      cerr << "Errore numero " << error << " nel parsing" << endl;
      stringstream ss;
      ss << "Some error occurred while parsing the Minerule, the error" << endl
	 << "code is:" << error << ". More informations can be found in" << endl
	 << "the parser Log file (look at the option file to enable it or" <<endl
	 << "to change its target." << ends;
      throw MineruleException(MR_ERROR_MINERULETEXT_PARSING, ss.str());
    }

    

    // Setting degli altri attributi

    if( par_sqlcode[PARFROMSQL]!=NULL )
      tab_source= trim(par_sqlcode[PARFROMSQL]);

    MRDebug() << "tab_source:" << tab_source << endl;

    if( partablename!=NULL )
      tab_result= trim(partablename);

    if( parsupport!=NULL )
      sup=floatFromText(parsupport);

    if( parconfidence!=NULL )
      conf=floatFromText(parconfidence);

    struct virtual_attr* curAttr = par_clustaggr;
    while( curAttr!=NULL ) {
      c_aggr_list.push_back(string(curAttr->realname)+":"+curAttr->virtualname);
      curAttr = curAttr->next;
    }

    fillAttrList( ba, par_attlist[PARBODYATT] );
    fillAttrList( ha, par_attlist[PARHEADATT] );
    fillAttrList( ga, par_attlist[PARGROUPATT]);
    fillAttrList( ca, par_attlist[PARCLUSTATT]);
    sort(ba.begin(), ba.end());
    sort(ha.begin(), ha.end());
    sort(ga.begin(), ga.end()); 
    sort(ca.begin(), ca.end());
    set_union(ba.begin(),ba.end(),ha.begin(),ha.end(),
	      insert_iterator<ListType>(ra, ra.begin()));


  
    tautologies = (par_tautologies=='T');
    body_coincident_head = (par_body_coincident_head == 'T');
    MinMaxPair p(0,0);
    p.setMin( Converter(parbody_mincard).toLong() );
    try {
      p.setMax( Converter(parbody_maxcard).toLong() );
    } catch (MineruleException&) {
      p.setDefaultMax();
    }

    bodyCardinalities = p;
  
    p.setMin( Converter(parhead_mincard).toLong() );
    try {
      p.setMax( Converter(parhead_maxcard).toLong() );
    } catch (MineruleException&) {
      p.setDefaultMax();
    }

    headCardinalities = p;
  
    MRLog() << "The name of the minerule is: " << tab_result << endl;
    MRLog() << "Analyzing minerule predicates..." << endl;
    //   cout << "MC:" << par_sqlcode[PARMINSQL] << endl;
    MRDebugPush("Parsing Mining Condition...");
    MRDebug() << "par_sqlcod[PARMINSQL]:"<< par_sqlcode[PARMINSQL]<<endl;

    mc=predicate_parser(par_sqlcode[PARMINSQL]);

    MRDebug() << "mc:" << mc << endl;
    MRDebugPop();
    //  cout << "GC:" << par_sqlcode[PARGROUPSQL] << endl;
    gc=predicate_parser(par_sqlcode[PARGROUPSQL]);
    //  cout << "CC:" << par_sqlcode[PARCLUSTSQL] << endl;
    cc=predicate_parser(par_sqlcode[PARCLUSTSQL]);

    // pulizia strutture parser
    clean_parser();

    MRDebugPop();
    MRLogPop();
    return;
#endif
  }

#if 0
  // quanto sotto faceva parte di init()... tempo addietro...
      if( !MineruleOptions::getSharedOptions().getOptimizations().getTryOptimizations())
      return;


      // S E Z I O N E   D I   O T T I M I Z Z A Z I O N E
      ListType::iterator li;
      ListType list_in;
      ListType list_out;
      string query;
      string key_id;
      for (li=ga.begin();li!=ga.end(); li++) {
      cout << "Normalizzazione attributo: " << *li << endl;
      list_out.clear();
      list_out.push_back(*li);
      cout << "fatta pulizia list_out e inserito'" << (*li) << endl;

      //mysql3 non sopporta select annidate
      /*
      query =  "SELECT col_name FROM mr_eq_keys_col WHERE key_id IN ";
      query += " (SELECT mr_eq__keys.ref_key_id FROM mr_eq_keys_col,mr_eq_keys ";
      query += "   WHERE mr_eq_keys.tab_name = '"+tab_source+"' AND ";
      query += "         mr_eq_keys.key_id=mr_eq_keys_col.key_id AND ";
      query += "         mr_eq_keys_col.col_name = '"+(*li)+"');";
    */
    statement=connection->createStatement();
    try
      {

	query =  "SELECT mr_eq_keys.ref_key_id FROM mr_eq_keys_col,mr_eq_keys ";
	query += "WHERE mr_eq_keys.tab_name = '"+tab_source+"' AND ";
	query += "      mr_eq_keys.key_id=mr_eq_keys_col.key_id AND ";
	query += "      mr_eq_keys_col.col_name = '"+(*li)+"';";
	cout << query << endl;
	rs=statement->executeQuery(query);
	
	try
	  {
	    rs->next(); key_id=rs->getString(1).c_str();
	    cout << "REF: " << key_id << endl;
	    
	    statement=connection->createStatement();
	    query =  "SELECT col_name FROM mr_eq_keys_col WHERE key_id='"+key_id+"';";
	    cout << query << endl;
	    rs=statement->executeQuery(query);
	    while(rs->next() ) {                        //&& trovato)
	      //trovato=false;
	      cout << " --> " << rs->getString(1).c_str() << endl;
	      //trovato=find_in_list(rs->getString(1),ga);
	    }
	  }
	catch (odbc::SQLException e) {
	  cerr << "MineRule Installation Error" <<endl;
	  cerr << "The Kernel module was not able to find an equivalent" << endl
	       << "key id for table '" <<tab_source<<"', column '" << (*li) << "'" <<endl;
	  cerr << "Among possible causes there are: " << endl;
	  cerr << "  1) an SQL server error" << endl
	       << "  2) the meta informations about the given table were not " << endl
	       << "     inserted properly into the MineRule system." << endl;
	  cerr << "The SQL query used to retrieve such information were:" << endl
	       << query << endl;
	  cerr << "The odbc driver returned the following error msg:"<<  endl;
	  cerr << e.getMessage() << endl;
	  cerr << endl;
	  cerr << "I'm continuing the execution anyway, be aware that you" <<endl
	       << "are likely to loose some optimizations" << endl;
	}
      }
    catch (odbc::SQLException& e) {
      cout << "What happened?" << e.getMessage() << endl;
      throw;
    }
    delete statement;
  } 
}
#endif
  
string ParsedMinerule::getAttrText(const ListType& l) const {
  string result;
  ListType::const_iterator it = l.begin();
  assert(it!=l.end());

  result = *it;
  it++;

  for(;it!=l.end();it++) {
    result += "," + *it;
  }

  return result;
}

string ParsedMinerule::getSimplePredText(const simple_pred* pred) const {
  assert(pred!=NULL);
  string result;
  if( pred->val1!=NULL)
    result+=pred->val1;

  if(pred->op!=NULL)
    result+=pred->op;

  if(pred->val2!=NULL)
    result+=pred->val2;

  return result;
}

string ParsedMinerule::getAndListText(const list_AND_node* cond)const {
  assert( cond != NULL);
  string result="";

  result = getSimplePredText(cond->sp);
  cond= cond->next;

  while(cond!=NULL) {
    result+=" AND "+getSimplePredText(cond->sp);
    cond=cond->next;
  }

  return result;
}

string ParsedMinerule::getCondText(const list_OR_node* cond)const {
  assert(cond!=NULL);
  string result ="";

  result = "("+getAndListText(cond->l_and)+")";
  cond = cond->next;

  while(cond!=NULL) {
    result += " OR (" + getAndListText(cond->l_and) + ")";
    cond=cond->next;
  }
    
  return result;
}

string ParsedMinerule::getCardsText(const MinMaxPair& mm) const {
  string result;
  result=Converter(mm.getMin()).toString()+"..";
  if( mm.getMax()!=mm.getDefaultMax() )
    result+=Converter(mm.getMax()).toString();
  else
    result+="n";
  return result;
}

string ParsedMinerule::getMinesequenceText() const {
  string result;
  result = 
    "MINE SEQUENCE " + tab_result + " AS " +
    "SELECT DISTINCT " + 
    getCardsText(bodyCardinalities) + " " + getAttrText(ba) +
    ", SUPPORT FROM "+tab_source+" GROUP BY "+getAttrText(ga)+
    " ORDER BY "+getAttrText(oa)+ " EXTRACTING SEQUENCES WITH SUPPORT:" +
    Converter(sup).toString();

  return result;
}

string ParsedMinerule::getText() const {
  switch( miningTask ) {
  case MTMineRules:
    return getMineruleText();
  case MTMineSequences:
    return getMinesequenceText();
  case MTMineItemsets:
    return getMineitemsetsText();
  default:
    throw MineruleException( MR_ERROR_MINERULETEXT_PARSING, 
			     "Current query is not currently supported by the system "
			     "(i.e., it is not a MINE RULE nor a MINE SEQUENCE query" );
  }
}

string ParsedMinerule::getMineruleText() const {
  string result;
  result = 
    "MINE RULE " + tab_result + " AS " +
    "SELECT DISTINCT " + 
    getCardsText(bodyCardinalities) + " " + getAttrText(ba) + " AS BODY," + 
    getCardsText(headCardinalities) + " " + getAttrText(ha) + " AS HEAD"
    ", SUPPORT, CONFIDENCE ";

  if( mc!=NULL ) 
    result += "WHERE "+getCondText(mc) + " ";
       
  result +=
    "FROM "+tab_source + " "
    "GROUP BY " + getAttrText(ga) + " ";

  if( gc!=NULL )
    result += getCondText(gc) + " ";

  if( !ca.empty() ) {
    result += "CLUSTER BY " + getAttrText(ca) + " ";

    if( cc!=NULL ) 
      result+= "HAVING " +getCondText(cc) + " ";
  }

  result += 
    "EXTRACTING RULES WITH SUPPORT:" + Converter(sup).toString() + ","+
    "CONFIDENCE:"+Converter(conf).toString();
    
  return result;
}

string ParsedMinerule::getMineitemsetsText() const {
  string result;
  result = 
    "MINE ITEMSET " + tab_result + " AS " +
    "SELECT DISTINCT " + 
    getCardsText(bodyCardinalities) + " " + getAttrText(ba) + " AS BODY" + 
    ", SUPPORT ";

  if( mc!=NULL ) 
    result += "WHERE "+getCondText(mc) + " ";
       
  result +=
    "FROM "+tab_source + " "
    "GROUP BY " + getAttrText(ga) + " ";

  if( gc!=NULL )
    result += getCondText(gc) + " ";

  result += 
    "EXTRACTING ITEMSETS WITH SUPPORT:" + Converter(sup).toString();
    
  return result;
}

bool ParsedMinerule::hasIDConstraints() const throw(MineruleException) {
  OptimizerCatalogue& optcat =
    MineruleOptions::getSharedOptions().getOptimizations().getCatalogue();
                                       

  list_OR_node* it_or=mc;
  for(;it_or!=NULL; it_or=it_or->next) {
    list_AND_node* it_and=it_or->l_and;
    for(;it_and!=NULL; it_and=it_and->next) {
      bool attr1 = SQLUtils::isAttribute(it_and->sp->val1);
      bool attr2 = SQLUtils::isAttribute(it_and->sp->val2);
      const ListType* attrList;
      string theAttr;

      if(attr1) {
	theAttr=it_and->sp->val1;
      }

      if(attr2) {
	theAttr=it_and->sp->val2;
      }

      if(attr1 && attr2)
	return false;


      if(theAttr.substr(0,5)=="BODY.") 
	attrList=&ba;
      else if(theAttr.substr(0,5)=="HEAD.")
	attrList=&ha;
      else throw MineruleException( MR_ERROR_MINERULETEXT_PARSING,
				    "Found a condition defined over an attribute,"
				    " but neither HEAD nor BODY selector has been"
				    " specified.");
      
      SQLUtils::removeHeadBodyFromAttrName(theAttr);

      if((attr1 || attr2) &&
	 !optcat.isIDAttribute(tab_source,*attrList,theAttr))
	return false;
    }
  }

  return true;
}
  


bool ParsedMinerule::hasCrossConditions(const list_OR_node* cond) const {
  MRLog()<<"Checking whether the minerule contains cross predicates" << endl;
  
  const list_OR_node* curr_OR;
  for(curr_OR=cond; curr_OR!=NULL; curr_OR=curr_OR->next) {
    list_AND_node* curr_AND;
    for(curr_AND=curr_OR->l_and; curr_AND!=NULL; curr_AND=curr_AND->next) {
      assert(curr_AND->sp!=NULL);
      MRLog() << curr_AND->sp->val1 << " " << curr_AND->sp->val2 << endl;
      bool hasHead = 
	strstr(curr_AND->sp->val1, "HEAD.")==curr_AND->sp->val1 ||
	strstr(curr_AND->sp->val2, "HEAD.")==curr_AND->sp->val2;
      bool hasBody = 
	strstr(curr_AND->sp->val1, "BODY.")==curr_AND->sp->val1 ||
	strstr(curr_AND->sp->val2, "BODY.")==curr_AND->sp->val2;
      if( hasHead && hasBody ) {
	MRLog() << "Cross predicate found!" << endl;
	return true;
      }
    }
  }

  MRLog() << "Cross predicate not found!" << endl;
  return false;
}
 
/*
  void ParsedMinerule::substituteInList(ListType& l,
  const string& attr,
  const SubstitutionInfo& info) const {
  // attr was == info.getAttribute() 
  ListType::const_iterator it;
  for(it=l.begin(); it!=l.end(); it++) {
  if( *it == attr ) {
  *it = substitute;
  }
  }
  }


  void ParsedMinerule::substituteInSimplePredicate(
  simple_pred* pred,
  const SubstitutionInfo& info) const {
  char** lhs=NULL;
  char** rhs=NULL;
  char** op = pred->op;

  if( pred->val1==attr ) {
  lhs = &pred->val1;
  rhs = &pred->val2;
  }

  if( pred->val2==attr ) {
  lhs = &pred->val2;
  rhs = &pred->val1;
  }

  if(lhs==NULL)
  return;
    
  string substitute = info.getSubstAttribute();
  *lhs = realloc( *lhs, sizeof( char[substitute.length()+1] ) );
  strcpy( *lhs, substitute.c_str() );

  string substVal;
  if( info.getSubstValue(*rhs,substVal) ) {
  *rhs = realloc( *rhs, sizeof( char[substVal.length()+1] ));
  strcpy( *rhs, substVal.c_str() );
  }

  string substOp;
  if( info.getSubstOp(*op, substOp) ) {
  *op = realloc( *op, sizeof( char[substOp.length()+1] ) );
  strcpy( *op, substOp.c_str() );
  }
  }

  void ParsedMinerule::substituteInPredicate(
  list_OR_node* pred,
  const SubstitutionInfo& info) const {
  while(pred!=NULL) {
  list_AND_node* currAnd = pred->l_and;
  while( currAnd!=NULL ) {
  substituteInSimplePredicate( currAnd->sp, attr, info );
  currAnd=currAnd->next;
  }

  pred=pred->next;
  }
  }
					


  void ParsedMinerule::substituteAttribute(const string& attr,
  const SubstitutionInfo& info) {
  substituteInList(ga,  info);
  substituteInList(ca,  info);
  substituteInList(ra,  info);
  substituteInList(ba,  info);
  substituteInList(ha,  info);

  substituteInPredicate(mc,  info);
  substituteInPredicate(gc,  info);
  substituteInPredicate(cc,  info);

  // what about aggreates?
  // substituteInAggregate(c_aggr_list);
  }
*/
} // namespace minerule

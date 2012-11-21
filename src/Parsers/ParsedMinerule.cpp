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



	bool find_in_list(std::string s,minerule::ParsedMinerule::AttrVector l);

//int init_analisi_minerule(int ,char **);
int init_analisi_minerule(const std::string&);

void
print_AND_list(std::ostream& os, list_AND_node* andnode) {
	if( andnode==NULL )
		return;

	if( andnode->sp!=NULL ) {
		assert( andnode->sp->val1!=NULL &&
			andnode->sp->op!=NULL &&
				andnode->sp->val2 != NULL );

		std::cout << andnode->sp->val1;
		std::cout << " " << andnode->sp->op;
		std::cout << " " << andnode->sp->val2;
	}

	if( andnode->next!=NULL ) {
		os << " AND ";
		print_AND_list(os, andnode->next);
	}
}

void
print_OR_list(std::ostream& os, list_OR_node* ornode) {
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

extern void mrerror(char const*);

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

	std::ostream& operator<<(std::ostream& os, const ParsedMinerule& mr) {
		os << "ParsedMinerule" << std::endl;
		os << " - ba:";
		copy( mr.ba.begin(), mr.ba.end(), std::ostream_iterator<std::string>(os, " "));
		os << std::endl;

		os << " - ha:";
		copy( mr.ha.begin(), mr.ha.end(), std::ostream_iterator<std::string>(os, " "));
		os << std::endl;

		os << " - ga:";
		copy( mr.ga.begin(), mr.ga.end(), std::ostream_iterator<std::string>(os, " "));
		os << std::endl 
			<< " - ca:";
		copy( mr.ca.begin(), mr.ca.end(), std::ostream_iterator<std::string>(os, " "));
		os << std::endl 
			<< " - ra:";
		copy( mr.ra.begin(), mr.ra.end(), std::ostream_iterator<std::string>(os, " "));
		os << std::endl
			<< " = mc:";
		print_OR_list(os,mr.mc);
		os << std::endl 
			<< " = gc:";
		print_OR_list(os,mr.gc);
		os << std::endl
			<< " = cc:";
		print_OR_list(os,mr.cc);
		os << std::endl;
  
		os << " ! clust. agg. list:";
		copy( mr.c_aggr_list.begin(), 
			mr.c_aggr_list.end(), 
				std::ostream_iterator<std::string>(os, " "));
		os << std::endl;

		os << " * sup:" << mr.sup << std::endl;
		os << " * conf:" << mr.conf << std::endl;
		os << " & tautologies: "<<mr.tautologies << std::endl;
		os << " & body coinc head:"<<mr.body_coincident_head << std::endl;
		os << " tab_source:" << mr.tab_source << std::endl;
		os << " tab_result:" << mr.tab_result << std::endl;
		os << "ParsedMinerule - end" << std::endl;

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

	std::string
	trim(std::string text ) {
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
	ParsedMinerule::fillAttrList(AttrVector& dest, struct att_list* source) {
		struct att_list* it=source;
		while( it!=NULL ) {
			dest.push_back(it->attrname);
			it=it->next;
		}
		} */

			void
		ParsedMinerule::init(const std::string& minerule_text) {
			parseMinerule( minerule_text, *this );
		}


		std::string ParsedMinerule::getAttrText(const AttrVector& l) const {
			std::string result;
			AttrVector::const_iterator it = l.begin();
			assert(it!=l.end());

			result = *it;
			it++;

			for(;it!=l.end();it++) {
				result += "," + *it;
			}

			return result;
		}
		std::string ParsedMinerule::getSimplePredText(const simple_pred* pred) const {
			assert(pred!=NULL);
			std::string result;
			if( pred->val1!=NULL)
				result+=pred->val1;

			if(pred->op!=NULL)
				result+=pred->op;

			if(pred->val2!=NULL)
				result+=pred->val2;

			return result;
		}
		std::string ParsedMinerule::getAndListText(const list_AND_node* cond)const {
			assert( cond != NULL);
			std::string result="";

			result = getSimplePredText(cond->sp);
			cond= cond->next;

			while(cond!=NULL) {
				result+=" AND "+getSimplePredText(cond->sp);
				cond=cond->next;
			}

			return result;
		}
		std::string ParsedMinerule::getCondText(const list_OR_node* cond)const {
			assert(cond!=NULL);
			std::string result ="";

			result = "("+getAndListText(cond->l_and)+")";
			cond = cond->next;

			while(cond!=NULL) {
				result += " OR (" + getAndListText(cond->l_and) + ")";
				cond=cond->next;
			}
    
			return result;
		}
		std::string ParsedMinerule::getCardsText(const MinMaxPair& mm) const {
			std::string result;
			result=Converter(mm.getMin()).toString()+"..";
			if( mm.getMax()!=mm.getDefaultMax() )
				result+=Converter(mm.getMax()).toString();
			else
				result+="n";
			return result;
		}
		std::string ParsedMinerule::getMinesequenceText() const {
			std::string result;
			result = 
				"MINE SEQUENCE " + tab_result + " AS " +
					"SELECT DISTINCT " + 
						getCardsText(bodyCardinalities) + " " + getAttrText(ba) +
							", SUPPORT FROM "+tab_source+" GROUP BY "+getAttrText(ga)+
								" ORDER BY "+getAttrText(oa)+ " EXTRACTING SEQUENCES WITH SUPPORT:" +
									Converter(sup).toString();

			return result;
		}
		std::string ParsedMinerule::getText() const {
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
		std::string ParsedMinerule::getMineruleText() const {
			std::string result;
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
		std::string ParsedMinerule::getMineitemsetsText() const {
			std::string result;
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
					const AttrVector* attrList;
					std::string theAttr;

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

					if((attr1 || attr2) && !optcat.isIDAttribute(tab_source,*attrList,theAttr))
							return false;
				}
			}

			return true;
		}
  


		bool ParsedMinerule::hasCrossConditions(const list_OR_node* cond) const {
			MRLog()<<"Checking whether the minerule contains cross predicates" << std::endl;
  
			const list_OR_node* curr_OR;
			for(curr_OR=cond; curr_OR!=NULL; curr_OR=curr_OR->next) {
				list_AND_node* curr_AND;
				for(curr_AND=curr_OR->l_and; curr_AND!=NULL; curr_AND=curr_AND->next) {
					assert(curr_AND->sp!=NULL);
					MRLog() << curr_AND->sp->val1 << " " << curr_AND->sp->val2 << std::endl;
					bool hasHead = 
						strstr(curr_AND->sp->val1, "HEAD.")==curr_AND->sp->val1 ||
							strstr(curr_AND->sp->val2, "HEAD.")==curr_AND->sp->val2;
					bool hasBody = 
						strstr(curr_AND->sp->val1, "BODY.")==curr_AND->sp->val1 ||
							strstr(curr_AND->sp->val2, "BODY.")==curr_AND->sp->val2;
					if( hasHead && hasBody ) {
						MRLog() << "Cross predicate found!" << std::endl;
						return true;
					}
				}
			}

			MRLog() << "Cross predicate not found!" << std::endl;
			return false;
		}
 
	} // namespace minerule

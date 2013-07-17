//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "minerule/Parsers/ParsedPredicate.h"


namespace minerule {

  PredicateBase* ParsedSimplePredicate::newPredicate() const {
    return new ParsedPredicate();
  }

  PredConjunctionBase* ParsedSimplePredicate::newPredConjunction() const {
    return new ParsedPredConjunction();
  }

  PredicateBase* ParsedPredConjunction::newPredicate() const {
    return new ParsedPredicate();
  }

  PredConjunctionBase* ParsedPredConjunction::newPredConjunction() const {
    return new ParsedPredConjunction();
  }

  PredicateBase* ParsedPredicate::newPredicate() const {
    return new ParsedPredicate();
  }

  PredConjunctionBase* ParsedPredicate::newPredConjunction() const {
    return new ParsedPredConjunction();
  }
  





  list_AND_node* ParsedPredicate::convert_and_list(const PredConjunctionBase& conj) const {
    PredConjunctionBase::const_iterator it_and;
    list_AND_node* list_and = NULL;
    list_AND_node* and_head = NULL;

    for( it_and = conj.begin(); it_and!=conj.end(); ++it_and ) {
      if(list_and==NULL) {
	list_and = (list_AND_node*)malloc( sizeof(list_AND_node) );
	list_and->next = NULL;
	and_head = list_and;
      } else {
	list_and->next = (list_AND_node*) malloc( sizeof( list_AND_node ) );
	list_and = list_and->next;
	list_and->next = NULL;
      }
      
      list_and->sp = (simple_pred*) malloc(sizeof(simple_pred));
      list_and->sp->val1=strdup((*it_and)->getVal1().c_str());
      list_and->sp->op=strdup((*it_and)->getOp().c_str());
      list_and->sp->val2=strdup((*it_and)->getVal2().c_str());
    }

    return and_head;
  }


  list_OR_node* ParsedPredicate::convert() const { 
    PredicateBase::const_iterator it_or;
    list_OR_node* list_or = NULL;
    list_OR_node* or_head = NULL;

    for( it_or = this->begin(); it_or!=this->end(); ++it_or ) {
      if(list_or==NULL) {
	list_or = (list_OR_node*) malloc( sizeof(list_OR_node) );
	list_or->next=NULL;
	or_head=list_or;
      } else {
	list_or->next = (list_OR_node*) malloc( sizeof(list_OR_node) );
	list_or = list_or->next;
	list_or->next=NULL;
      }

      list_or->l_and = convert_and_list(**it_or);
    }

    return or_head;
  }
  


} // namespace

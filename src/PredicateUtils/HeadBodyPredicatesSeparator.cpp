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
#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.hpp"
#include "minerule/Utils/SQLUtils.hpp"

namespace minerule {

	void HeadBodyPredicatesSeparator::setState( ConstraintClass& current, ConstraintClass newone )  {
		if( current==NO_INFO || current==newone ) {
			current=newone;
			return;
		}

		current=ERROR;
		throw MineruleException(MR_ERROR_INTERNAL,
			"The optimizer found a constraint defined  on both BODY and HEAD!");

	} 

	HeadBodyPredicatesSeparator::ConstraintClass
	HeadBodyPredicatesSeparator::setStateAccordinglyToString(std::string& str, ConstraintClass state)  {
		std::string substr = str.substr(0,5);
		if(substr=="BODY.")
			setState(state, BODY_CONSTR);
		else if(substr=="HEAD.")
			setState(state, HEAD_CONSTR);
		else if(SQLUtils::isAttribute(str))
			throw MineruleException(MR_ERROR_INTERNAL,
				"The optimizer found a constraint defined neither on BODY nor on HEAD!");
		else return state;
  
  // we need to remove the HEAD. and BODY. part of the string
		SQLUtils::removeHeadBodyFromAttrName( str );
		return state;
	}

	void
	HeadBodyPredicatesSeparator::updateConstraint( std::string& str, const std::string& v1, const std::string& op, const std::string& v2) {
		if(str!="")
			str+=" AND ";

		str+=v1+" "+op+" "+v2;
	}

	void
		HeadBodyPredicatesSeparator::separate(list_AND_node* l_and, 
			std::string& bodyConstraints,
	std::string& headConstraints) {
		if( l_and == NULL ) {
			bodyConstraints = "1=1";
			headConstraints = "1=1";
			return;
		}

		while(l_and!=NULL) {
			ConstraintClass state = NO_INFO;
			std::string v1 = l_and->sp->val1;
			std::string v2 = l_and->sp->val2;
    
			state = setStateAccordinglyToString(v1, state);
			state = setStateAccordinglyToString(v2, state);
   
			switch(state) {
				case BODY_CONSTR:
					updateConstraint(bodyConstraints, v1, l_and->sp->op, v2);
					break;
				case HEAD_CONSTR:
					updateConstraint(headConstraints, v1, l_and->sp->op, v2);
					break;
				default:
					throw MineruleException(MR_ERROR_INTERNAL,
						"The optimizer found a constraint defined neither on BODY nor on HEAD!");
			};

			l_and=l_and->next;
		}
	}

} // namespace

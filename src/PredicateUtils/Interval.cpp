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
#include "minerule/PredicateUtils/Interval.h"
#include "minerule/Utils/MineruleOptions.h"
#include "minerule/PredicateUtils/Predicate.h"

namespace minerule {
  
  /* ======================================================================
   * Interval
   * ======================================================================
   */

  const char* Interval::POSINFTY="+";
  const char* Interval::NEGINFTY="-";

  std::ostream& operator<<(std::ostream& os, const Interval& i) {
    if( i.lwrOpen || i.openPoints.find(i.lwr)!=i.openPoints.end()) os << "("; else os << "[";
    os << i.lwr << "," << i.upp;
    if( i.uppOpen || i.openPoints.find(i.upp)!=i.openPoints.end()) os << ")"; else os << "]";

    return os;
  }
  

  Interval::Interval( Operator op, const char* val, SQLUtils::Type t )  {
    upp=POSINFTY;
    lwr=NEGINFTY;
    uppOpen=lwrOpen=true;
    setType(t);

    switch(op) {
    case Less:
      upp=val;
      break;
    case LessEq:
      upp=val;
      uppOpen=false;
      break;
    case Grt:
      lwr=val;
      break;
    case GrtEq:
      lwr=val;
      lwrOpen=false;
      break;
    case Eq:
      lwr=val;
      upp=val;
      lwrOpen=false;
      uppOpen=false;
      break;
    case NotEq:
      openPoints.insert(val);
      break;
    }
  }

  Interval::Operator Interval::getOperator( const char* op, bool negateIt ) const throw(MineruleException) {
    static const char* errMsg = "Do not know how to handle operator: ";
    bool twoCharOp = op[1]!='\0';
    if(op[0]=='<') {
      if(!twoCharOp)
	return negateIt?GrtEq:Less;
      else if(op[1]=='>')
	return negateIt?Eq:NotEq;
      else if(op[1]=='=')
	return negateIt?Grt:LessEq;
      else throw MineruleException( MR_ERROR_OPTIMIZER_ERROR,
				    errMsg+ std::string(op) );
    } else if( op[0]=='>' ) {
      if(!twoCharOp)
	return negateIt ? LessEq : Grt;
      else if(op[1]=='=')
	return negateIt ? Less : GrtEq;
      else throw MineruleException(  MR_ERROR_OPTIMIZER_ERROR,
				     errMsg+ std::string(op) );

    }	else if( op[0]=='=' ) {
      if(twoCharOp)
	throw  MineruleException(  MR_ERROR_OPTIMIZER_ERROR,
				   errMsg+ std::string(op) );
      else 
	return negateIt ? NotEq : Eq;
    } else throw MineruleException( MR_ERROR_OPTIMIZER_ERROR,
				    errMsg+ std::string(op) );
  }

  int Interval::compareValues( const char* val1, const char* val2) const throw(MineruleException) {
    assert(typeOk);

    if(val1==NEGINFTY) {
      if(val2==NEGINFTY)
	return 0;
      else
	return -1;
    }
    if(val1==POSINFTY) {
      if(val2==POSINFTY)
	return 0;
      else
	return 1;
    }
    if(val2==NEGINFTY)
      return 1;
    if(val2==POSINFTY)
      return -1;

    if( type==SQLUtils::String) {
      return strcmp(val1,val2);
    } else if( type==SQLUtils::Numeric ) {
      return int(Converter(val1).toDouble()-Converter(val2).toDouble());
    } else {
      throw MineruleException(MR_ERROR_INTERNAL,
			      "SimplePredAnalyzer still does not support SQL types"
			      " which differ fromstd::string and numeric");
    }
  }

  const char* Interval::getMaxLwr( Interval& lhs, Interval& rhs, bool& maxIsOpen ) const throw(MineruleException) {
    int order = compareValues(lhs.lwr, rhs.lwr);
      
    if( order<0 ) {
      maxIsOpen = rhs.lwrOpen;
      return rhs.lwr;
    } else if(order>0) {
      maxIsOpen = lhs.lwrOpen;
      return lhs.lwr;
    } else {
      maxIsOpen = lhs.lwrOpen || rhs.lwrOpen;
      return lhs.lwr;
    }
  }

  const char* Interval::getMinUpp( Interval& lhs, Interval& rhs, bool& minIsOpen ) const throw(MineruleException) {
    int order = compareValues(lhs.upp, rhs.upp);

    if( order<0 ) {
      minIsOpen = lhs.uppOpen;
      return lhs.upp;
    } else if( order>0 ){
      minIsOpen = rhs.uppOpen;
      return rhs.upp;
    } else {
      minIsOpen = lhs.uppOpen || rhs.uppOpen;
      return lhs.upp;
    }
  }

    
  const char* Interval::getValue( const char* val1, const char* val2 ) {
    if( !SQLUtils::isAttribute(val1) )
      return val1;

    if( !SQLUtils::isAttribute(val2) )
      return val2;

    return NULL;
  }

  const char* Interval::getAttribute(const char* val1, const char* val2 ) {
    if( SQLUtils::isAttribute(val1) )
      return val1;

    if( SQLUtils::isAttribute(val2) )
      return val2;

    return NULL;
  }

  

  void Interval::update(const list_AND_node* l, bool negateIt) throw(MineruleException) {
    assert(l!=NULL && l->sp!=NULL && 
	   l->sp->val1!=NULL && l->sp->op!=NULL && l->sp->val2!=NULL);

    const char* value = getValue(l->sp->val1, l->sp->val2);
    if(value==NULL) // this is a cross condition, we just skip the update
      return;

    Interval i( getOperator( l->sp->op, negateIt ), value, type );
    intersect(i);
  }

  /* ======================================================================
   * IntervalChecker
   * ======================================================================
   */
  
  SQLUtils::Type IntervalChecker::typeForAttribute(const char* attr) throw(MineruleException, odbc::SQLException) {
    std::map<const char*, SQLUtils::Type>::const_iterator it = 
      typesMap.find(attr);
    
    SQLUtils::Type t;
    if(it!=typesMap.end()) {
      t = it->second;
    } else {
      t = SQLUtils::getType( MineruleOptions::getSharedOptions().getODBC().getODBCConnection(),
			     tab_source,
			     attr );
      
      typesMap[attr] = t;
    }
    
    return t;
  }

  

  bool IntervalChecker::impossibleVariableSetting( const VarSet& vset, const list_AND_node* l) {
    std::map<const char*, Interval, StringCompare> intervals;
    //    std::cout << std::endl;

    while( l!=NULL ) {
      const char* attr = Interval::getAttribute( l->sp->val1, l->sp->val2 );
      if(attr!=NULL) {
	SimplePredicate& pred = SimplePredicate::newSimplePredicate(l->sp);
	Interval& i=intervals[attr];
	i.setType(typeForAttribute(attr));
	i.update(l, !vset.getVar(pred.getVarId()));
	
	//	std::cout << l->sp->val1 << l->sp->op << l->sp->val2 << " ";
	//	std::cout << "id:" << pred.getVarId() << " v(id):" << vset.getVar(pred.getVarId()) << " ";
	//	std::cout << i << std::endl;

	if(i.isEmpty())
	  return true;
      };
      
      l=l->next;
    }
    
    //    std::cout << intervals["price"] << " ";

    return false;
  }
  
  
} // namespace 

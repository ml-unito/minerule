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
#ifndef __QUERY_RESULT_IMPL_H__
#define __QUERY_RESULT_IMPL_H__

#include <algorithm>
#include "minerule/Result/QueryResult.h"
#include "minerule/Database/Connection.h"
#include "minerule/Optimizer/OptimizerCatalogue.h"

namespace minerule {

  template <class Sorter>
	void 
	QueryResult::ResultSet<Sorter>::load(const std::string& qryName, double sup, double con) throw(MineruleException) {
		QueryResult::Iterator qit;
		OptimizerCatalogue::getMRQueryResultIterator(qryName,qit,sup,con);

		while( qit.next() ) {
			Rule r;
			qit.getRule(r);

			std::set<Rule,Sorter>::insert(r);
		}
	}


 // 
 // Complexity note: let this->size()=n, rhs.size()=m, and let o denote
 // the size of the intersection. 
 // This implementation requires:
 //  m+n steps to iterate thru the two lists
 //  o*log(o) steps to inser the results into the tmp variable
 //  o*log(o) steps to copy the result into *this
 // 
 // The implementation that deletes the nodes which are not found in
 // *this, would require: 
 // m steps to iterate thru rhs 
 // m*log(n) steps to search the objects in this 
 // (n-o)log(n) (approximated) steps to delete the object that do *
 // not belong to the intersection from *this.
 // 
 // The analysis about when one is better than the other is a little
 // bit complicated, however, it roughly speaking, it seems
 // reasonable to say that if o is very small then this
 // implementation should be a good one, since it is ~= m+n while the
 // second one is ~= m+mlog(n)+nlog(n). On the other side, if o~=n
 // then this implementation is m+n+2nlog(n), while the other one is
 // m+mlog(n)+nlog(n).
 // 

  template <class Sorter>
	QueryResult::ResultSet<Sorter>& QueryResult::ResultSet<Sorter>::inplace_intersect(const ResultSet<Sorter>& rhs) {
		QueryResult::ResultSet<Sorter> tmp;
		set_intersection( this->begin(), this->end(), rhs.begin(),rhs.end(),
			std::insert_iterator<ResultSet<Sorter> >( tmp, tmp.begin() ), Sorter());

		*this = tmp;
		return *this;
	}

  template <class Sorter> 
	QueryResult::ResultSet<Sorter>&	QueryResult::ResultSet<Sorter>::inplace_union(const ResultSet<Sorter>& rhs) {

		copy( rhs.begin(), rhs.end(), 
			std::insert_iterator<ResultSet<Sorter> >(*this, this->begin()));
		return *this;
	}

  template <class Sorter>
	QueryResult::ResultSet<Sorter>& QueryResult::ResultSet<Sorter>::operator&=(const ResultSet<Sorter>& rhs) {
		return inplace_intersect(rhs);
	}
   template <class Sorter>
	QueryResult::ResultSet<Sorter>& QueryResult::ResultSet<Sorter>::operator|=(const ResultSet<Sorter>& rhs) {
		return inplace_union(rhs);
	}
  

  template <class Sorter>
	void QueryResult::ResultSet<Sorter>::save(const ParsedMinerule& mr) const {
		Connection connection;
		connection.setOutTableName(mr.tab_result);
		connection.setBodyCardinalities(mr.bodyCardinalities);
		connection.setHeadCardinalities(mr.headCardinalities);
		connection.useODBCConnection( MineruleOptions::getSharedOptions().getODBC().getODBCConnection());
		
		connection.createResultTables(SourceRowMetaInfo(connection.getODBCConnection(), mr));

		typename ResultSet<Sorter>::const_iterator it;
		for(it=this->begin();it!=this->end();it++) {
			connection.insert( it->getBody(), it->getHead(), it->getSupport(), it->getConfidence() );
		}
	}
}

#endif

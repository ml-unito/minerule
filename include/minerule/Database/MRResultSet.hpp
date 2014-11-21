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
#ifndef __MRRESULTSETITERATOR_H__
#define __MRRESULTSETITERATOR_H__

#include "minerule/mrdb/PreparedStatement.hpp"
#include "minerule/mrdb/ResultSet.hpp"
// #include "MIndex.hpp"

class MRResultSetIterator {
	mrdb::PreparedStatement * query;
	mrdb::ResultSet* rs;
	// MIndexIterator*     mi;

	void setParameters(const std::string& values) {
		int i = 1;
		char * s = new char[values.length()+1];
		strcpy(s, values.c_str());
		char * p = strtok(s,",");
		while (p != NULL) {
			std::string val(p);
			query->setString(i,val);
			i++;
			p = strtok(NULL,",");
		}
		delete s;
	}
  
public:
	MRResultSetIterator(mrdb::PreparedStatement * q/*, MIndexIterator* miningIndex = NULL*/) : query(q), rs(NULL)/*, mi(miningIndex)*/ {
		/*if (mi == NULL)*/ rs = query->executeQuery();
	}

	bool next() {
		bool ok = false;
		// if( mi==NULL )
			return rs->next();
		// else if (rs == NULL || !(ok = rs->next())) {
		// 	while (mi->current() != mi->end() && !ok) {
		// 		if (rs != NULL) delete rs;
		// 		setParameters(*(mi->current()));
		// 		rs = query->executeQuery();
		// 		(*mi)++;
		// 		if( (ok = rs->next()) ) break;
		// 	} 
		// }
		return ok;
	}

	void reset() {
		if( rs!=NULL ) { delete rs; rs = NULL; }
		// if( mi==NULL ) 
			rs=query->executeQuery();
		// else
		// 	mi->reset();
	}

	mrdb::ResultSet* getResultSet() {
		return rs;
	}
};

#endif

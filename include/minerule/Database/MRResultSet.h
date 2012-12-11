#ifndef __MRRESULTSETITERATOR_H__
#define __MRRESULTSETITERATOR_H__

#include <odbc++/preparedstatement.h>
#include <odbc++/resultset.h>
#include "MIndex.h"

class MRResultSetIterator {
	odbc::PreparedStatement * query;
	odbc::ResultSet* rs;
	MIndexIterator*     mi;

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
	MRResultSetIterator(odbc::PreparedStatement * q, MIndexIterator* miningIndex = NULL) : query(q), rs(NULL), mi(miningIndex) {
		if (mi == NULL) rs = query->executeQuery();
	}

	bool next() {
		bool ok = false;
		if( mi==NULL )
			return rs->next();
		else if (rs == NULL || !(ok = rs->next())) {
			while (mi->current() != mi->end() && !ok) {
				if (rs != NULL) delete rs;
				setParameters(*(mi->current()));
				rs = query->executeQuery();
				(*mi)++;
				if( (ok = rs->next()) ) break;
			} 
		}
		return ok;
	}

	void reset() {
		if( rs!=NULL ) { delete rs; rs = NULL; }
		if( mi==NULL ) 
			rs=query->executeQuery();
		else
			mi->reset();
	}

	odbc::ResultSet* getResultSet() {
		return rs;
	}
};

#endif

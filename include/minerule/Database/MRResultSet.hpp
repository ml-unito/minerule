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
  mrdb::PreparedStatement *query;
  mrdb::ResultSet *rs;

public:
  MRResultSetIterator(mrdb::PreparedStatement *q) : query(q), rs(NULL) {
    rs = query->executeQuery();
  }

  bool next() { return rs->next(); }

  void reset() {
    if (rs != NULL) {
      delete rs;
    }

    rs = query->executeQuery();
  }

  mrdb::ResultSet *getResultSet() { return rs; }
};

#endif

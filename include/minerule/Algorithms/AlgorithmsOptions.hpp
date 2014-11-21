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
#ifndef __ALGORITHMS_OPTIONS_H__
#define __ALGORITHMS_OPTIONS_H__

#include <string.h>
#include "minerule/mrdb/PreparedStatement.hpp"
#include "minerule/mrdb/Connection.hpp"

#include "minerule/Database/SourceRow.hpp"
#include "minerule/Parsers/ParsedMinerule.hpp"
#include "minerule/Database/MRResultSet.hpp"
#include "minerule/Utils/MineruleOptions.hpp"

namespace minerule {

class AlgorithmsOptions {
 private:
  size_t totGroups; // total number of groups in the unfiltered table
  double support;
  double confidence;
  unsigned int rowsPerPartition;
  MinMaxPair headCardinalities;
  MinMaxPair bodyCardinalities;
  mrdb::Connection* connection;
  mrdb::PreparedStatement* statement;

  // the following will substitute the prepared statement 
  // when the MRResultSetIterator will be integrated in the proj.
  MRResultSetIterator* mriterator;

  SourceRowColumnIds sourceRowDescription;
 std::string outTableName;

  MineruleOptions::MiningAlgorithms miningAlgorithmsOptions;
 public:

  // Note that headCardinalities and maxBodyCardinalities=1000
  // is far beyond the manageable sizes (i.e. it is the same
  // as setting them to infinity).
  AlgorithmsOptions() :
    headCardinalities(MinMaxPair(1,1000)), bodyCardinalities(MinMaxPair(1,1000)) {};


  void setSupport(double sup);
  void setConfidence(double conf);
  void setHeadCardinalities(const MinMaxPair& n) {
    headCardinalities = n;
  }
  void setBodyCardinalities(const MinMaxPair& n) {
    bodyCardinalities = n;
  }
  void setConnection(mrdb::Connection*);
  void setStatement(mrdb::PreparedStatement*);
  void setSourceRowDescription(const SourceRowColumnIds& srdes);
  void setOutTableName(const std::string& fname) {
    outTableName=fname;
  }
  
  void setMiningAlgorithmsOptions(const MineruleOptions::MiningAlgorithms& opt) {
    miningAlgorithmsOptions = opt;
  }

  double getSupport() const;
  double getConfidence() const;
  const std::string& getOutTableName() const {
    return outTableName;
  }

  const MineruleOptions::MiningAlgorithms& getMiningAlgorithmsOptions() const {
    return miningAlgorithmsOptions;
  }

  const MinMaxPair& getHeadCardinalities() const {
    return headCardinalities;
  }

  const MinMaxPair& getBodyCardinalities() const {
    return bodyCardinalities;
  }

  MinMaxPair& getBodyCardinalities() {
    return bodyCardinalities;
  }

  MinMaxPair& getHeadCardinalities() {
    return headCardinalities;
  }

  void setMRIterator( MRResultSetIterator& mrit ) {
    mriterator = &mrit;
  }

  MRResultSetIterator&
    getMRIterator() const {
    return *mriterator;
  }

  size_t getTotGroups() const {
    return totGroups;
  }

  void setTotGroups(size_t groups) {
    totGroups=groups;
  }
  
  mrdb::Connection* getODBCConnection() const;
  mrdb::PreparedStatement* getStatement() const;
  const SourceRowColumnIds& 
    getSourceRowDescription() const;
};


} // end minerule namespace

#endif

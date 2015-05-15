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

  // the following will substitute the prepared statement
  // when the MRResultSetIterator will be integrated in the proj.
  MRResultSetIterator *mriterator;

  SourceRowColumnIds sourceRowDescription;
  std::string outTableName;

  MineruleOptions::MiningAlgorithms miningAlgorithmsOptions;

public:
  // Note that headCardinalities and maxBodyCardinalities=1000
  // is far beyond the manageable sizes (i.e. it is the same
  // as setting them to infinity).
  AlgorithmsOptions()
      : headCardinalities(MinMaxPair(1, 1000)),
        bodyCardinalities(MinMaxPair(1, 1000)){};

  /// Sets the support threshold
  /// @param sup the support threshold
  void setSupport(double sup);

  /// Sets the confidence threshold
  /// @param conf the confidence threshold
  void setConfidence(double conf);

  /// Sets head cardinalities: i.e., the minimum/maximum number of
  /// elements that can be present in the head a mined rule.
  /// @param n a min/max pair
  void setHeadCardinalities(const MinMaxPair &n) { headCardinalities = n; }

  /// Sets the body cardinalities: i.e., the minimum/maximum number of
  /// elements that can be present in the body a mined rule.
  /// @param n a min/max pair
  void setBodyCardinalities(const MinMaxPair &n) { bodyCardinalities = n; }

  /// Sets the mrdb connection to be used
  /// @param connection the connection to be set. The class will store a weak
  ///   reference to the connection. Deallocation remains therefore under
  ///   the caller responsibility
  void setConnection(mrdb::Connection *connection);

  /// Sets the prepared statement needed to build the result set
  /// that has to be mined.
  /// @param statement the statement to be set. The class will store a weak
  ///   reference to the statement. Deallocation remains therefore under
  ///   the caller responsibility
  void setStatement(mrdb::PreparedStatement *statement);

  /// Sets the source row description needed by algorithms to distinguish
  /// body/head/group attributes in the mining set.
  /// @param srdes a source row description
  void setSourceRowDescription(const SourceRowColumnIds &srdes);

  /// Sets the name of the output table. The output table will be created
  /// by the mining algorithm and should not exists on the database. If
  /// it exists an error will be generated.
  /// @param fname the name of the output table
  void setOutTableName(const std::string &fname) { outTableName = fname; }

  /// Sets the options pertaining to the specific mining algorithm being
  /// executed
  /// (e.g., number of partitions for the Partition algorithm.)
  void
  setMiningAlgorithmsOptions(const MineruleOptions::MiningAlgorithms &opt) {
    miningAlgorithmsOptions = opt;
  }

  /// @return the current support threshold
  double getSupport() const;

  /// @return the current confidence threshold
  double getConfidence() const;

  /// @return the name of the output table
  const std::string &getOutTableName() const { return outTableName; }

  /// @return the mining options
  const MineruleOptions::MiningAlgorithms &getMiningAlgorithmsOptions() const {
    return miningAlgorithmsOptions;
  }

  /// @return the MinMaxPair specifying the minimum/maximum number of
  ///   elements that can be present in the head of a mined rule.
  const MinMaxPair &getHeadCardinalities() const { return headCardinalities; }

  /// @return the MinMaxPair specifying the minimum/maximum number of
  ///   elements that can be present in the body of a mined rule.
  const MinMaxPair &getBodyCardinalities() const { return bodyCardinalities; }

  /// @return the source row description the algorithm should use to
  ///    figure out which columns of the source table are to interpreted
  ///    as head/body/group elements.
  const SourceRowColumnIds &getSourceRowDescription() const;
};

} // end minerule namespace

#endif

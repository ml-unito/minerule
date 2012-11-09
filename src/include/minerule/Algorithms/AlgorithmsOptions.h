#ifndef __ALGORITHMS_OPTIONS_H__
#define __ALGORITHMS_OPTIONS_H__

#include <string.h>
#include<odbc++/preparedstatement.h>
#include<odbc++/connection.h>
#include"Database/SourceRow.h"
#include"Utils/MineruleOptions.h"
#include"Parsers/ParsedMinerule.h"
#include"Database/MRResultSet.h"

namespace minerule {

class AlgorithmsOptions {
 private:
  size_t totGroups; // total number of groups in the unfiltered table
  double support;
  double confidence;
  unsigned int rowsPerPartition;
  MinMaxPair headCardinalities;
  MinMaxPair bodyCardinalities;
  odbc::Connection* connection;
  odbc::PreparedStatement* statement;
  // the following will substitute the prepared statement 
  // when the MRResultSetIterator will be integrated in the proj.
  MRResultSetIterator* mriterator;

  HeadBodySourceRowDescription sourceRowDescription;
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
  void setConnection(odbc::Connection*);
  void setStatement(odbc::PreparedStatement*);
  void setSourceRowDescription(const HeadBodySourceRowDescription& srdes);
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
  
  odbc::Connection* getConnection() const;
  odbc::PreparedStatement* getStatement() const;
  const HeadBodySourceRowDescription& 
    getSourceRowDescription() const;
};


} // end minerule namespace

#endif

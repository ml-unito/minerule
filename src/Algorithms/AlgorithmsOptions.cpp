#include "minerule/Algorithms/AlgorithmsOptions.h"

using namespace minerule;

void AlgorithmsOptions::setSupport(double sup)  {
  support = sup;
}

void AlgorithmsOptions::setConfidence(double conf)  {
  confidence = conf;
}

/*void
AlgorithmsOptions::setRowsPerPartition(unsigned int rowPerPart)  {
  rowsPerPartition = rowPerPart;
  }*/

void AlgorithmsOptions::setConnection(odbc::Connection* conn) {
  connection=conn;
}

void AlgorithmsOptions::setStatement(odbc::PreparedStatement* stat) {
  statement=stat;
}

void AlgorithmsOptions::setSourceRowDescription(const SourceRowColumnIds& srdes) {
  sourceRowDescription = srdes;
}

double AlgorithmsOptions::getSupport() const {
  return support;
}

double AlgorithmsOptions::getConfidence() const {
  return confidence;
}

/*
unsigned int 
AlgorithmsOptions::getRowsPerPartition() const {
  return rowsPerPartition;
  }*/

odbc::Connection* AlgorithmsOptions::getODBCConnection() const {
  return connection;
}

odbc::PreparedStatement* AlgorithmsOptions::getStatement() const {
  return statement;
}

const SourceRowColumnIds& AlgorithmsOptions::getSourceRowDescription() const {
  return sourceRowDescription;
}

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
#include "minerule/Utils/MineruleOptions.hpp"
#include <iostream>
#include <fstream>

//#include <malloc.h>

const std::string MINERULE_OPTIONS_PARSING_ERROR =
    "Parsing Error while parsing MineruleOptions";

#include "minerule/Utils/OptionParserLib.hpp"

namespace minerule {
const std::string MineruleOptions::DEFAULT_FILE_NAME("options.txt");

MineruleOptions MineruleOptions::sharedOptions;

MineruleOptions::~MineruleOptions() {
  std::map<std::string, MRLogger *>::iterator it;
  for (it = knownStreams.begin(); it != knownStreams.end(); it++) {
    std::ostream *ostr = NULL;

    if (it->first != "<stdout>" && it->first != "<stderr>") {
      ostr = it->second->getStream();
    }
    delete it->second;
    if (ostr != NULL)
      delete ostr;
  }
}

OptionBase &MineruleOptions::subclassForName(const std::string &oclass) {
  if (oclass == "mrdb")
    return getMRDB();
  else if (oclass == "safety")
    return getSafety();
  else if (oclass == "miningalgorithms")
    return getMiningAlgorithms();
  else if (oclass == "optimizations")
    return getOptimizations();
  else if (oclass == "logstream")
    return getLogStreamObj();
  else if (oclass == "errstream")
    return getErrStreamObj();
  else if (oclass == "warnstream")
    return getWarnStreamObj();
  else if (oclass == "debugstream")
    return getDebugStreamObj();
  else if (oclass == "parsers")
    return getParsers();
  else
    return OptionBase::subclassForName(oclass);
}

void MineruleOptions::init() {
  miningAlgorithms.getRulesMiningAlgorithms()
      .getPartitionBase()
      .setRowsPerPartition(300000); /*300.000*/
  miningAlgorithms.getRulesMiningAlgorithms()
      .getPartitionWithClusters()
      .setRowsPerPartition(300000); /*300.000*/
  miningAlgorithms.getRulesMiningAlgorithms().getFPGrowth().setAlgoType(
      MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::FPGrowth::
          Original);

  optimizations.setTryOptimizations(false);

  MRLogger *stdlog = new MRLogger(std::cout);
  MRLogger *errlog = new MRLogger(std::cerr);

  logStream.setLogger(*stdlog);
  errStream.setLogger(*errlog);
  warnStream.setLogger(*stdlog);
  debugStream.setLogger(*errlog);

  parsers.setLogFILE("/dev/null");

  knownStreams["<stdout>"] = stdlog;
  knownStreams["<stderr>"] = errlog;

  setMineruleSourceName("default-mr-name");
  setMineruleName("default-mr-name");

  ready = true;
}

void MineruleOptions::readFromFile(std::string filename) {
  FILE *file = fopen(filename.c_str(), "r");

  try {
    if (file == NULL) {
      throw MineruleException(MR_ERROR_INPUT_FILE_NOT_FOUND,
                              "Cannot open file:" + filename);
    }
    initializeOptionsFromFile(*this, file);
    mrdb_db.resetConnection();
  } catch (MineruleException &e) {
    if (file != NULL)
      fclose(file);

    throw;
  } catch (mrdb::SQLException &e) {
    if (file != NULL)
      fclose(file);

    throw;
  }

  fclose(file);
}

void MineruleOptions::readFromString(const std::string &str) {
  initializeOptionsFromString(*this, str);

  mrdb_db.resetConnection();
}

std::ostream &MineruleOptions::saveOptions(std::ostream &os) const {
  os << "# Options related to the MRDB connection" << std::endl;
  os << "mrdb::{" << std::endl << "  +name=" << getMRDB().getName() << std::endl
     << "  +username=" << getMRDB().getUsername() << std::endl
     << "  +password=" << getMRDB().getPassword() << std::endl
     << "  +cacheWrites=" << Converter(getMRDB().getCacheWrites()).toString()
     << std::endl << "# dbms allows one to specify the underlying dbms, "
                     "supported dbms are presently" << std::endl
     << "# mysql and postgres" << std::endl << "  +dbms=" << getMRDB().getDBMS()
     << std::endl << "}" << std::endl << std::endl;

  os << "# Options related to data safety issues" << std::endl;
  os << "safety::{" << std::endl
     << "# if the following option is set to 'True', then the" << std::endl
     << "# system will delete old results whenever a new minerule" << std::endl
     << "# having the same name of an old one is inserted. Otherwise"
     << std::endl << "# the system will report an error message and exit."
     << std::endl << "  +overwriteHomonymMinerules="
     << Converter(getSafety().getOverwriteHomonymMinerules()).toString()
     << std::endl << "# if overwriteHomonymMinerules is set to True, then the"
     << std::endl
     << "# following option decides whether the system should delete"
     << std::endl << "# also the minerules for which the result depends on the "
     << std::endl << "# deleted one. If the option is set to True, those "
     << std::endl << "# minerule will be deleted as well, otherwise the system "
     << std::endl << "# with halt reporting an error." << std::endl
     << "  +allowCascadeDeletes="
     << Converter(getSafety().getAllowCascadeDeletes()).toString() << std::endl
     << "}" << std::endl << std::endl;

  os << "# Options related to mining algorithms" << std::endl;
  os << "miningalgorithms::{" << std::endl;
  os << "  #options for configuring rule mining algorithms" << std::endl;
  os << "  rulesmining::{" << std::endl;
  os << "    +preferredAlgorithm="
     << algorithmTypeToString(getMiningAlgorithms()
                                  .getRulesMiningAlgorithms()
                                  .getPreferredAlgorithm()) << std::endl
     << std::endl;
  os << "    # Options related to PartitionBase algorithm" << std::endl;
  os << "    partitionbase::{" << std::endl << "      +rowsPerPartition="
     << getMiningAlgorithms()
            .getRulesMiningAlgorithms()
            .getPartitionBase()
            .getRowsPerPartition() << std::endl << "    }" << std::endl
     << std::endl;

  os << "    # Options related to PartitionWithClusters algorithm" << std::endl;
  os << "    partitionwithclusters::{" << std::endl
     << "      +rowsPerPartition="
     << getMiningAlgorithms()
            .getRulesMiningAlgorithms()
            .getPartitionWithClusters()
            .getRowsPerPartition() << std::endl << "    }" << std::endl
     << std::endl;

  std::string algoType;
  if (getMiningAlgorithms()
          .getRulesMiningAlgorithms()
          .getFPGrowth()
          .getAlgoType() ==
      MiningAlgorithms::RulesMiningAlgorithms::FPGrowth::Original)
    algoType = "Original";
  else
    algoType = "SingleReorder";

  os << "    # Options related to FPGrowth algorithms" << std::endl;
  os << "    fpgrowth::{" << std::endl << "      +algoType=" << algoType
     << std::endl << "    }" << std::endl;
  os << "  }" << std::endl;

  os << " itemsetsmining::{" << std::endl;
  os << "    +preferredAlgorithm="
     << algorithmTypeToString(getMiningAlgorithms()
                                  .getItemsetsMiningAlgorithms()
                                  .getPreferredAlgorithm()) << std::endl
     << std::endl;
  os << " }" << std::endl;
  os << "}" << std::endl << std::endl;

  std::string optimizations;
  if (getOptimizations().getTryOptimizations())
    optimizations = "True";
  else
    optimizations = "False";
  std::string incrAlgorithm;
  switch (getOptimizations().getIncrementalAlgorithm()) {
  case Optimizations::ConstructiveAlgo:
    incrAlgorithm = "constructive";
    break;
  case Optimizations::DestructiveAlgo:
    incrAlgorithm = "destructive";
    break;
  case Optimizations::AutochooseIncrAlgo:
    incrAlgorithm = "Auto";
    break;
  }

  os << "# Options related to Optimizations" << std::endl;
  os << "optimizations::{" << std::endl
     << "  +enableOptimizations=" << optimizations << std::endl
     << "# If set to True, this option will disable the detection of dominant"
     << std::endl << "# queries (this imply also that the system will not try "
                     "to find equivalent" << std::endl
     << "# queries, since they are a particular case of dominance)" << std::endl
     << "  +avoidDominanceDetection="
     << Converter(getOptimizations().getAvoidDominanceDetection()).toString()
     << std::endl << "# If set to True this option will make the optimizer to "
     << std::endl
     << "# consider equivalent queries as if they were dominant ones"
     << std::endl << "# (i.e., it will call an incremental algorithm instead of"
     << std::endl << "# dealing with the equivalence)." << std::endl
     << "  +avoidEquivalenceDetection="
     << Converter(getOptimizations().getAvoidEquivalenceDetection()).toString()
     << std::endl << "# If set to True the optimizer will not try to find "
     << std::endl
     << "# a combinations of previous queries equivalent to the current one."
     << std::endl
     << "# Notice that the search for combination may be a slow process"
     << std::endl << "  +avoidCombinationDetection="
     << Converter(getOptimizations().getAvoidCombinationDetection()).toString()
     << std::endl << "# The following option allows the user to specify how a "
     << std::endl
     << "# particular incremental algorithm  have to be chosen. The"
     << std::endl << "# following values are allowed:{Constructive,Destructive,"
     << std::endl << "# Auto}" << std::endl
     << "# Constructive and Destructive force the corresponding " << std::endl
     << "# algorithm to be chosen. " << std::endl
     << "# Auto leaves the choice to the optimizer." << std::endl
     << "  +incrementalAlgorithm=" << incrAlgorithm << std::endl
     << "# Options related to the query combinator algorithm" << std::endl
     << "  combinator::{" << std::endl
     << "# amount of time the search for a combination is allowed to run "
     << std::endl << "    +timeOut="
     << Converter(getOptimizations().getCombinator().getTimeOutThreshold())
            .toString() << std::endl << "# Max number of disjuncts. It is the "
                                        "number of disjuncts that is considered"
     << std::endl << "# during the search. Notice that increasing this number "
                     "has a strong impact" << std::endl
     << "# on the dimension of the search space." << std::endl
     << "    +maxDisjuncts="
     << Converter(long(getOptimizations().getCombinator().getMaxDisjuncts()))
            .toString() << std::endl << "# Max number of queries. Max number "
                                        "of distinct queries the user allows to"
     << std::endl << "# be combined in the result. Formulae with a larger "
                     "number of queries are" << std::endl
     << "# penalized in the evaluation function." << std::endl
     << "    +maxQueries="
     << Converter(long(getOptimizations().getCombinator().getMaxQueries()))
            .toString() << std::endl << "# Max distinct predicates. Max number "
                                        "of distinct predicates that the user"
     << std::endl << "# allows. This afflict the response time: the time spent "
                     "in assessing each" << std::endl
     << "# formula grows exponentially fast with the number of predicates."
     << std::endl << "    +maxDistinctPredicates="
     << Converter(
            long(getOptimizations().getCombinator().getMaxDistinctPredicates()))
            .toString() << std::endl << "  }" << std::endl << "}" << std::endl
     << std::endl;
  os << "# Options related to the parsing algorithms" << std::endl;
  os << "parsers::{" << std::endl;
  os << "# Parsers log stream, valid names are:" << std::endl;
  os << "#   <stdout>, <stderr> and any writeable file." << std::endl;
  os << " +logfile=/dev/null" << std::endl;
  os << "# The following four options allows to set constraint on" << std::endl
     << "# cardinalities of elements which appears in the body/head"
     << std::endl
     << "# part of rules. The constraints set here 'win' on the ones"
     << std::endl
     << "# in minerules (i.e., if you say '1..n' as BODY in your minerule"
     << std::endl
     << "# but set it to 1..5 here, than 1..5 will be used instead."
     << std::endl;
  os << " +minBodyElems=" << getParsers().getBodyCardinalities().getMin()
     << std::endl;
  os << " +maxBodyElems=" << getParsers().getBodyCardinalities().getMax()
     << std::endl;
  os << " +minHeadElems=" << getParsers().getHeadCardinalities().getMin()
     << std::endl;
  os << " +maxHeadElems=" << getParsers().getHeadCardinalities().getMax()
     << std::endl;
  os << "}" << std::endl << std::endl;

  os << "# Options related to streams, note that they are commented."
     << std::endl
     << "# the reason is that the following conresponds to default "
     << std::endl << "# settings instead of the actual ones." << std::endl
     << "# In specifying the stream parameter, you can:" << std::endl
     << "# 1) Specify a file path" << std::endl
     << "# 2) Specify <stdout>,<stderr> in order to specify the standard"
     << std::endl << "#    output and the standard error respectively"
     << std::endl << "# 3) Specify a file path including the %m and %i symbols"
     << std::endl
     << "# In case 3) %m is expandend to the current minerule name as"
     << std::endl
     << "# it appear in the minerule text, %i is expanded to the value"
     << std::endl << "# of the -i parameter if any, to 'mr' otherwise"
     << std::endl
     << "# The loglevel option allows you to select how deep the log nesting"
     << std::endl << "# can grow. The default (loglevel==100) means: \"grow as "
                     "much as needed\"" << std::endl
     << "# (no function in minerule will ever nest logs more than a few "
        "levels)." << std::endl << "# If set to 0, then the log is actually "
                                   "suppressed (the first level is 1)."
     << std::endl
     << "# Otherwise, if set to n, only the first n levels will be displayed."
     << std::endl << "#" << std::endl << "# logstream::{" << std::endl
     << "#    +stream=<stdout> " << std::endl << "#    +loglevel=100"
     << std::endl << "# }" << std::endl << "# errstream::{" << std::endl
     << "#    +stream=<stderr> " << std::endl << "#    +loglevel=100"
     << std::endl << "# }" << std::endl << "# warnstream::{" << std::endl
     << "#    +stream=<stdout> " << std::endl << "#    +loglevel=100"
     << std::endl << "# }" << std::endl << "# debugstream::{" << std::endl
     << "#    +stream=<stderr> " << std::endl << "#    +loglevel=100"
     << std::endl << "# }" << std::endl;

  return os;
}

void MineruleOptions::Parsers::setLogFILE(const std::string &fname) {
  clearStream();
  logfile = fopen(fname.c_str(), "w");
  if (logfile == NULL)
    throw MineruleException(
        MR_ERROR_OUTPUT_FILE_PROBLEM,
        std::string("Error while parsing options,I've tried to open file:") +
            fname + std::string(", but an error occurred") +
            std::string(" the reason is:\n") + strerror(errno));
}

void MineruleOptions::Parsers::setLogOnStdout() { logfile = stdout; }

void MineruleOptions::Parsers::setLogOnStderr() { logfile = stderr; }

void MineruleOptions::Mrdb::setOption(
    const std::string &name,
    const std::string &value) {
  if (name == "name")
    setName(value);
  else if (name == "username")
    setUsername(value);
  else if (name == "password")
    setPassword(value);
  else if (name == "cacheWrites")
    setCacheWrites(Converter(value).toBool());
  else if (name == "dbms")
    setDBMS(value);
  else {
    std::cerr << "Error while parsing options, expecting an mrdb option in:"
              << std::endl
              << "{name, userName, password, cacheWrites,dbms} and: "
              << std::endl << "\"" << name << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::Safety::setOption(
    const std::string &name,
    const std::string &value) {
  try {
    if (name == "overwriteHomonymMinerules") {
      setOverwriteHomonymMinerules(Converter(value).toBool());
    } else if (name == "allowCascadeDeletes") {
      setAllowCascadeDeletes(Converter(value).toBool());
    } else {
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              "Expected a value in "
                              "{overwriteHomonymMinerules,allowCascadeDeletes},"
                              " but " +
                                  value + " found.");
    }
  } catch (MineruleException &e) {
    std::cerr
        << "Parsing error while parsing a safety option(given option name:"
        << name << " given option value:" << value << "). The reason for"
        << " the error is:" << e.what() << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::PartitionBase::
    setOption(const std::string &name,
              const std::string &value) {
  if (name == "rowsPerPartition") {
    unsigned int rpp = stringToLong(value, name);
    setRowsPerPartition(rpp);
  } else {
    std::cerr << "Error while parsing options, expecting a partition option in:"
              << std::endl << "{rowsPerPartition} and: " << std::endl << "\""
              << name << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::
    PartitionWithClusters::setOption(
        const std::string &name,
        const std::string &value) {
  if (name == "rowsPerPartition") {
    unsigned int rpp = stringToLong(value, name);
    setRowsPerPartition(rpp);
  } else {
    std::cerr << "Error while parsing options, expecting a partition option in:"
              << std::endl << "{rowsPerPartition} and: " << std::endl << "\""
              << name << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::FPGrowth::
    setOption(const std::string &name,
              const std::string &value) {
  if (name == "algoType") {
    if (value == "Original") {
      setAlgoType(Original);
    } else if (value == "SingleReorder") {
      setAlgoType(SingleReorder);
    } else {
      std::cerr
          << "Error while parsing options, expecting a fpgrowth::algoType "
          << " value in:" << std::endl
          << "{Original,SingleReorder} and: " << std::endl << "\"" << value
          << "\" found." << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else {
    std::cerr << "Error while parsing options, expecting a fpgrowth option in:"
              << std::endl << "{algoType} and: " << std::endl << "\"" << name
              << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::setOption(
    const std::string &name,
    const std::string &value) {
  if (name == "preferredAlgorithm") {
    try {
      setPreferredAlgorithm(stringToAlgorithmType(value));
    } catch (MineruleException e) {
      std::cerr
          << "Error while parsing options, expecting an algorithm option in {"
          << stringWithListOfAlgorithmTypes() << "}, but \"" << value
          << "\" found." << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else {
    std::cerr << "Error while parsing options, expecting a miningalgorithms "
                 "option in:" << std::endl
              << "{preferredAlgorithm} and: " << std::endl << "\"" << name
              << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::MiningAlgorithms::ItemsetsMiningAlgorithms::setOption(
    const std::string &name,
    const std::string &value) {
  if (name == "preferredAlgorithm") {
    try {
      setPreferredAlgorithm(stringToAlgorithmType(value));
    } catch (MineruleException e) {
      std::cerr
          << "Error while parsing options, expecting an algorithm option in {"
          << stringWithListOfAlgorithmTypes() << "}, but \"" << value
          << "\" found." << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else {
    std::cerr << "Error while parsing options, expecting a miningalgorithms "
                 "option in:" << std::endl
              << "{preferredAlgorithm} and: " << std::endl << "\"" << name
              << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::MiningAlgorithms::SequencesMiningAlgorithms::setOption(
    const std::string &name,
    const std::string &value) {
  if (name == "preferredAlgorithm") {
    try {
      setPreferredAlgorithm(stringToAlgorithmType(value));
    } catch (MineruleException e) {
      std::cerr
          << "Error while parsing options, expecting an algorithm option in {"
          << stringWithListOfAlgorithmTypes() << "}, but \"" << value
          << "\" found." << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else {
    std::cerr << "Error while parsing options, expecting a miningalgorithms "
                 "option in:" << std::endl
              << "{preferredAlgorithm} and: " << std::endl << "\"" << name
              << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::Optimizations::setOption(
    const std::string &name,
    const std::string &value) {
  if (name == "enableOptimizations") {
    if (value == "True") {
      setTryOptimizations(true);
    } else if (value == "False") {
      setTryOptimizations(false);
    } else {
      std::cerr << "Error while parsing options, expecting an "
                << "optimizations::tryOptimizations value in " << std::endl
                << "{True,False} and: " << std::endl << "\"" << value
                << "\" found." << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else if (name == "incrementalAlgorithm") {
    if (value == "Constructive") {
      setIncrementalAlgorithm(Optimizations::ConstructiveAlgo);
    } else if (value == "Destructive") {
      setIncrementalAlgorithm(Optimizations::DestructiveAlgo);
    } else if (value == "Auto") {
      setIncrementalAlgorithm(Optimizations::AutochooseIncrAlgo);
    } else {
      std::cerr << "Error while parsing options, expecting an "
                   "optimizations::incrementalAlgorithm value" << std::endl
                << "in {Constructive,Destructive,Auto}, but \"" + value +
                       "\" found." << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else if (name == "avoidDominanceDetection") {
    try {
      setAvoidDominanceDetection(Converter(value).toBool());
    } catch (MineruleException &e) {
      std::cerr
          << "Error while parsing avoidDominanceDetection option, expecting "
             "a value in {True, False}, but " << value << "found" << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else if (name == "avoidEquivalenceDetection") {
    try {
      setAvoidEquivalenceDetection(Converter(value).toBool());
    } catch (MineruleException &e) {
      std::cerr
          << "Error while parsing avoidEquivalenceDetection option, expecting "
             "a value in {True, False}, but " << value << "found" << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else if (name == "avoidCombinationDetection") {
    try {
      setAvoidCombinationDetection(Converter(value).toBool());
    } catch (MineruleException &e) {
      std::cerr
          << "Error while parsing avoidCombinationDetection option, expecting "
             "a value in {True, False}, but " << value << "found" << std::endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,
                              MINERULE_OPTIONS_PARSING_ERROR);
    }
  } else {
    std::cerr
        << "Error while parsing options, expecting an optimization option in:"
        << std::endl << "{enableOptimizations, incrementalAlgorithm, "
                        "avoidEquivalenceDetection"
        << ", tryOptimizationThruCombination} and: " << std::endl << "\""
        << name << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::Optimizations::Combinator::setOption(
    const std::string &name,
    const std::string &value) {

  if (name == "timeOut")
    setTimeOutThreshold(Converter(value).toDouble());
  else if (name == "maxDisjuncts")
    setMaxDisjuncts(Converter(value).toLong());
  else if (name == "maxQueries")
    setMaxQueries(Converter(value).toLong());
  else if (name == "maxDistinctPredicates")
    setMaxDistinctPredicates(Converter(value).toLong());
  else {
    std::cerr << "Error while parsing options, expecting a combinator option i:"
              << std::endl
              << "{timeOut,maxDisjuncts,maxQueries,maxDistinctPredicates}, but"
              << std::endl << "\"" << name << "\" found." << std::endl;

    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::OutStream::setOption(
    const std::string &name,
    const std::string &value) {
  // we need to modify value, hence we made a copy of it
  // and use it in the rest of the procedure
  std::string valueCopy = value;
  std::map<std::string, MRLogger *> &knownStreams =
      getSharedOptions().knownStreams;

  if (name == "stream") {
    size_t markerPos;
    if ((markerPos = valueCopy.find("%i")) != valueCopy.npos) {
      valueCopy.erase(markerPos, 2);
      valueCopy.insert(markerPos, options->getMineruleSourceName());
    }

    if ((markerPos = valueCopy.find("%m")) != valueCopy.npos) {
      valueCopy.erase(markerPos, 2);
      valueCopy.insert(markerPos, options->getMineruleName());
    }

    if (knownStreams.find(valueCopy) == knownStreams.end()) {
      std::ostream *ostrptr(new std::ofstream(valueCopy.c_str()));

      if (!*ostrptr) {
        std::cerr << "Error while parsing options, failure while trying to"
                  << std::endl << "open in output the following file "
                  << valueCopy << std::endl
                  << "Possible values for the stream options are:"
                  << "{<stdin>,<stdout>,anyStringRepresentingAFileName}"
                  << std::endl << "Note you can also use file names having the "
                                  "modifier %i or %m inside." << std::endl
                  << "In that case %i is expandended into the current value of "
                     "the -i parameter" << std::endl
                  << "(provided that the -i parameter is used instead of the "
                     "-m one, otherwise, the" << std::endl
                  << "default 'mr' value is assumed). Analogously, the %m "
                     "parameter will be" << std::endl
                  << "substituted with the current minerule name (as it appear "
                     "in the minerule text" << std::endl;
        throw MineruleException(MR_ERROR_OPTION_PARSING,
                                MINERULE_OPTIONS_PARSING_ERROR);
      }

      MRLogger *logger = new MRLogger(*ostrptr);
      knownStreams[valueCopy] = logger;
    }

    setLogger(*knownStreams[valueCopy]);
  } else if (name == "loglevel") {
    try {
      size_t level = Converter(valueCopy).toLong();
      setLogLevel(level);
    } catch (MineruleException &e) {
      throw MineruleException(
          MR_ERROR_OPTION_PARSING,
          "Error parsing options while converting loglevel value `" +
              valueCopy + "' to int.");
    }
  } else {
    std::cerr << "Error while parsing options, expecting a stream option in:"
              << std::endl << "{stream} and: " << std::endl << "\"" << name
              << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

void MineruleOptions::Parsers::setOption(
    const std::string &name,
    const std::string &value) {
  if (name == "logfile") {
    if (value == "<stdout>")
      setLogOnStdout();
    else if (value == "<stderr>")
      setLogOnStderr();
    else
      setLogFILE(value);
  } else if (name == "minBodyElems") {
    int min = stringToLong(value, name);
    setMinBodyElems(min);
  } else if (name == "maxBodyElems") {
    int max = stringToLong(value, name);
    setMaxBodyElems(max);
  } else if (name == "minHeadElems") {
    int min = stringToLong(value, name);
    setMinHeadElems(min);
  } else if (name == "maxHeadElems") {
    int max = stringToLong(value, name);
    setMaxHeadElems(max);
  } else {
    std::cerr << "Error while parsing options, expecting a parser option in:"
              << std::endl << "{logfile, minBodyElems, maxBodyElems, "
                              "minHeadElems, maxHeadElems} and: " << std::endl
              << "\"" << name << "\" found." << std::endl;
    throw MineruleException(MR_ERROR_OPTION_PARSING,
                            MINERULE_OPTIONS_PARSING_ERROR);
  }
}

} // namespace

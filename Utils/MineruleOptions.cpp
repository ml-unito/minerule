#include "MineruleOptions.h"
#include <iostream>
#include <fstream>

using namespace std;
//#include <malloc.h>

#include<odbc++/drivermanager.h>

const string MINERULE_OPTIONS_PARSING_ERROR = "Parsing Error while parsing MineruleOptions";

#include "OptionParserLib.h"

namespace minerule {

  MineruleOptions MineruleOptions::sharedOptions;

  MineruleOptions::~MineruleOptions() {
    map<string, MRLogger*>::iterator it;
    for(it=knownStreams.begin(); it!=knownStreams.end(); it++) {
      ostream* ostr=NULL;
      
      if( it->first!="<stdout>" &&
	  it->first!="<stderr>" ) {
	ostr=it->second->getStream();
      }
      delete it->second;
      if(ostr!=NULL)
	delete ostr;
    }
  }

  OptionBase& MineruleOptions::subclassForName(const std::string& oclass)
  throw(MineruleException) {
      if(oclass=="odbc")
	return getOdbc_db();
      else if(oclass=="safety")
	return getSafety();
      else if(oclass=="miningalgorithms")
	return getMiningAlgorithms();
      else if(oclass=="optimizations")
	return getOptimizations();
      else if(oclass=="logstream")
	return getLogStreamObj();
      else if(oclass=="errstream")
	return getErrStreamObj();
      else if(oclass=="warnstream")
	return getWarnStreamObj();
      else if(oclass=="debugstream")
	return getDebugStreamObj();
      else if(oclass=="parsers")
	return getParsers();
      else return OptionBase::subclassForName(oclass);
    }
  

  
  void
  MineruleOptions::init() {
    miningAlgorithms.getRulesMiningAlgorithms().getPartitionBase().setRowsPerPartition(300000); /*300.000*/
    miningAlgorithms.getRulesMiningAlgorithms().getPartitionWithClusters().setRowsPerPartition(300000); /*300.000*/
    miningAlgorithms.getRulesMiningAlgorithms().getFPGrowth().setAlgoType(MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::FPGrowth::Original);
    

    optimizations.setTryOptimizations(false);

    MRLogger* stdlog=new MRLogger(cout);
    MRLogger* errlog=new MRLogger(cerr);

    logStream.setLogger(*stdlog);
    errStream.setLogger(*errlog);
    warnStream.setLogger(*stdlog);
    debugStream.setLogger(*errlog);

    parsers.setLogFILE("/dev/null");

    knownStreams["<stdout>"]=stdlog;
    knownStreams["<stderr>"]=errlog;

    setMineruleSourceName("mr");
    setMineruleName("mr");

    ready = true;
  }

  void
  MineruleOptions::readFromFile(string filename) throw(MineruleException){
    FILE* file = fopen(filename.c_str(), "r");

    try {
      if(file==NULL) {
	throw MineruleException(MR_ERROR_INPUT_FILE_NOT_FOUND,
				"Cannot open file:"+filename);
      }
      initializeOptionsFromFile(*this,file);
      odbc_db.resetConnection();
    } catch(MineruleException& e) {
      if(file!=NULL)
	fclose(file);
      
      cerr << e.what() << endl;
      throw;
    } catch(odbc::SQLException& e) {
      if( file!=NULL )
	fclose(file);

      cerr << e.getMessage() << endl;
      throw;
    }

    fclose(file);
  }

  void
  MineruleOptions::readFromString(const string& str) throw(MineruleException, odbc::SQLException){
    initializeOptionsFromString(*this,str);

    odbc_db.resetConnection();
  }

  ostream& MineruleOptions::saveOptions(ostream& os) const {
    os << "# Options related to the ODBC connection" << endl;
    os << "odbc::{" << endl
       << "  name="<< getOdbc_db().getName() <<endl
       << "  username="<<getOdbc_db().getUsername() << endl
       << "  password="<<getOdbc_db().getPassword() << endl
       << "  cacheWrites="<<Converter(getOdbc_db().getCacheWrites()).toString() << endl
       << "}" << endl << endl;

    os << "# Options related to data safety issues" << endl;
    os << "safety::{" << endl
       << "# if the following option is set to 'True', then the"<<endl
       << "# system will delete old results whenever a new minerule"<<endl
       << "# having the same name of an old one is inserted. Otherwise"<<endl
       << "# the system will report an error message and exit." << endl
       << "  overwriteHomonymMinerules=" 
       << Converter( getSafety().getOverwriteHomonymMinerules() ).toString() << endl
       << "# if overwriteHomonymMinerules is set to True, then the" << endl
       << "# following option decides whether the system should delete"<<endl
       << "# also the minerules for which the result depends on the " << endl
       << "# deleted one. If the option is set to True, those " << endl
       << "# minerule will be deleted as well, otherwise the system " <<endl
       << "# with halt reporting an error." << endl
       << "  allowCascadeDeletes=" 
       << Converter( getSafety().getAllowCascadeDeletes() ).toString() << endl
       << "}" << endl << endl;

    os << "# Options related to mining algorithms" << endl;
    os << "miningalgorithms::{" << endl;
    os << "  #options for configuring rule mining algorithms" << endl;
    os << "  rulesmining::{" << endl;
    os << "    preferredAlgorithm=" << algorithmTypeToString(getMiningAlgorithms().getRulesMiningAlgorithms().getPreferredAlgorithm()) << endl << endl;
    os << "    # Options related to PartitionBase algorithm" << endl;
    os << "    partitionbase::{" << endl
       << "      rowsPerPartition=" << 
                   getMiningAlgorithms().getRulesMiningAlgorithms().getPartitionBase().getRowsPerPartition() << endl
       << "    }" << endl << endl;

    os << "    # Options related to PartitionWithClusters algorithm" << endl;
    os << "    partitionwithclusters::{" << endl
       << "      rowsPerPartition=" << 
                   getMiningAlgorithms().getRulesMiningAlgorithms().getPartitionWithClusters().getRowsPerPartition() << endl
       << "    }" << endl << endl;
    
    string algoType;
    if(getMiningAlgorithms().getRulesMiningAlgorithms().getFPGrowth().getAlgoType()==MiningAlgorithms::RulesMiningAlgorithms::FPGrowth::Original)
      algoType="Original";
    else
      algoType="SingleReorder";

    os << "    # Options related to FPGrowth algorithms" << endl;
    os << "    fpgrowth::{" << endl
       << "      algoType=" << algoType << endl
       << "    }" <<endl;
    os << "  }" << endl;
    
    os << " itemsetsmining::{" << endl;
    os << "    preferredAlgorithm=" << algorithmTypeToString(getMiningAlgorithms().getItemsetsMiningAlgorithms().getPreferredAlgorithm()) << endl << endl;
    os << " }" << endl;
    os << "}" << endl << endl;

    string optimizations;
    if(getOptimizations().getTryOptimizations())
      optimizations="True";
    else
      optimizations="False";
    string incrAlgorithm;
    switch(getOptimizations().getIncrementalAlgorithm()) {
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

    os << "# Options related to Optimizations" << endl;
    os << "optimizations::{" << endl
       << "  enableOptimizations=" << optimizations << endl
       << "# If set to True, this option will disable the detection of dominant" << endl
       << "# queries (this imply also that the system will not try to find equivalent" << endl
       << "# queries, since they are a particular case of dominance)" << endl
       << "  avoidDominanceDetection=" 
       << Converter( getOptimizations().getAvoidDominanceDetection() ).toString() << endl
       << "# If set to True this option will make the optimizer to "<<endl
       << "# consider equivalent queries as if they were dominant ones"<<endl
       << "# (i.e., it will call an incremental algorithm instead of"<< endl
       << "# dealing with the equivalence)." << endl
       << "  avoidEquivalenceDetection=" 
       << Converter( getOptimizations().getAvoidEquivalenceDetection() ).toString() << endl
       << "# If set to True the optimizer will not try to find " << endl
       << "# a combinations of previous queries equivalent to the current one." << endl
       << "# Notice that the search for combination may be a slow process" << endl
       << "  avoidCombinationDetection=" 
       << Converter(getOptimizations().getAvoidCombinationDetection() ).toString() << endl
       << "# The following option allows the user to specify how a " << endl
       << "# particular incremental algorithm  have to be chosen. The" <<endl
       << "# following values are allowed:{Constructive,Destructive," << endl
       << "# Auto}"<<endl
       << "# Constructive and Destructive force the corresponding "<<endl
       << "# algorithm to be chosen. " << endl
       << "# Auto leaves the choice to the optimizer." << endl
       << "  incrementalAlgorithm=" << incrAlgorithm << endl
       << "# Options related to the query combinator algorithm" << endl
       << "  combinator {" << endl
       << "# amount of time the search for a combination is allowed to run " << endl
       << "    timeOut=" 
       << Converter(getOptimizations().getCombinator().getTimeOutThreshold()).toString() << endl
       << "# Max number of disjuncts. It is the number of disjuncts that is considered" << endl
       << "# during the search. Notice that increasing this number has a strong impact" << endl
       << "# on the dimension of the search space." << endl 
       << "    maxDisjuncts=" 
       << Converter(long(getOptimizations().getCombinator().getMaxDisjuncts())).toString() << endl
       << "# Max number of queries. Max number of distinct queries the user allows to" <<endl
       << "# be combined in the result. Formulae with a larger number of queries are" << endl
       << "# penalized in the evaluation function." << endl
       << "    maxQueries="
       << Converter(long(getOptimizations().getCombinator().getMaxQueries())).toString() << endl
       << "# Max distinct predicates. Max number of distinct predicates that the user"<<endl
       << "# allows. This afflict the response time: the time spent in assessing each" <<endl
       << "# formula grows exponentially fast with the number of predicates." << endl
       << "    maxDistinctPredicates="
       << Converter(long(getOptimizations().getCombinator().getMaxDistinctPredicates())).toString()<<endl
       << "  }" << endl
       << "}" <<endl << endl;
    os << "# Options related to the parsing algorithms"<< endl;
    os << "parsers::{" << endl;
    os << "# Parsers log stream, valid names are:" << endl;
    os << "#   <stdout>, <stderr> and any writeable file." << endl;
    os << " logfile=/dev/null" << endl;
    os << "# The following four options allows to set constraint on" << endl
       << "# cardinalities of elements which appears in the body/head" <<endl
       << "# part of rules. The constraints set here 'win' on the ones"<<endl
       << "# in minerules (i.e., if you say '1..n' as BODY in your minerule"<<endl
       <<"# but set it to 1..5 here, than 1..5 will be used instead." <<endl;
    os <<" minBodyElems=" << 
      getParsers().getBodyCardinalities().getMin() <<endl;
    os <<" maxBodyElems=" 
       << getParsers().getBodyCardinalities().getMax() << endl;
    os <<" minHeadElems=" 
       << getParsers().getHeadCardinalities().getMin() << endl;
    os <<" maxHeadElems="
       << getParsers().getHeadCardinalities().getMax()<<endl;
    os << "}" << endl << endl;
    
    os << "# Options related to streams, note that they are commented." << endl
       << "# the reason is that the following conresponds to default " << endl
       << "# settings instead of the actual ones." << endl
       << "# In specifying the stream parameter, you can:" << endl
       << "# 1) Specify a file path" << endl
       << "# 2) Specify <stdout>,<stderr> in order to specify the standard" << endl
       << "#    output and the standard error respectively" << endl
       << "# 3) Specify a file path including the %m and %i symbols"  << endl
       << "# In case 3) %m is expandend to the current minerule name as" << endl
       << "# it appear in the minerule text, %i is expanded to the value" << endl
       << "# of the -i parameter if any, to 'mr' otherwise" << endl
       << "#" << endl
       << "# logstream::{" << endl
       << "#    stream=<stdout> " << endl
       << "#    loglevel=100" << endl
       << "# }" << endl
       << "# errstream::{" << endl
       << "#    stream=<stderr> " << endl
       << "#    loglevel=100" << endl
       << "# }" << endl
       << "# warnstream::{" << endl
       << "#    stream=<stdout> " << endl
       << "#    loglevel=100" << endl
       << "# }" << endl
       << "# debugstream::{" << endl
       << "#    stream=<stderr> " << endl
       << "#    loglevel=100" << endl
       << "# }" << endl;
    
    return os;
  }


  ostream& MRLog(int id) {
    return MineruleOptions::getSharedOptions().getLogStream();
  }

  ostream& MRErr(int id) {
    return MineruleOptions::getSharedOptions().getErrStream();
  }

  ostream& MRWarn(int id) {
    return MineruleOptions::getSharedOptions().getWarnStream();
  }

  ostream& MRDebug(int id) {
    return MineruleOptions::getSharedOptions().getDebugStream();
  }

  int MRLogGetNewID() {
    static int logId = 4;
    return logId++;
  }

  void MRLogPush(const string& descr) {
    MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().push(descr);
  }

  void MRLogPop() {
    MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().pop();
  }

  void MRErrPush(const string& descr) {
    MineruleOptions::getSharedOptions().getErrStreamObj().getLogger().push(descr);
  }

  void MRErrPop() {
    MineruleOptions::getSharedOptions().getErrStreamObj().getLogger().pop();
  }

  void MRWarnPush(const string& descr) {
    MineruleOptions::getSharedOptions().getWarnStreamObj().getLogger().push(descr);
  }

  void MRWarnPop() {
    MineruleOptions::getSharedOptions().getWarnStreamObj().getLogger().pop();
  }

  void MRDebugPush(const string& descr) {
    MineruleOptions::getSharedOptions().getDebugStreamObj().getLogger().push(descr);
  }

  void MRDebugPop() {
    MineruleOptions::getSharedOptions().getDebugStreamObj().getLogger().pop();
  }

  
  void
  MineruleOptions::Parsers::setLogFILE(const string& fname) throw(MineruleException) {
    clearStream();
    logfile = fopen(fname.c_str(), "w");
    if(logfile==NULL) 
      throw MineruleException(MR_ERROR_OUTPUT_FILE_PROBLEM,
			      string("Error while parsing options,I've tried to open file:")
			      +fname+
			      string(", but an error occurred")
			      +string(" the reason is:\n") + strerror(errno));
  }

  void
  MineruleOptions::Parsers::setLogOnStdout() {
    logfile=stdout;
  }

  void
  MineruleOptions::Parsers::setLogOnStderr() {
    logfile=stderr;
  }

 


  void 
  MineruleOptions::Odbc_db::setOption(const string& name, 
				      const string& value) 
                      throw(MineruleException) {
    if(name=="name")
      setName(value);
    else if(name=="username")
      setUsername(value);
    else if(name=="password")
      setPassword(value);
    else if(name=="cacheWrites")
      setCacheWrites(Converter(value).toBool());
    else {
      cerr << "Error while parsing options, expecting an odbc option in:" <<endl
	   << "{name, userName, password, cacheWrites} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }

  void 
  MineruleOptions::Safety::setOption(const string& name,
				     const string& value)
    throw(MineruleException) {
    try {
      if(name=="overwriteHomonymMinerules") {
	setOverwriteHomonymMinerules( Converter(value).toBool() );
      } else if( name == "allowCascadeDeletes" ) {
	setAllowCascadeDeletes( Converter(value).toBool() );
      } else {
	throw MineruleException( MR_ERROR_OPTION_PARSING, 
				 "Expected a value in {overwriteHomonymMinerules,allowCascadeDeletes}, but "+value+" found.");
      }
    } catch (MineruleException& e) {
      cerr << "Parsing error while parsing a safety option(given option name:"
	   << name << " given option value:" << value << "). The reason for"
	   << " the error is:" << e.what() << endl;
      throw MineruleException( MR_ERROR_OPTION_PARSING, 
			       MINERULE_OPTIONS_PARSING_ERROR);
    }
  }

  
  void
  MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::PartitionBase::setOption(const string& name, 
					    const string& value) throw(MineruleException) {
    if(name=="rowsPerPartition") {
      unsigned int rpp = stringToLong(value,name);
      setRowsPerPartition(rpp);
    }
    else {
      cerr << "Error while parsing options, expecting a partition option in:"
	   <<endl
	   << "{rowsPerPartition} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }


  void
  MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::PartitionWithClusters::setOption(const string& name, 
					    const string& value) throw(MineruleException) {
    if(name=="rowsPerPartition") {
      unsigned int rpp = stringToLong(value,name);
      setRowsPerPartition(rpp);
    }
    else {
      cerr << "Error while parsing options, expecting a partition option in:"
	   <<endl
	   << "{rowsPerPartition} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }
  
  void 
  MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::FPGrowth::setOption(const string& name, 
							 const string& value) throw(MineruleException) {
    if(name=="algoType") {
      if(value=="Original") {
	setAlgoType(Original);
      } else if(value=="SingleReorder") {
	setAlgoType(SingleReorder);
      } else {
	cerr << "Error while parsing options, expecting a fpgrowth::algoType "
	     <<" value in:" <<endl
	     << "{Original,SingleReorder} and: " << endl
	     << "\"" << value << "\" found." << endl;
	throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
      }
    }
    else {
      cerr << "Error while parsing options, expecting a fpgrowth option in:" 
	   <<endl
	   << "{algoType} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }

  void
  MineruleOptions::MiningAlgorithms::RulesMiningAlgorithms::setOption(const string& name, 
					       const string& value) throw(MineruleException) {
    if(name=="preferredAlgorithm") {
      try {
	setPreferredAlgorithm( stringToAlgorithmType(value) );
      } catch (MineruleException e) {
	cerr << "Error while parsing options, expecting an algorithm option in {" <<
	  stringWithListOfAlgorithmTypes() << "}, but \"" << value << "\" found." << endl;
	throw MineruleException( MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else {
      cerr << "Error while parsing options, expecting a miningalgorithms option in:" 
	   << endl
	   << "{preferredAlgorithm} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }

  void
  MineruleOptions::MiningAlgorithms::ItemsetsMiningAlgorithms::setOption(const string& name, 
									 const string& value) throw(MineruleException) {
    if(name=="preferredAlgorithm") {
      try {
	setPreferredAlgorithm( stringToAlgorithmType(value) );
      } catch (MineruleException e) {
	cerr << "Error while parsing options, expecting an algorithm option in {" <<
	  stringWithListOfAlgorithmTypes() << "}, but \"" << value << "\" found." << endl;
	throw MineruleException( MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else {
      cerr << "Error while parsing options, expecting a miningalgorithms option in:" 
	   << endl
	   << "{preferredAlgorithm} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }

  void
  MineruleOptions::MiningAlgorithms::SequencesMiningAlgorithms::setOption(const string& name,
                                                                         const string& value) throw(MineruleException) {
    if(name=="preferredAlgorithm") {
      try {
        setPreferredAlgorithm( stringToAlgorithmType(value) );
      } catch (MineruleException e) {
        cerr << "Error while parsing options, expecting an algorithm option in {" <<
          stringWithListOfAlgorithmTypes() << "}, but \"" << value << "\" found." << endl;
        throw MineruleException( MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else {
      cerr << "Error while parsing options, expecting a miningalgorithms option in:"
           << endl
           << "{preferredAlgorithm} and: " << endl
           << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }




  
  void 
  MineruleOptions::Optimizations::setOption(const string& name, 
					    const string& value) 
                                            throw(MineruleException) {
    if(name=="enableOptimizations") {
      if(value=="True") {
	setTryOptimizations(true);
      } else if(value=="False") {
	setTryOptimizations(false);
      } else {
	cerr << "Error while parsing options, expecting an "
	     << "optimizations::tryOptimizations value in " <<endl
	   << "{True,False} and: " << endl
	   << "\"" << value << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else if(name=="incrementalAlgorithm") {
      if(value=="Constructive") {
	setIncrementalAlgorithm(Optimizations::ConstructiveAlgo);
      } else if(value=="Destructive") {
	setIncrementalAlgorithm(Optimizations::DestructiveAlgo);
      } else if(value=="Auto") {
	setIncrementalAlgorithm(Optimizations::AutochooseIncrAlgo);
      } else {
	cerr << "Error while parsing options, expecting an optimizations::incrementalAlgorithm value" << endl
	     << "in {Constructive,Destructive,Auto}, but \""+value+"\" found." << endl;
	throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else if(name=="avoidDominanceDetection") {
      try {
	setAvoidDominanceDetection( Converter(value).toBool() );
      } catch (MineruleException& e) {
	cerr << "Error while parsing avoidDominanceDetection option, expecting "
	  "a value in {True, False}, but " << value << "found" << endl;
	throw MineruleException( MR_ERROR_OPTION_PARSING,
				 MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else if(name=="avoidEquivalenceDetection") {
      try {
	setAvoidEquivalenceDetection( Converter(value).toBool() );
      } catch (MineruleException& e) {
	cerr << "Error while parsing avoidEquivalenceDetection option, expecting "
	  "a value in {True, False}, but " << value << "found" << endl;
	throw MineruleException( MR_ERROR_OPTION_PARSING,
				 MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else if(name=="avoidCombinationDetection") {
      try {
	setAvoidCombinationDetection( Converter(value).toBool() );
      } catch (MineruleException& e) {
	cerr << "Error while parsing avoidCombinationDetection option, expecting "
	  "a value in {True, False}, but " << value << "found" << endl;
	throw MineruleException( MR_ERROR_OPTION_PARSING,
				 MINERULE_OPTIONS_PARSING_ERROR);
      }
    } else {
      cerr << "Error while parsing options, expecting an optimization option in:" 
	   <<endl
	   << "{enableOptimizations, incrementalAlgorithm, avoidEquivalenceDetection"
	   << ", tryOptimizationThruCombination} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,MINERULE_OPTIONS_PARSING_ERROR);
    }
  }

  void MineruleOptions::Optimizations::Combinator::setOption(const std::string& name, 
							    const std::string& value)
    throw(MineruleException) {

    if( name=="timeOut" )
      setTimeOutThreshold(Converter(value).toDouble());
    else if(name=="maxDisjuncts") 
      setMaxDisjuncts( Converter(value).toLong() );
    else if(name=="maxQueries")
      setMaxQueries( Converter(value).toLong() );
    else if(name=="maxDistinctPredicates")
      setMaxDistinctPredicates( Converter(value).toLong() );
    else {
      cerr << "Error while parsing options, expecting a combinator option i:"
	   << endl
	   << "{timeOut,maxDisjuncts,maxQueries,maxDistinctPredicates}, but" << endl
	   << "\"" << name << "\" found." << endl;

      throw MineruleException(MR_ERROR_OPTION_PARSING,
			      MINERULE_OPTIONS_PARSING_ERROR);
    }
    
  }


  void 
  MineruleOptions::OutStream::setOption(const string& name, 
					const string& value) 
                                 throw(MineruleException) {
    // we need to modify value, hence we made a copy of it
    // and use it in the rest of the procedure
    string valueCopy=value;
    map<string, MRLogger* >& knownStreams =
      getSharedOptions().knownStreams;

    if(name=="stream") {
      size_t markerPos;
      if((markerPos=valueCopy.find("%i"))!=valueCopy.npos) {
	valueCopy.erase(markerPos,2);
	valueCopy.insert(markerPos, options->getMineruleSourceName());
      }

      if((markerPos=valueCopy.find("%m"))!=valueCopy.npos) {
	valueCopy.erase(markerPos,2);
	valueCopy.insert(markerPos, options->getMineruleName());
      }


      if(knownStreams.find(valueCopy)==knownStreams.end()) {
	ostream* ostrptr(new ofstream(valueCopy.c_str()));

	if(!*ostrptr) {
	  cerr << "Error while parsing options, failure while trying to" 
	       << endl
	       << "open in output the following file " << valueCopy << endl
	       << "Possible values for the stream options are:" 
	       << "{<stdin>,<stdout>,anyStringRepresentingAFileName}" << endl
	       << "Note you can also use file names having the modifier %i or %m inside." << endl
	       << "In that case %i is expandended into the current value of the -i parameter" << endl
	       << "(provided that the -i parameter is used instead of the -m one, otherwise, the" << endl
	       << "default 'mr' value is assumed). Analogously, the %m parameter will be" << endl
	       << "substituted with the current minerule name (as it appear in the minerule text" << endl;
	  throw MineruleException(MR_ERROR_OPTION_PARSING,MINERULE_OPTIONS_PARSING_ERROR);
	}

	MRLogger* logger=new MRLogger(*ostrptr);
	knownStreams[valueCopy]=logger;
      }

      setLogger( *knownStreams[valueCopy] );
    } else if(name=="loglevel") {
      try {
	size_t level = Converter(valueCopy).toLong();
	setLogLevel(level);
      } catch (MineruleException& e){
	throw MineruleException(MR_ERROR_OPTION_PARSING,
		  "Error parsing options while converting loglevel value `"
		  +valueCopy+"' to int.");
      }
    } else {
      cerr << "Error while parsing options, expecting a stream option in:" <<endl
	   << "{stream} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING,MINERULE_OPTIONS_PARSING_ERROR);
    }
  }


  void 
  MineruleOptions::Parsers::setOption(const string& name, 
					const string& value) 
                                 throw(MineruleException) {
    if(name=="logfile") {
      if(value=="<stdout>")
	setLogOnStdout();
      else if(value=="<stderr>")
	setLogOnStderr();
      else 
	setLogFILE(value);
    } else if( name=="minBodyElems" ) {
      int min = stringToLong(value,name);
      setMinBodyElems(min);
    } else if( name=="maxBodyElems" ) {
      int max = stringToLong(value,name);
      setMaxBodyElems(max);
    } else if( name=="minHeadElems" ) {
      int min = stringToLong(value,name);
      setMinHeadElems(min);
    } else if( name=="maxHeadElems" ) {
      int max = stringToLong(value,name);
      setMaxHeadElems(max);
    } else {
      cerr << "Error while parsing options, expecting a parser option in:" <<endl
	   << "{logfile, minBodyElems, maxBodyElems, minHeadElems, maxHeadElems} and: " << endl
	   << "\"" << name << "\" found." << endl;
      throw MineruleException(MR_ERROR_OPTION_PARSING, MINERULE_OPTIONS_PARSING_ERROR);
    }
  }
  
  
} // namespace

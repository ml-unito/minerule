#include <cassert>

#include "Utils/MRLogger.h"
#include "Utils/StringUtils.h"

namespace minerule {

  const std::string MRLogger::START_SEPARATOR="+";
  const std::string MRLogger::CONT_SEPARATOR="|";
  const std::string MRLogger::END_SEPARATOR="`--";
  std::ofstream MRLogger::nullLog("/dev/null");


 std::string MRLogger::evalTimeMemInfo(const LogInfo& li) const {
    std::stringstream ss;
    ss<<"[cpu:"
      << getCpuSecs(li,logStack.front())
      <<" d-cpu:"
      << getCpuSecs(li,logStack.back())
      <<" time:"
      << getTimeSecs(li,logStack.front())
      <<" d-time:" 
      << getTimeSecs(li,logStack.back())
      <<"]";
    return ss.str();
  }

  double MRLogger::getCurrentCpuSecs() const {
    LogInfo curInfo("");
    return getCpuSecs(curInfo,logStack.front());
  }

  double MRLogger::getCurrentCpuDelta() const {
    LogInfo curInfo("");
    return getCpuSecs(curInfo,logStack.back());
  }

  double MRLogger::getCurrentTimeSecs() const {
    LogInfo curInfo("");
    return getTimeSecs(curInfo,logStack.front());
  }

  double MRLogger::getCurrentTimeDelta() const {
    LogInfo curInfo("");
    return getTimeSecs(curInfo,logStack.back());
  }

  MRLogger::MRLogger(void) : indentInset("  "), os(NULL), logLevel(100), curLogLevel(0) {
    updateIndentString();
  }

  MRLogger::MRLogger(std::ostream& ostr) : indentInset("  "), os(NULL), logLevel(100), curLogLevel(0) {
    setStream(ostr);
    updateIndentString();
  }

  void MRLogger::setStream(std::ostream& ostr)  {
    os=&ostr;
  };

  MRLogger::~MRLogger(void) {
    while(!logStack.empty()) {
      logStack.pop_back();
    }
  }

  void MRLogger::updateIndentString() {
    LogStack::const_iterator it;
    indentString="";
    for(it=logStack.begin(); it!=logStack.end(); it++) {
      indentString+=it->indent;
    }
  }

  void MRLogger::push(const std::string& descr) {
    curLogLevel++;
    if(curLogLevel>logLevel)
      return; 

    assert(os!=NULL);
    indent();
    *os<<std::endl;

   std::string insetSep;
    if(logStack.empty())
      insetSep="";
    else
      insetSep=indentInset;

    LogInfo li(insetSep+CONT_SEPARATOR);
    logStack.push_back(li);

    indent();
    *os<<insetSep<<"+"<< StringUtils::to_bold(descr) <<std::endl;
    updateIndentString();
  }

 

  void MRLogger::pop() {
    if(logStack.empty()) return;
	
    if(curLogLevel>logLevel) {
      curLogLevel--;
      return;
    } else {
      curLogLevel--;
    }
    assert(os!=NULL);
    LogInfo curInfo("");
    std::string timeMemInfo=evalTimeMemInfo(curInfo);

    logStack.pop_back();
    updateIndentString();
    indent();

    if(!logStack.empty())
      *os<<indentInset<<END_SEPARATOR<< StringUtils::to_bold(timeMemInfo) <<std::endl;
    else
	  *os<<END_SEPARATOR<<StringUtils::to_bold(timeMemInfo)<<std::endl;

    indent();
    *os<<std::endl;
  }
  
  void MRLogger::logMeasurement(const std::string& description, const MeasurementInfo& data) {	  
	  log() << description << StringUtils::to_bold(" cpu time:") << data.totCpu << StringUtils::to_bold(" time:") << data.totTime << std::endl;
  }
  
  void MRLogger::logMeasurements() {
	  push("Showing measured execution times:");
	  for( Measurements::const_iterator it=measurements.begin(); it!=measurements.end(); ++it ) {
		  logMeasurement(it->first, it->second);
	  }
	  pop();
  }


} // minerule

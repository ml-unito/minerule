#include "Utils/MRLogger.h"

using namespace std;

#include <cassert>
namespace minerule {

  const string MRLogger::START_SEPARATOR="+";
  const string MRLogger::CONT_SEPARATOR="|";
  const string MRLogger::END_SEPARATOR="`--";
  ofstream MRLogger::nullLog("/dev/null");


  string MRLogger::evalTimeMemInfo(const LogInfo& li) const {
    stringstream ss;
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

  MRLogger::MRLogger(ostream& ostr) : indentInset("  "), os(NULL), logLevel(100), curLogLevel(0) {
    setStream(ostr);
    updateIndentString();
  }

  void MRLogger::setStream(ostream& ostr)  {
    os=&ostr;
  };

  MRLogger::~MRLogger(void) {
    while(!logStack.empty()) {
      pop();
    }
  }

  void MRLogger::updateIndentString() {
    LogStack::const_iterator it;
    indentString="";
    for(it=logStack.begin(); it!=logStack.end(); it++) {
      indentString+=it->indent;
    }
  }

  void MRLogger::push(const string& descr) {
    curLogLevel++;
    if(curLogLevel>logLevel)
      return; 

    assert(os!=NULL);
    indent();
    *os<<endl;

    string insetSep;
    if(logStack.empty())
      insetSep="";
    else
      insetSep=indentInset;

    LogInfo li(insetSep+CONT_SEPARATOR);
    logStack.push_back(li);

    indent();
    *os<<insetSep<<"+"<<descr<<endl;
    updateIndentString();
  }

 

  void MRLogger::pop() {
    if(curLogLevel>logLevel) {
      curLogLevel--;
      return;
    } else {
      curLogLevel--;
    }
    assert(os!=NULL);
    assert(!logStack.empty());
    LogInfo curInfo("");
    string timeMemInfo=evalTimeMemInfo(curInfo);

    logStack.pop_back();
    updateIndentString();
    indent();

    if(!logStack.empty())
      *os<<indentInset<<END_SEPARATOR<<timeMemInfo<<endl;
    else
      *os<<END_SEPARATOR<<timeMemInfo<<endl;

    indent();
    *os<<endl;
  }


} // minerule

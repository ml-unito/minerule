#include "Utils/MRLogger.h"


namespace minerule {

  const string MRLogger::START_SEPARATOR="+";
  const string MRLogger::CONT_SEPARATOR="|";
  const string MRLogger::END_SEPARATOR="\\--";


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

  MRLogger::MRLogger(ostream& ostr) : indentInset("  ") {
    os=&ostr;
    LogInfo li(CONT_SEPARATOR);
    logStack.push_back(li);
    *os<<START_SEPARATOR<<"MRLogger starting..."<<endl;
    updateIntentString();
  };

  MRLogger::~MRLogger(void) {
    while(!logStack.empty()) {
      pop();
    }
  }

  void MRLogger::updateIntentString() {
    LogStack::const_iterator it;
    indentString="";
    for(it=logStack.begin(); it!=logStack.end(); it++) {
      indentString+=it->indent;
    }
  }

  void MRLogger::push(const string& descr) {
    indent();
    *os<<endl;

    LogInfo li(indentInset+CONT_SEPARATOR);
    logStack.push_back(li);

    indent();
    *os<<indentInset<<"+"<<descr<<endl;
    updateIntentString();
  }

 

  void MRLogger::pop() {
    LogInfo curInfo("");
    string timeMemInfo=evalTimeMemInfo(curInfo);

    logStack.pop_back();
    updateIntentString();
    indent();

    if(!logStack.empty())
      *os<<indentInset<<END_SEPARATOR<<timeMemInfo<<endl;
    else
      *os<<END_SEPARATOR<<"MRLogger terminated:"<<timeMemInfo<<endl;

    indent();
    *os<<endl;
  }


} // minerule

#ifndef __MINERULE_LOGS_H__
#define __MINERULE_LOGS_H__

#include <string>
#include <iostream>

namespace minerule {
  /* LOGGING FACILITIES */
  /* These functions returns the shared streams contained in the
   * object returned by sharedOptions() (see classes below)
   * They are the preferred way of accessing such streams since
   * they provide time and memory logging (they output the informations
   * on the required stream before returning it).
   * The functions below are defined in MineruleOptions.cpp
   */

  std::ostream& MRLog(int id=0);
  std::ostream& MRErr(int id=0);
  std::ostream& MRWarn(int id=0);
  std::ostream& MRDebug(int id=0);
  void MRLogPush(const std::string& descr);
  void MRLogPop();
  void MRErrPush(const std::string& descr);
  void MRErrPop();
  void MRWarnPush(const std::string& descr);
  void MRWarnPop();
  void MRDebugPush(const std::string& descr);
  void MRDebugPop();

  /**
   * This is the safest way to call for MRLogPush & MRLogPop.
   * If this class is used correctly, MRLogPop is automatically
   * called in the destructor (this also ensure exception safety!
   */

  class MRLogPusher {
  public:
    MRLogPusher(const std::string& str) {
      MRLogPush(str);
    }
    
    ~MRLogPusher() {
      MRLogPop();
    }
  };

  /**
   * This is the safest way to call for MRErrPush & MRErrPop.
   * If this class is used correctly, MRErrPop is automatically
   * called in the destructor (this also ensure exception safety!
   */


  class MRErrPusher {
  public:
    MRErrPusher(const std::string& str) {
      MRErrPush(str);
    }
    
    ~MRErrPusher() {
      MRErrPop();
    }
  };

  /**
   * This is the safest way to call for MRWarnPush & MRWarnPop.
   * If this class is used correctly, MRWarnPop is automatically
   * called in the destructor (this also ensure exception safety!
   */
  class MRWarnPusher {
  public:
    MRWarnPusher(const std::string& str) {
      MRWarnPush(str);
    }
    
    ~MRWarnPusher() {
      MRWarnPop();
    }
  };

 
  /**
   * This is the safest way to call for MRDebugPush & MRDebugPop.
   * If this class is used correctly, MRDebugPop is automatically
   * called in the destructor (this also ensure exception safety!
   */
  class MRDebugPusher {
  public:
    MRDebugPusher(const std::string& str) {
      MRDebugPush(str);
    }
    
    ~MRDebugPusher() {
      MRDebugPop();
    }
  };

  int MRLogGetNewID();
}


#endif

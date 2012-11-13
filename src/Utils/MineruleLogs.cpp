#include "Utils/MineruleLogs.h"
#include "Utils/MineruleOptions.h"

namespace minerule {
    std::ostream& MRLog(int id) {
      return MineruleOptions::getSharedOptions().getLogStream();
    }

    std::ostream& MRErr(int id) {
      return MineruleOptions::getSharedOptions().getErrStream();
    }

    std::ostream& MRWarn(int id) {
      return MineruleOptions::getSharedOptions().getWarnStream();
    }

    std::ostream& MRDebug(int id) {
      return MineruleOptions::getSharedOptions().getDebugStream();
    }

    int MRLogGetNewID() {
      static int logId = 4;
      return logId++;
    }

    void MRLogPush(const std::string& descr) {
      MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().push(descr);
    }

    void MRLogPop() {
      MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().pop();
    }

    void MRErrPush(const std::string& descr) {
      MineruleOptions::getSharedOptions().getErrStreamObj().getLogger().push(descr);
    }

    void MRErrPop() {
      MineruleOptions::getSharedOptions().getErrStreamObj().getLogger().pop();
    }

    void MRWarnPush(const std::string& descr) {
      MineruleOptions::getSharedOptions().getWarnStreamObj().getLogger().push(descr);
    }

    void MRWarnPop() {
      MineruleOptions::getSharedOptions().getWarnStreamObj().getLogger().pop();
    }

    void MRDebugPush(const std::string& descr) {
      MineruleOptions::getSharedOptions().getDebugStreamObj().getLogger().push(descr);
    }

    void MRDebugPop() {
      MineruleOptions::getSharedOptions().getDebugStreamObj().getLogger().pop();
    }
}
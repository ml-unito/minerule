#include "Utils/MineruleLogs.h"
#include "Utils/MineruleOptions.h"

namespace minerule {
    std::ostream& MRLog() {
      return MineruleOptions::getSharedOptions().getLogStream();
    }

    std::ostream& MRErr() {
      return MineruleOptions::getSharedOptions().getErrStream();
    }

    std::ostream& MRWarn() {
      return MineruleOptions::getSharedOptions().getWarnStream();
    }

    std::ostream& MRDebug() {
      return MineruleOptions::getSharedOptions().getDebugStream();
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
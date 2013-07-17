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
#include "minerule/Utils/MineruleLogs.h"
#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Utils/StringUtils.h"

const size_t MAX_LOG_LENGTH = 70;

namespace minerule {
	
	void logRespectingMaxLogLength(std::ostream& (*logger)(void), size_t indentLen, const std::string& msg ) {
		
		std::vector<std::string>* chunks = StringUtils::splitToLength(msg, MAX_LOG_LENGTH-indentLen);
		
		std::vector<std::string>::const_iterator it= chunks->begin();
		if( it != chunks->end() ) {
			logger() << *it << std::endl;						
		}
		
		++it;
		
		for( ; it!=chunks->end(); ++it ) {
			logger() << StringUtils::toGreen("  +") << *it << std::endl;			
		}
			
		delete chunks;		
	}
	
	
    std::ostream& MRLog() {
      return MineruleOptions::getSharedOptions().getLogStream();
    }
		
	void MRLog(const std::string& msg) {
		size_t indentLength = MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().getIndentLen();
		logRespectingMaxLogLength(MRLog, indentLength ,msg);
	}

    std::ostream& MRErr() {
      return MineruleOptions::getSharedOptions().getErrStream();
    }

	void MRErr(const std::string& msg) {
		size_t indentLength = MineruleOptions::getSharedOptions().getErrStreamObj().getLogger().getIndentLen();
		logRespectingMaxLogLength(MRErr, indentLength, msg);
	}


    std::ostream& MRWarn() {
      return MineruleOptions::getSharedOptions().getWarnStream();
    }

	void MRWarn(const std::string& msg) {
  	    size_t indentLength = MineruleOptions::getSharedOptions().getWarnStreamObj().getLogger().getIndentLen();		
		logRespectingMaxLogLength(MRWarn, indentLength, msg);
	}


    std::ostream& MRDebug() {
      return MineruleOptions::getSharedOptions().getDebugStream();
    }
	
	void MRDebug(const std::string& msg) {
    	size_t indentLength = MineruleOptions::getSharedOptions().getDebugStreamObj().getLogger().getIndentLen();		
		logRespectingMaxLogLength(MRDebug, indentLength, msg);
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
	
	
    void MRLogStartMeasuring(const std::string& description) {
    	MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().startMeasuring(description);
    }
    void MRLogStopMeasuring(const std::string& description) {
     	MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().stopMeasuring(description);  	
    }
	
	void MRLogShowMeasurements() {
		MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().logMeasurements();
	}
}
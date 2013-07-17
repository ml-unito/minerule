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
   */

  std::ostream& MRLog();
  std::ostream& MRErr();
  std::ostream& MRWarn();
  std::ostream& MRDebug();
  
  // The following methods print msg on the log and ensure that
  // the whole thing stays in 80 characters.
  void MRLog(const std::string& msg);
  void MRErr(const std::string& msg);
  void MRWarn(const std::string& msg);
  void MRDebug(const std::string& msg);
  
  void MRLogPush(const std::string& descr);
  void MRLogPop();
  void MRErrPush(const std::string& descr);
  void MRErrPop();
  void MRWarnPush(const std::string& descr);
  void MRWarnPop();
  void MRDebugPush(const std::string& descr);
  void MRDebugPop();
  
  
  void MRLogStartMeasuring(const std::string& description);
  void MRLogStopMeasuring(const std::string& description);
  void MRLogShowMeasurements();

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

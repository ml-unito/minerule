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

#include <dlfcn.h>


class MineruleOptions : public OptionBase {
public:
	static const std::string DEFAULT_FILE_NAME;

  /* ==============================
   * Inner classes
   * ============================== */

	/* ========== MRDB_DB ========== */

	#include "minerule/Utils/MineruleOptions_implementations/mrdb.hpp"
  /* ========== SAFETY ========== */

	#include "minerule/Utils/MineruleOptions_implementations/safety.hpp"
  /* ========== MININGALGORITHMS ========== */


  	#include "minerule/Utils/MineruleOptions_implementations/miningalgorithms.hpp"

  /* ========== OPTIMIZATIONS ========== */
  	#include "minerule/Utils/MineruleOptions_implementations/optimizations.hpp"


  /* ========== PARSERS ========== */

  	#include "minerule/Utils/MineruleOptions_implementations/parsers.hpp"

  /* ========== OUTSTREAM ========== */

  	#include "minerule/Utils/MineruleOptions_implementations/outstream.hpp"


  /* ==============================
   * Private members
   * ============================== */

private:
  static MineruleOptions sharedOptions;
  Mrdb mrdb_db;
  Safety safety;
  Optimizations optimizations;
  MiningAlgorithms miningAlgorithms;
  OutStream logStream;
  OutStream errStream;
  OutStream warnStream;
  OutStream debugStream;
  Parsers parsers;
  std::map<std::string,std::string> userOptions;
  std::string mineruleSourceName; // file name of the file which contained the
                             // minerule text
  std::string mineruleName; // name of the minerule as it appear in the
                       // minerule statement

  // the following mapping is used to allow streams to share a common
  // std::ostream. If, for instance, the user says that both log and err
  // stream should be redirected onto file "foo", then we will create
  // an std::ostream on "foo" once and use for both the streams. Obviously
  // this means that we need to keep track of already "known" streams.
  std::map<std::string, MRLogger* > knownStreams;


  // This boolean is set when an option object
  // is ready to provide the informations for
  // which it was created
  bool ready;

  bool isReady() {
    return ready;
  }

  /**
   * This method is called by getSharedOptions() in
   * order to initialize the sharedOptions object
   */
  void init();

  static long stringToLong(const std::string& value, const std::string& name)
    {
    Converter c(value);
    long result;
    try {
	result = c.toLong();
    } catch(MineruleException&) {
	throw MineruleException(MR_ERROR_OPTION_PARSING,
				"Error while parsing option '"+name+"':"
				+ " an integer was expected, but " + value
				+ " found");
    }

    return result;
  }

protected:
  virtual ~MineruleOptions();
   // till now there is no need to declare any
  // new MineruleOptions, nor to create copy of
  // them...
  MineruleOptions() :
    logStream(this,"logstream"),
    errStream(this,"errstream"),
    warnStream(this,"warnstream"),
    debugStream(this,"debugstream") {  }

  MineruleOptions(const MineruleOptions& rhs) :
    mrdb_db(rhs.mrdb_db),
    miningAlgorithms(rhs.miningAlgorithms),
    logStream(this,"logstream"),
    errStream(this,"errstream"),
    warnStream(this,"warnstream"),
    debugStream(this,"debugstream")  {  }


public:
  /* ==============================
   * Public Members
   * ============================== */

  virtual std::string className() const {
    return "root";
  }

  virtual void setOption(const std::string& name, const std::string& value)
    {
    throw MineruleException(MR_ERROR_OPTION_PARSING,
			      "Attempting to set an option in the root class");
  }

  virtual OptionBase& subclassForName(const std::string& oclass)
    ;

  static
    MineruleOptions& getSharedOptions() {
    if( !sharedOptions.isReady() )
	sharedOptions.init();

    return sharedOptions;
  }

  void
    readFromFile(std::string filename) ;

  void
    readFromString(const std::string&) ;


  // set the name of the file from which the current
  // minerule was read
  void setMineruleSourceName(const std::string& name) {
    mineruleSourceName=name;
  }

  // see set method
  const std::string&
    getMineruleSourceName() const {
    return mineruleSourceName;
  }

  // set the name of the minerule as it appears in
  // the minerule statement
  void setMineruleName(const std::string& name) {
    mineruleName = name;
  }

  const std::map< std::string, MRLogger*>&
    getKnownStreams() const {
    return knownStreams;
  }

  // see set method
  const std::string&
    getMineruleName() const {
    return mineruleName;
  }

  Mrdb&
    getMRDB() {
    return mrdb_db;
  }

  const Mrdb&
    getMRDB() const {
    return mrdb_db;
  }

  const Safety&
  getSafety() const {
    return safety;
  }

  Safety&
  getSafety() {
    return safety;
  }

  Optimizations&
    getOptimizations()  {
    return optimizations;
  }

  const Optimizations&
    getOptimizations() const {
    return optimizations;
  }

  MiningAlgorithms&
    getMiningAlgorithms()  {
    return miningAlgorithms;
  }

  const MiningAlgorithms&
    getMiningAlgorithms() const {
    return miningAlgorithms;
  }

  std::ostream&
    getLogStream() const {
		return logStream.getStream() << StringUtils::toGreen("Log:");
  }

  std::ostream&
    getErrStream() const {
    return errStream.getStream() << StringUtils::toRed("ERROR:");
  }

  std::ostream&
    getWarnStream() const {
    return warnStream.getStream() << StringUtils::toYellow("WARNING:");
  }

  std::ostream&
    getDebugStream() const {
    return debugStream.getStream() << StringUtils::toOrange("DEBUG:");
  }

  OutStream&
    getLogStreamObj()  {
    return logStream;
  }

  const OutStream&
    getLogStreamObj() const {
    return logStream;
  }

  OutStream&
    getErrStreamObj()  {
    return errStream;
  }

  const OutStream&
    getErrStreamObj() const {
    return errStream;
  }

  OutStream&
    getWarnStreamObj()  {
    return warnStream;
  }

  const OutStream&
    getWarnStreamObj() const {
    return warnStream;
  }

  OutStream&
    getDebugStreamObj()  {
    return debugStream;
  }

  const OutStream&
    getDebugStreamObj() const {
    return debugStream;
  }

  Parsers&
    getParsers() {
    return parsers;
  }

  const Parsers&
    getParsers() const {
    return parsers;
  }

  std::ostream& saveOptions(std::ostream& os) const;

  // this could be used to store particular user options
  std::map<std::string,std::string>& getUserOptions() {
    return userOptions;
  }
};

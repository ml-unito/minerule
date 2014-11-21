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
class OutStream : public OptionBase {
private:
  MRLogger* logger;
  size_t logLevel;
  // reference to the enclosing option object
  MineruleOptions* options; 
  std::string streamName;
public:
  OutStream(MineruleOptions* opt, const std::string& name) 
	: logger(NULL), options(opt), streamName(name) {};
  virtual ~OutStream() {};

  virtual std::string className() const {
	return streamName;
  }

  virtual void setOption(const std::string& name,const std::string& value) 
	throw(MineruleException);

  void setLogger(MRLogger& log) {
	logger=&log;
	logLevel = log.getLogLevel();
  }

  MRLogger& getLogger() {
	return *logger;
  }
  std::ostream& getStream() const {
	assert(logger!=NULL);
	return logger->log();
  }

  void setLogLevel(size_t level) {
	assert(logger!=NULL);
	logger->setLogLevel(level);
	logLevel=level;
  }

  void disable() {
	logger->setLogLevel(0);
  }

  void enable() {
	logger->setLogLevel(logLevel);
  }
};

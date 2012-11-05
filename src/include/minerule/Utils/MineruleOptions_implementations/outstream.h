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
  }

  MRLogger& getLogger() {
	return *logger;
  }
  ostream& getStream() const {
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

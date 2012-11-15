class Odbc_db : public OptionBase {
  std::string name;
  std::string username;
  std::string password;
  odbc::Connection* connection;
  bool cacheWrites;
  std::string dbms;
public:
  Odbc_db() : connection(NULL), cacheWrites(false), dbms("mysql") {};

  virtual ~Odbc_db()  { }

  virtual std::string className() const {
	return "odbc";
  }

  virtual void setOption(const std::string& name,const std::string& value) 
	throw(MineruleException);

  std::string getName() const {
	return name;
  }
  
  std::string getUsername() const {
	return username;
  }
  std::string getPassword() const {
	return password;
  }
  
  std::string getDBMS() const {
	  return dbms;
  }
  

  bool getCacheWrites() const {
	return cacheWrites;
  }

  // at least for the sharedOptions, this
  // object is set by init()
  odbc::Connection* getODBCConnection() const throw(MineruleException) {
	if( connection==NULL ) {
	  throw MineruleException(MR_ERROR_DATABASE_ERROR,
				  (std::string) 
				  "Check out the parameters, it looks like that"
				  " you did not specified any odbc data source");
	}

	return connection;
  }

  void setName(const std::string& str) {
	name = str;
  }

  void setUsername(const std::string& str) {
	username = str;
  }

  void setPassword(const std::string& str) {
	password = str;
  }

  void setCacheWrites(bool v) {
	cacheWrites = v;
  }
  
  void setDBMS(std::string v) {
	  dbms = v;
  }

  void setConnection(odbc::Connection* con) {
	connection = con;
  }

  void clearConnection() {
	if( connection!=NULL ) {
	  delete connection;
	  connection=NULL;
	}
  }

  // if necessary, free the current connection (using delete) and
  // create a new one using the currente parameters
  void resetConnection() throw(odbc::SQLException) {
	clearConnection();

	odbc::Connection* con = 
	  odbc::DriverManager::getConnection(getName(),
					     getUsername(),
					     getPassword());
	setConnection(con);
  }
};

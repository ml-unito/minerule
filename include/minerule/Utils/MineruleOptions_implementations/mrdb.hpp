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

class Mrdb : public OptionBase {
  std::string name;
  std::string username;
  std::string password;
  mrdb::Connection *connection;
  bool cacheWrites;
  std::string dbms;

public:
  Mrdb() : connection(NULL), cacheWrites(false), dbms("postgres"){};

  virtual ~Mrdb() {}

  virtual std::string className() const { return "mrdb"; }

  virtual void setOption(const std::string &name,
                         const std::string &value) ;

  std::string getName() const { return name; }

  std::string getUsername() const { return username; }
  std::string getPassword() const { return password; }

  std::string getDBMS() const { return dbms; }

  bool getCacheWrites() const { return cacheWrites; }

  // at least for the sharedOptions, this
  // object is set by init()
  mrdb::Connection *getMRDBConnection() const {
    if (connection == NULL) {
      throw MineruleException(
          MR_ERROR_DATABASE_ERROR,
          (std::string) "Check out the parameters, it looks like that"
                        " you did not specified any mrdb data source");
    }

    return connection;
  }

  void setName(const std::string &str) { name = str; }

  void setUsername(const std::string &str) { username = str; }

  void setPassword(const std::string &str) { password = str; }

  void setCacheWrites(bool v) { cacheWrites = v; }

  void setDBMS(std::string v) { dbms = v; }

  void setConnection(mrdb::Connection *con) { connection = con; }

  void clearConnection() {
    if (connection != NULL) {
      delete connection;
      connection = NULL;
    }
  }

  std::string getLibName(const std::string& dbms)  {
    if(dbms=="postgres") {
      return "libmrdb_pg.so";
    } else {
      throw MineruleException(MR_ERROR_DATABASE_ERROR,
      "Database " + dbms + " not supported yet.");
    }
  }

  // if necessary, free the current connection (using delete) and
  // create a new one using the currente parameters
  void resetConnection() {
    clearConnection();

    std::string libName(getLibName(getDBMS()));
    static void *handle = dlopen( libName.c_str(), RTLD_LAZY);

    if (handle == NULL) {
      throw MineruleException(MR_ERROR_DATABASE_ERROR, dlerror());
    }

    MRDBConnectFun connect = (MRDBConnectFun)dlsym(handle, "connect");

    if (connect == NULL) {
      throw MineruleException(MR_ERROR_DATABASE_ERROR, dlerror());
    }

    setConnection(connect(getName().c_str(), getUsername().c_str(),
                          getPassword().c_str()));
  }
};

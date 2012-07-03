#ifndef __MRSTATEMENT_H__
#define __MRSTATEMENT_H__

#include "odbc++/statement.h"
#include "odbc++/resultset.h"
#include "odbc++/preparedstatement.h"
#include <iostream>

using namespace std;
using namespace odbc;

class MRResultSet : public odbc::ResultSet {
  string str;
 public:
  MRResultSet(const string& s) : ResultSet(NULL, 0, false), str(s) {
    cout << "MRResultSet created!" << endl;
  }

  virtual bool next() {
    cout << "MRResultSet next" << endl;
    return false;
  }
  

};



class MRPreparedStatement : public odbc::PreparedStatement {
  string str;
 public:
  MRPreparedStatement(std::string s) : 
    PreparedStatement(NULL,0,"",0,0,0), str(s) {
  }

  virtual bool execute(const std::string& sql) {
    return false;
  }

  virtual ResultSet* executeQuery(const std::string &sql) {
    return new MRResultSet(str);
  }
};




#endif

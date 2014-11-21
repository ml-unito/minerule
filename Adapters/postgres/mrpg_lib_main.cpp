#include "Connection.hpp"
#include <iostream>

extern "C" {
void *connect(const char *db, const char *username, const char *password) {
  return new mrdb::postgres::Connection(db, username,password);
}
}

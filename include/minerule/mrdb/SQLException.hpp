#ifndef MRDB_SQLEXCEPTION_H_MNG3MX83__
#define MRDB_SQLEXCEPTION_H_MNG3MX83__

#include <stdexcept>

namespace mrdb {

class SQLException : public std::runtime_error {
public:
  SQLException(const std::string &message) : std::runtime_error(message){};
  virtual ~SQLException() throw() {}
};

}

#endif /* end of include guard: MRDB_SQLEXCEPTION_H_MNG3MX83__ */

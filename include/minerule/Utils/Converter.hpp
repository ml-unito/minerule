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
#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

#include <sstream>
#include <string>
#include <cerrno>
#include <string.h>
#include <stdlib.h>
#include "minerule/Utils/MineruleException.hpp"

namespace minerule {

class Converter {
  typedef enum { NO_FLAG = 0, ALLOW_QUOTES_IN_NUMBERS = 1 } ConversionFlags;
  std::string value;

public:
  explicit Converter(const char *val) : value(val){};
  explicit Converter(std::string val) : value(val){};
  explicit Converter(double val) {
    std::stringstream ss;
    ss << val; // << std::ends;
    value = ss.str();
  }

  explicit Converter(int val) {
    std::stringstream ss;
    ss << val;
    value = ss.str();
  }

  explicit Converter(long val) {
    std::stringstream ss;
    ss << val;
    value = ss.str();
  }

  explicit Converter(unsigned int val) {
    std::stringstream ss;
    ss << val; // << std::ends;
    value = ss.str();
  }

  explicit Converter(unsigned long val) {
    std::stringstream ss;
    ss << val; // << std::ends;
    value = ss.str();
  }

  explicit Converter(bool val) {
    if (val)
      value = "True";
    else
      value = "False";
  }

  std::string toString() const { return value; }

  double toDouble(unsigned int flags = ALLOW_QUOTES_IN_NUMBERS) const
      {
    char *endPtr = NULL;
    double result;
    const char *startChar;
    char endChar;
    errno = 0;

    if ((flags & ALLOW_QUOTES_IN_NUMBERS) && value.length() > 1 &&
        value[0] == '\'' && value[value.length() - 1] == '\'') {
      startChar = &value.c_str()[1];
      endChar = '\'';
    } else {
      startChar = value.c_str();
      endChar = '\0';
    }

    result = strtod(startChar, &endPtr);
    if (*endPtr != endChar || errno == ERANGE || errno == EINVAL)
      throw MineruleException(MR_ERROR_INTERNAL,
                              "Convertion error while converting string:" +
                                  value + " to a Double value");
    return result;
  }

  long toLong(unsigned int flags = ALLOW_QUOTES_IN_NUMBERS) const {
    char *endPtr = NULL;
    errno = 0;
    long result;
    const char *startChar;
    char endChar;

    if ((flags & ALLOW_QUOTES_IN_NUMBERS) && value.length() > 1 &&
        value[0] == '\'' && value[value.length() - 1] == '\'') {
      startChar = &value.c_str()[1];
      endChar = '\'';
    } else {
      startChar = value.c_str();
      endChar = '\0';
    }

    result = strtol(startChar, &endPtr, 10);
    if (*endPtr != endChar || errno == ERANGE || errno == EINVAL) {
      std::stringstream ss;
      ss << "Convertion error while converting string:" << value
         << " to a long value. lenght:" << value.length()
         << " strlen:" << strlen(value.c_str());
      if (errno != 0)
        ss << " errno string:" << strerror(errno);

      throw MineruleException(MR_ERROR_INTERNAL, ss.str());
    }

    return result;
  }

  bool toBool() const {
    if (value == "True")
      return true;
    else if (value == "False")
      return false;
    else if (isNumber()) {
      return toDouble() > 0;
    } else
      throw MineruleException(MR_ERROR_INTERNAL,
                              "Converting error while converting string:"
                              "'" +
                                  value + "' to a bool value");
  }

  bool isNumber() const {
    try {
      toDouble();
    } catch (MineruleException &e) {
      return false;
    }

    return true;
  }
};
}

#endif

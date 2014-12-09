#ifndef MRDB_TYPES_HPP_JACKJHCLKAEJCE__
#define MRDB_TYPES_HPP_JACKJHCLKAEJCE__

// The following list of type definition is the same as the one defined by
// libmrdb++. The actual values are taken from unixmrdb header files.

namespace mrdb {
struct Types {
  enum SQLType {
    BIGINT = -5,
    BINARY = -2,
    BIT = -7,
    CHAR = 1,
    DATE = 9,
    DECIMAL = 3,
    DOUBLE = 8,
    FLOAT = 6,
    INTEGER = 4,
    LONGVARBINARY = -4,
    LONGVARCHAR = -1,
    NUMERIC = 2,
    REAL = 7,
    SMALLINT = 5,
    TIME = 10,
    TIMESTAMP = 11,
    TINYINT = -6,
    VARBINARY = -3,
    VARCHAR = 12
  };
};
}

#endif /* end of include guard: MRDB_TYPES_HPP_JACKJHCLKAEJCE__ */

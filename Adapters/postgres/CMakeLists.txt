PROJECT (postgres_adapter)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

MESSAGE(${CMAKE_MODULE_PATH})

FIND_PACKAGE (PostgreSQL REQUIRED)
FIND_PACKAGE (Minerule REQUIRED)

INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS}/server)
INCLUDE_DIRECTORIES(${Minerule_INCLUDE_DIRS})

FILE(GLOB mrdb_pg_src *.cpp)
ADD_LIBRARY(mrdb_pg MODULE ${mrdb_pg_src})
INCLUDE_DIRECTORIES("$(PROJECT_SOURCE_DIR)")

FILE(GLOB TestStatements_Src tests/TestStatements.cpp)
ADD_EXECUTABLE(TestStatements ${TestStatements_Src})

FILE(GLOB TestResultSet_Src tests/TestResultSet.cpp)
ADD_EXECUTABLE(TestResultSet ${TestResultSet_Src})

FILE(GLOB TestDatabaseMetaData_Src tests/TestDatabaseMetaData.cpp)
ADD_EXECUTABLE(TestDatabaseMetaData ${TestDatabaseMetaData_Src})

FILE(GLOB TestResultSetMetaData_Src tests/TestResultSetMetaData.cpp)
ADD_EXECUTABLE(TestResultSetMetaData ${TestResultSetMetaData_Src})

TARGET_LINK_LIBRARIES (mrdb_pg ${PostgreSQL_LIBRARIES})
TARGET_LINK_LIBRARIES (TestStatements ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES} ${CMAKE_DL_LIBS})
TARGET_LINK_LIBRARIES (TestResultSet ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES} ${CMAKE_DL_LIBS})
TARGET_LINK_LIBRARIES (TestDatabaseMetaData ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES} ${CMAKE_DL_LIBS})
TARGET_LINK_LIBRARIES (TestResultSetMetaData ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES} ${CMAKE_DL_LIBS})

ENABLE_TESTING()

ADD_TEST(NAME TestStatements COMMAND TestStatements )
ADD_TEST(NAME TestDatabaseMetaData COMMAND TestDatabaseMetaData )
ADD_TEST(NAME TestResultSetMetaData COMMAND TestResultSetMetaData )
ADD_TEST(NAME TestResultSet COMMAND TestResultSet )

INSTALL(TARGETS mrdb_pg
        LIBRARY DESTINATION lib)

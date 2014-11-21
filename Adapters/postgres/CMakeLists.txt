PROJECT (postgres_adapter)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

MESSAGE(${CMAKE_MODULE_PATH})

FIND_PACKAGE (PostgresQL REQUIRED)
FIND_PACKAGE (Minerule REQUIRED)

INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS}/server)
INCLUDE_DIRECTORIES(${Minerule_INCLUDE_DIRS})

FILE(GLOB mrdb_pg_src *.cpp)
ADD_LIBRARY(mrdb_pg MODULE ${mrdb_pg_src})
INCLUDE_DIRECTORIES("$(PROJECT_SOURCE_DIR)")

FILE(GLOB TestStatements_Src tests/TestStatements.cpp)
ADD_EXECUTABLE(TestStatements ${TestStatements_Src})

FILE(GLOB TestDatabaseMetaData_Src tests/TestDatabaseMetaData.cpp)
ADD_EXECUTABLE(TestDatabaseMetaData ${TestDatabaseMetaData_Src})

FILE(GLOB TestResultSetMetaData_Src tests/TestResultSetMetaData.cpp)
ADD_EXECUTABLE(TestResultSetMetaData ${TestResultSetMetaData_Src})

TARGET_LINK_LIBRARIES (mrdb_pg ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES})
TARGET_LINK_LIBRARIES (TestStatements ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES})
TARGET_LINK_LIBRARIES (TestDatabaseMetaData ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES})
TARGET_LINK_LIBRARIES (TestResultSetMetaData ${PostgreSQL_LIBRARIES} ${Minerule_LIBRARIES})

ENABLE_TESTING()

ADD_TEST(NAME TestStatements COMMAND TestStatements )
ADD_TEST(NAME TestDatabaseMetaData COMMAND TestDatabaseMetaData )
ADD_TEST(NAME TestResultSetMetaData COMMAND TestResultSetMetaData )

INSTALL(TARGETS mrdb_pg
        LIBRARY DESTINATION lib)

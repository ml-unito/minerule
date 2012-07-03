#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>

#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Utils/MineruleOptions.h"
#include "Optimizer/OptimizerCatalogue.h"

using namespace std;
using namespace minerule;

const unsigned int NOTHING_TO_DO = 0;
const unsigned int ERROR_CANNOT_OPEN_OPTION_FILE = 1;
const unsigned int QUERY_NAME_FOUND = 2;
const unsigned int QUERY_NAME_NOT_FOUND = 3;
const unsigned int OPTION_PARSING_ERROR = 4;


bool
fileExists(const string& filename) {
  struct stat st;
  if( stat(filename.c_str(),&st)==0 && S_ISREG(st.st_mode) ) {
    return true;
  } else {
    return false;
  }
}

void
printHelp(int argc, char** argv) {
  cout << "Usage:" << endl
       << "   " << argv[0] << " [-h] [-f <optionfile>] [-n <queryname>] resultsetname"
       << endl
       << "The program allow to inspect the MR catalogue" << endl
       << "-h - output this message and returns NOTHING_TO_DO" << endl
       << "-n - Look in the catalogue for the specified query" << endl
       << "     it returns QUERY_NAME_FOUND, or QUERY_NAME_NOT_FOUND" << endl
       << "     accordingly to whether the query could be found."<<endl 
       << "Return values:" << endl
       << "      NOTHING_TO_DO = 0" << endl
       << "      ERROR_CANNOT_OPEN_OPTION_FILE = 1" << endl
       << "      QUERY_NAME_FOUND = 2" << endl
       << "      QUERY_NAME_NOT_FOUND = 3" << endl
       << "      OPTION_PARSING_ERROR = 4" << endl;
}

void
loadMROptions(int argc, char** argv, MineruleOptions& opt) {
  int j=argc;
  for(int i=0; i<argc; i++) {
    if(string(argv[i])=="-f")
      j=i+1;
  }

  if(j<argc && fileExists(argv[j])) {
    opt.readFromFile(optarg);
  } else {
    string errstr;
    if(errno==0) {
      errstr = "Not a regular file!";
    } else {
      errstr = strerror(errno);
    }
	
    // file argv[i+1] does not exists
    cout << "Could not open file:" << argv[i+1] <<endl
	 << "The reason is:" << errstr << endl;
    exit(ERROR_CANNOT_OPEN_OPTION_FILE);
  }
}

bool checkQueryName(const string& tablename) {
  try {
    resultsetname = OptimizerCatalogue::getResultsetName(tablename);
  } catch (MineruleException& mr) {
    return false;
  }

  return true;
}

void
parseCommands(int argc, char** argv) {
  int ch;
  const char* optstr = "f:n:";
  while( (ch=getopt(argc,argc,optstr))!=-1 ) {
    switch(ch) {
    case 'f':
      // this specifies the mroption file, we already
      // used this option.
      break;
    case 'h':
      printHelp();
      return NOTHING_TO_DO;
    case 'n':
      // this query amount to ask if the queryName exists
      if(checkQueryName(optarg)) {
	cout << "The query you specified is present in the catalogue.";
	return QUERY_NAME_FOUND;
      } else {
	cout << "The query you specified is NOT present in the catalogue.";
	return QUERY_NAME_NOT_FOUND;
      }
      break;
    }
  default:
    cout << "Option Parsing Error" << endl;
    return OPTION_PARSING_ERROR;
  }
}

int
main(int argc, char** argv) {
  
  MineruleOptions& mr = MineruleOptions::getSharedOptions();
  loadMROptions(argc, argv, mr);

  return parseCommands(argc, argv, mr);
}

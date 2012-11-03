#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>

#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include "Utils/MineruleOptions.h"
#include "Optimizer/OptimizerCatalogue.h"

using namespace std;
using namespace minerule;

typedef enum {
  SUCCESS=0,
  NOTHING_TO_DO,
  QUERY_NAME_FOUND,
  QUERY_NAME_NOT_FOUND,
  ERROR_OPTION_PARSING,
  ERROR_CANNOT_OPEN_OPTION_FILE,
  ERROR_EXCEPTION_THROWN
} MRCResults;

class MRCException : public exception {
  MRCResults result;
  string errmsg;
public:
  MRCException(MRCResults res,
	       const string& msg) :  result(res),errmsg(msg) { }

  virtual ~MRCException() throw() {};

  virtual const char* what() const throw () {
    return errmsg.c_str();
  }

  virtual MRCResults getResultID() const {
    return result;
  }
};


class ProgramOptions {
public:
  typedef enum {
    NoCommand,
    ShowList,
    SearchQry,
    DeleteQry
  } Command;

  class ListFormat {
  public:
    bool size;
    bool text;
    bool result;

    ListFormat() : size(false),
		   text(false),
		   result(false) {}
  };

  typedef enum {
    QryName=0,
    LastParam
  } QryParams;

protected:
  Command command;

  string searchParam[LastParam];
  ListFormat listFormat;
  string sepString; // the string that should be used to separate output elements
public:

  ProgramOptions(): 
    command(NoCommand), sepString(" ")  {  }

  void setListFormat(const char* format) 
     throw (MRCException) {
    if(format==NULL)
      return;

    for(int i=0; format[i]!='\0'; i++) {
      switch( format[i] ) {
      case 's':
	listFormat.size=true;
	break;
      case 't':
	listFormat.text = true;
	break;
      case 'r':
	listFormat.result = true;
	break;
      default:
	throw MRCException(ERROR_OPTION_PARSING,
			   string("Unknown flag:")+format[i]);
      }
    }
  }

  void setShowList() {
    if(command!=NoCommand && command!=ShowList) {
      throw MRCException( ERROR_OPTION_PARSING,
	     "You can specify either -l or -n option, but not both!" );
    }

    command = ShowList;
  }

  void setSearchQry() {
    if(command!=NoCommand && command!=SearchQry) {
      throw MRCException( ERROR_OPTION_PARSING,
	     "You can specify either -l or -n option, but not both!" );
    }

    command = SearchQry;
  }

  void setDeleteQry() {
    if(command!=NoCommand && command!=DeleteQry) {
      throw MRCException( ERROR_OPTION_PARSING,
	  "Too many different options have been given, check out the parameters");
    }

    command = DeleteQry;
  }

  void setSearchParam(QryParams qryParam, const string& param) {
    searchParam[qryParam]=param;
  }

  const string& getSearchParam(QryParams qryParam) const {
    return searchParam[qryParam];
  }

  void setSepString(const string& s) {
    sepString=s;
  }

  const string& getSepString() const {
    return sepString;
  }

  Command getCommand() const {
    return command;
  }

  const ListFormat& getListFormat() const {
    return listFormat;
  }
};





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
       << "   " << argv[0] << " [-h] [-s <sepstring>] [-f <optionfile>] " << endl
       << "           [-n <queryname>] [-l] [-F formatSpecs] [-d <queryname>]"
       << endl
       << "The program allow to inspect the MR catalogue" << endl
       << "-h - Output this message and returns NOTHING_TO_DO" << endl
       << "-s - Change the string used to separate different elements in" << endl
       << "     the output. The default is ' '" << endl
       << "-n - Look in the catalogue for the specified query" << endl
       << "     it returns QUERY_NAME_FOUND, or QUERY_NAME_NOT_FOUND" << endl
       << "     accordingly to whether the query could be found."<<endl 
       << "-l - Print the list of already executed queries, returns" << endl
       << "     SUCCESS upon completion" << endl
       << "-d - Delete the given minerule from the system (notice that" << endl
       << "     the safety options in the option file must be setted" << endl
       << "     in such a way to allow the deletion. " << endl
       << "-F - Format specifiers for printing the list of queries (-l)" <<endl
       << "     Valid specifiers: s - Print the size of the result set" << endl
       << "                       t - Print the text of the original mr" <<endl
       << "                       r - Print the name of the result set" << endl
       << "     the default is '', i.e.: print onlty the qry name" << endl
       << "Return values:" << endl
       << "      SUCCESS = " << SUCCESS << endl
       << "      NOTHING_TO_DO = " << NOTHING_TO_DO << endl
       << "      QUERY_NAME_FOUND = " << QUERY_NAME_FOUND << endl
       << "      QUERY_NAME_NOT_FOUND = " << QUERY_NAME_NOT_FOUND << endl
       << "      ERROR_OPTION_PARSING = " << ERROR_OPTION_PARSING << endl
       << "      ERROR_CANNOT_OPEN_OPTION_FILE = " << ERROR_CANNOT_OPEN_OPTION_FILE <<endl;
}

void 
doLoadOptions(const char* fname, MineruleOptions& opt) {
  
  if( fileExists(fname) ) {
    opt.readFromFile(fname);
  } else {
    string errstr;
    if(errno==0) {
      errstr = "Not a regular file!";
    } else {
      errstr = strerror(errno);
    }
    
    throw MRCException(ERROR_CANNOT_OPEN_OPTION_FILE,
		       string("Could not open file:") + fname +
		       ". The reason is:" + errstr);
  }
}

void printVersion() {
  cout << "MRCatalogue v:" << MR_VERSION << endl;
}

void
parseOptions(int argc, char** argv, 
	      MineruleOptions& mopt,
	      ProgramOptions& popt) {
  const char* optstr = "hf:lF:n:d:s:v";
  int opt;
  bool didLoadOptions=false;
  while((opt=getopt(argc, argv, optstr))!=-1) {
    switch( opt ) {
    case 'f': 
      doLoadOptions(optarg, mopt);
      didLoadOptions=true;
      break;
    case 'h':
      printHelp(argc, argv);
      break;
    case 'F':
      popt.setListFormat(optarg);
      break;
    case 'l':
      popt.setShowList();
      break;
    case 'n':
      popt.setSearchQry();
      popt.setSearchParam(ProgramOptions::QryName, optarg);
      break;
    case 'd':
      popt.setDeleteQry();
      popt.setSearchParam(ProgramOptions::QryName, optarg);
      break;
    case 's':
      popt.setSepString(optarg);
      break;
    case 'v':
      printVersion();
      exit(0);
    case '?':
    default:
      throw MRCException( ERROR_OPTION_PARSING,
			  "Error while parsing options, use -h to show"
			  " the help message" );
    }
  }
  if( !didLoadOptions ) {
    throw MRCException( ERROR_OPTION_PARSING,
			"You should specify at least one -f option");
  }
}

void printQueryInfo(ostream& out, 
		    const CatalogueInfo& info,
		    const ProgramOptions& options) {
  out << info.qryName;
  if(options.getListFormat().size) {
    out << options.getSepString() << info.resSize;
  }
  
  if(options.getListFormat().result) {
    out << options.getSepString() << info.resName;
  }
  
  if(options.getListFormat().text) {
    out << options.getSepString() << info.qryText;
  }
  
  out << endl;
}


int checkQueryName(const ProgramOptions& options) {
  const string& tablename = 
    options.getSearchParam(ProgramOptions::QryName);

  try {
    CatalogueInfo info;
    OptimizerCatalogue::getMRQueryInfo(tablename,info);
    printQueryInfo(cout, info, options);
  } catch (MineruleException& mr) {
    cout << "The query you specified is NOT present in the catalogue." << endl;
    return QUERY_NAME_NOT_FOUND;
  }

  return QUERY_NAME_FOUND;
}



void printMRQueryList(const ProgramOptions& options) {
  vector<CatalogueInfo> mrqlist;
  OptimizerCatalogue::getMRQueryInfos(mrqlist);
  vector<CatalogueInfo>::const_iterator it;
  for(it=mrqlist.begin(); it!=mrqlist.end(); it++) {
    printQueryInfo(cout, *it, options);
  }
}

void deleteQuery(const ProgramOptions& options) {
  if( !MineruleOptions::getSharedOptions().getSafety().getOverwriteHomonymMinerules() )
    throw MineruleException(MR_ERROR_OPTION_PARSING,
			    "The settings in the option file do not allow for "
			    "query deletions. If you really want to delete the "
			    "given minerule, set safety::overwriteHomonymMinerules to "
			    "True in your option file.");

  if( checkQueryName(options)==QUERY_NAME_FOUND )
    OptimizerCatalogue::deleteMinerule( options.getSearchParam(ProgramOptions::QryName) );
}


int
performCommands(ProgramOptions& options) {
  if( options.getCommand()==ProgramOptions::ShowList ) {
    printMRQueryList(options);
    return SUCCESS;
  }

  if( options.getCommand()==ProgramOptions::SearchQry) {
    return checkQueryName(options);
  }

  if( options.getCommand()==ProgramOptions::DeleteQry ) {
    deleteQuery(options);
    return SUCCESS;
  } 

  return NOTHING_TO_DO;
}

int
main(int argc, char** argv) {
  int resultVal;
  try {
    MineruleOptions& mr = MineruleOptions::getSharedOptions();
    ProgramOptions options;
    parseOptions(argc, argv, mr, options);

    resultVal=performCommands(options);
  } catch (MRCException& e) {
    resultVal = e.getResultID();
    if(resultVal!=NOTHING_TO_DO)
      cout << e.what() << endl;
  } catch (std::exception& e) {
    cout << "An error occurred, the error msg is:" << endl;
    cout << e.what() << endl;
    resultVal=ERROR_EXCEPTION_THROWN;
  }

  return resultVal;
}
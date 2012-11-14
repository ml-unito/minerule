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

#include "MRCUtils.h"



namespace mrc {
	bool
	fileExists(const std::string& filename) {
	  struct stat st;
	  if( stat(filename.c_str(),&st)==0 && S_ISREG(st.st_mode) ) {
	    return true;
	  } else {
	    return false;
	  }
	}

	void
	printHelp(int argc, char** argv) {
	  std::cout << "Usage:" << std::endl
	       << "   " << argv[0] << " [-h] [-s <sepstring>] [-f <optionfile>] " << std::endl
	       << "           [-n <queryname>] [-l] [-F formatSpecs] [-d <queryname>]"
	       << std::endl
	       << "The program allow to inspect the MR catalogue" << std::endl
	       << "-h - Output this message and returns NOTHING_TO_DO" << std::endl
	       << "-s - Change thestd::string used to separate different elements in" << std::endl
	       << "     the output. The default is ' '" << std::endl
	       << "-n - Look in the catalogue for the specified query" << std::endl
	       << "     it returns QUERY_NAME_FOUND, or QUERY_NAME_NOT_FOUND" << std::endl
	       << "     accordingly to whether the query could be found."<<std::endl 
	       << "-l - Print the list of already executed queries, returns" << std::endl
	       << "     SUCCESS upon completion" << std::endl
	       << "-d - Delete the given minerule from the system (notice that" << std::endl
	       << "     the safety options in the option file must be setted" << std::endl
	       << "     in such a way to allow the deletion. " << std::endl
	       << "-F - Format specifiers for printing the list of queries (-l)" <<std::endl
	       << "     Valid specifiers: s - Print the size of the result set" << std::endl
	       << "                       t - Print the text of the original mr" <<std::endl
	       << "                       r - Print the name of the result set" << std::endl
	       << "     the default is '', i.e.: print onlty the qry name" << std::endl
	       << "Return values:" << std::endl
	       << "      SUCCESS = " << mrc::SUCCESS << std::endl
	       << "      NOTHING_TO_DO = " << mrc::NOTHING_TO_DO << std::endl
	       << "      QUERY_NAME_FOUND = " << mrc::QUERY_NAME_FOUND << std::endl
	       << "      QUERY_NAME_NOT_FOUND = " << mrc::QUERY_NAME_NOT_FOUND << std::endl
	       << "      ERROR_OPTION_PARSING = " << mrc::ERROR_OPTION_PARSING << std::endl
	       << "      ERROR_CANNOT_OPEN_OPTION_FILE = " << mrc::ERROR_CANNOT_OPEN_OPTION_FILE <<std::endl;
	}

	void 
	doLoadOptions(const char* fname, minerule::MineruleOptions& opt) {
  
		if( fileExists(fname) ) {
			opt.readFromFile(fname);
		} else {
			std::string errstr;
			if(errno==0) {
				errstr = "Not a regular file!";
			} else {
				errstr = strerror(errno);
			}
    
			throw Exception(mrc::ERROR_CANNOT_OPEN_OPTION_FILE,
				std::string("Could not open file:") + fname + ". The reason is:" + errstr);
		}
	}

	void printVersion() {
	  std::cout << "MRCatalogue v:" << MR_VERSION << std::endl;
	}

	void
	parseOptions(int argc, char** argv, 
		      minerule::MineruleOptions& mopt,
		      Options& popt) {
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
	      popt.setSearchParam(Options::QryName, optarg);
	      break;
	    case 'd':
	      popt.setDeleteQry();
	      popt.setSearchParam(Options::QryName, optarg);
	      break;
	    case 's':
	      popt.setSepString(optarg);
	      break;
	    case 'v':
	      printVersion();
	      exit(0);
	    case '?':
	    default:
	      throw Exception( mrc::ERROR_OPTION_PARSING,
				  "Error while parsing options, use -h to show"
				  " the help message" );
	    }
	  }
	  if( !didLoadOptions  ) {
		  if( !fileExists(minerule::MineruleOptions::DEFAULT_FILE_NAME) )
		    throw Exception( mrc::ERROR_OPTION_PARSING,"You should specify at least one -f option or provide ./"+minerule::MineruleOptions::DEFAULT_FILE_NAME);
		  else
	        doLoadOptions(minerule::MineruleOptions::DEFAULT_FILE_NAME.c_str(), mopt);
	  }
	}

	void printQueryInfo(std::ostream& out, 
			    const minerule::CatalogueInfo& info,
			    const Options& options) {
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
  
	  out << std::endl;
	}


	int checkQueryName(const Options& options) {
	  const std::string& tablename = 
	    options.getSearchParam(Options::QryName);

	  try {
	    minerule::CatalogueInfo info;
	    minerule::OptimizerCatalogue::getMRQueryInfo(tablename,info, options.getListFormat().size);
	    printQueryInfo(std::cout, info, options);
	  } catch (minerule::MineruleException& mr) {
	    std::cout << "The query you specified is NOT present in the catalogue." << std::endl;
	    return mrc::QUERY_NAME_NOT_FOUND;
	  }

	  return mrc::QUERY_NAME_FOUND;
	}



	void printMRQueryList(const Options& options) {
	  std::vector<minerule::CatalogueInfo> mrqlist;
	  minerule::OptimizerCatalogue::getMRQueryInfos(mrqlist, options.getListFormat().size );
	  std::vector<minerule:: CatalogueInfo>::const_iterator it;
	  for(it=mrqlist.begin(); it!=mrqlist.end(); it++) {
	    printQueryInfo(std::cout, *it, options);
	  }
	}

	void deleteQuery(const Options& options) {
	  if( !minerule::MineruleOptions::getSharedOptions().getSafety().getOverwriteHomonymMinerules() )
	    throw minerule::MineruleException(minerule::MR_ERROR_OPTION_PARSING,
				    "The settings in the option file do not allow for "
				    "query deletions. If you really want to delete the "
				    "given minerule, set safety::overwriteHomonymMinerules to "
				    "True in your option file.");

	  if( checkQueryName(options)==mrc::QUERY_NAME_FOUND )
	    minerule::OptimizerCatalogue::deleteMinerule( options.getSearchParam(Options::QryName) );
	}


	int
	performCommands(Options& options) {
	  if( options.getCommand()==Options::ShowList ) {
	    printMRQueryList(options);
	    return mrc::SUCCESS;
	  }

	  if( options.getCommand()==Options::SearchQry) {
	    return checkQueryName(options);
	  }

	  if( options.getCommand()==Options::DeleteQry ) {
	    deleteQuery(options);
	    return mrc::SUCCESS;
	  } 

	  return mrc::NOTHING_TO_DO;
	}
} // namespace mrc
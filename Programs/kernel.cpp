#include<cstdio>
#include<cstdlib>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<iterator>
#include<cstring>
#include<cctype>
#include<sys/stat.h>
#include <unistd.h>

#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>

#include "Parsers/ParsedMinerule.h"
#include "Algorithms/Algorithms.h"
#include "Optimizer/OptimizedMinerule.h"
#include "Utils/MineruleOptions.h"
#include "Utils/MineruleErrors.h"


using namespace std;
using namespace minerule;


bool fileExist(const string& fname) {
  struct stat trash;
  if( stat(fname.c_str(),&trash)==0 ) 
    return true;
  else
    return false;
}

void
printHelp(char* progName) {
  cout << "Usage:" << endl
       << "  " << progName << " [-i <mineruletextfile>] [-m '<mineruletext>'] " << endl
       << "     -f <mineruleoptionfile> -O <optionlist> -v -h " << endl << endl;
  cout << "   -i -- specify a file name containing the text of the minerule" << endl
       << "   -m -- the argument is the text of the minerule" << endl
       << "   -f -- specify a file name containing the Options to be used." << endl
       << "         more than one is allowed and later ones override the options"<< endl
       << "         read from previous ones" << endl
       << "   -O -- Allows to specify some options from the command line" << endl
       << "         whether those options overrides or not the ones given by -f"<<endl
       << "         commands depends on which comes first" <<endl
       << "   -v -- version informations" << endl
       << "   -h -- this message" << endl
       << " exit codes:" << endl;
  for(int i=me_error_begin(); i<me_error_end(); i++) {
    cout 
      <<  "\t" << i << "\t- " << me_error_name((MineruleErrors) i) << endl;





  }
  
  exit(0);
}

void printVersion() {
  cout << "Minerule Kernel v:" << MR_VERSION << endl;
}


/*
 * Tries to parse mrtext in order to find the name of
 * the current minerule. 
 */

string 
parseMineruleName(const string& mrtext) {
  string mrtextcopy = mrtext;
  string spacechars = " \t\n";
  
  string mstringrule="mine rule ";
  string mstringsequence="mine sequence";
  string mstringitemset="mine itemset";
  string mstring;

  size_t minerulepos;
  size_t startnamepos = string::npos;
  size_t endnamepos = string::npos;

  // strlwr seems to be not standard and hence not
  // present in all systems. The following substitue it
  // (it is needed in order to implement case insensitivity)
  for(size_t i=0; i<mrtext.size(); i++) {
    mrtextcopy[i]=tolower(mrtextcopy[i]);
  }

  if(mrtextcopy.find(mstringrule,0)!=string::npos)
    mstring=mstringrule;
  else if(mrtextcopy.find(mstringitemset,0)!=string::npos)
    mstring=mstringitemset;
  else
    mstring=mstringsequence;
  
  minerulepos = mrtextcopy.find(mstring,0);
  
  if(minerulepos!=string::npos) 
    startnamepos = mrtextcopy.find_first_not_of(spacechars ,
						minerulepos+mstring.size());

  if(startnamepos!=string::npos)
    endnamepos = mrtextcopy.find_first_of(spacechars,startnamepos);

  
  if(endnamepos==string::npos ) {
    // notice we will be here, if any of the searches above fails
    MRErr() << "Error in retrieving the minerule name from the " 
	    << "text which define the minerule. I could not "
	    << "initialize, then, the text for %m parameter. " 
	    << "I'm now switching to the default 'mrnameerror' mine rule name" << endl
	    << "NOTE: If the minerule main parser succeed in parsing the" << endl
	    << " minerule text, then, this message is a BUG and should be"<<endl
	    << " reported!" << endl;
    return "mrnameerror";
  } 

  return mrtext.substr(startnamepos, endnamepos-startnamepos);
}


void
parseOptions(int argc, char** argv, MineruleOptions& mrOpts, string& mrtext) {
  int opt;
  bool mrtextok=false;
  while((opt=getopt(argc,argv,"hi:f:m:O:ev"))!=-1) {
    switch(opt) {
    case 'h':
      printVersion();
      printHelp(argv[0]);
      exit(0);
    case 'v':
      printVersion();
      exit(0);
    case 'i':
      {
	if(mrtextok) {
	  MRErr() << "More than one minerule has been specified! "
	    "(you cannot specify more than one {-i,-m} options)" << endl;

	  cerr  << "More than one minerule has been specified! "
	    "(you cannot specify more than one {-i,-m} options)" << endl;
	  exit(MR_ERROR_OPTION_PARSING);
	}
	ifstream infile(optarg);
	if(!infile) {
	  MRErr() << "Cannot open input file" << endl;
	  cerr <<  "Cannot open input file" << endl;
	  exit(MR_ERROR_INPUT_FILE_NOT_FOUND);
	}
	string buf;
	mrtext = "";
	while( infile >> buf ) {
	  mrtext += " " + buf;
	}

	mrtextok=true;
	mrOpts.setMineruleName(parseMineruleName(mrtext));
	mrOpts.setMineruleSourceName(optarg);
      }
      break;
    case 'f':
      if( fileExist(optarg) ) {
	mrOpts.readFromFile(optarg);
	MRLog() << "Options read from '" << optarg <<"'." << endl;
      } else {
	MRLog() << optarg << " not found, starting with default Options" << endl;
      }
      break;
    case 'O':
      mrOpts.readFromString(string(optarg));
      MRLog() << "Setted options accordingly to command line paramenter:-O " 
	      << optarg << endl;
      break;
    case 'm':
      if( mrtextok ) {
	  MRErr() << "More than one minerule has been specified! "
	    "(you cannot specify more than one {-i,-m} options)" << endl;

	  cerr  << "More than one minerule has been specified! "
	    "(you cannot specify more than one {-i,-m} options)" << endl;
	  exit(MR_ERROR_OPTION_PARSING);
      }
      mrtext=optarg;
      mrtextok=true;
      mrOpts.setMineruleName(parseMineruleName(mrtext));
      mrOpts.setMineruleSourceName("MRFromSTDIN");
      break;
    default:
      MRErr() << "Option not recognized:" << (char)opt << endl;
      cerr <<  "Option not recognized:" << (char)opt << endl;
      exit(MR_ERROR_OPTION_PARSING);
    }
  }

  if(!mrtextok) {
    MRErr() << "No minerule has been specified (use -i or -m options)" << endl;
    cerr << "No minerule has been specified (use -i or -m options)" << endl;
    exit(MR_ERROR_NO_MINERULE_SPECIFIED);
  }
}

int main (int argc, char *argv[])
{
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  string mrtext;
  parseOptions(argc, argv, mrOpts, mrtext);
  MRDebug() << "Minerule source text:[" << mrtext << "]" <<endl;

  MRLogPush("Minerule system starting");

  try {
    OptimizedMinerule optMR(mrtext);
    Algorithms::executeMinerule(optMR);
  } catch (odbc::SQLException& e) {
    MRErr() << "ODBC Exception:" << e.getMessage() << endl;
    cout << "ODBC Exception:" << e.getMessage() << endl;
    exit(MR_ERROR_DATABASE_ERROR);
    throw;
  } catch (MineruleException& e) {
    MRErr() << "Minerule Exception:" << e.what() << endl;
    cout << "Minerule Exception:" << e.what() << endl;
    exit( e.getErrorCode() );
  } catch (std::exception& e) {
    MRErr() << "Exception:" << e.what() << endl;
    cout << "Exception:" << e.what() << endl;
    exit( MR_ERROR_UNKNOWN );
  }
  catch(...) {
    MRErr() << "An unknown exception has been thrown..." << endl;
    cout << "An unknown exception has been thrown..." << endl;
    exit( MR_ERROR_UNKNOWN );
  }

  MRLogPop();

  return 0;
}



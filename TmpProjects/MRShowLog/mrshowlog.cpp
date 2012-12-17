#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>


#include "minerule/Utils/MineruleOptions.h"

using namespace std;
using namespace minerule;



void
printHelp(int argc, char** argv) {
  cout << "Usage:" << endl
       << "   " << argv[0] << " [-h] " 
       << "<option file> <log type> <minerule name>" << endl
       << endl
       << "The program prints one of the log files of the specified minerule." << endl
       << "The <log type> parameters can be either: log, err, warn, debug." << endl
       << "Return values:" << endl
       << "      0 on success" << endl
       << "      1 on error" << endl;
}

void getToken( istream& infile, string& curToken ) {
  static char buf[2048];
  static int pos = -1;

  while( buf[pos]==' ' )
    pos++;

  if(pos==-1 || buf[pos]=='\0') {
    bool isAComment;
    do {
      infile.getline(buf, 2048);
      size_t i=0;
      while(buf[i]==' ')
	i++;

      isAComment=buf[i]=='#' || buf[i]=='\0';
    } while(infile && isAComment);

    if(!infile)
      return;
    pos=0;
  }
  
  curToken="";
  if(buf[pos]=='"') {
    pos++;
    while(buf[pos]!='\0' && buf[pos]!='"') {
      curToken+=buf[pos];
      pos++;
    }
    pos++;
  } else {
    while( buf[pos]!='\0' && (isalpha(buf[pos])||isdigit(buf[pos])) ) {
      curToken+=buf[pos];
      pos++;
    }
  }
  
  if(buf[pos]=='\0')
    pos=-1; 
  else if(curToken=="") {
    curToken=buf[pos];
    pos++;
  }

  //  cout << curToken << endl;
}


string getLogfileName( const string& optfile, string logname) {
  logname+="stream";

  ifstream infile(optfile.c_str());
  string curtok;
  enum _parserstate {
    OUTER,
    LOGNAME_FOUND,
    INSIDE_STREAM_BLOCK
  };

  enum _parserstate parserstate = OUTER;
  getToken(infile, curtok);

  while(infile) {
    //    cout << parserstate << endl;
    switch( parserstate ) {
    case OUTER:
      if(curtok==logname) {
	parserstate=LOGNAME_FOUND;
      }
      getToken(infile, curtok);
      break;

    case LOGNAME_FOUND:
      if(curtok!=":")
	throw MineruleException(MR_ERROR_OPTION_PARSING, 
				"Found the logstream id, but not the :: characters");
      getToken(infile,curtok);
      if(!infile || curtok!=":")
	throw MineruleException(MR_ERROR_OPTION_PARSING, 
				"Found the logstream id, but not the :: characters");

      getToken(infile,curtok);
      if(!infile || curtok=="{")
	getToken(infile,curtok);

      parserstate = INSIDE_STREAM_BLOCK;
      
      break;

    case INSIDE_STREAM_BLOCK:
      if(curtok=="stream") {
	getToken(infile,curtok);
	if(!infile || curtok!="=")
	  throw MineruleException(MR_ERROR_OPTION_PARSING, 
				  "Found the stream, but not the = character");
	getToken(infile,curtok);
	return curtok;
      } 

      getToken(infile,curtok);
    }
  }

  throw MineruleException(MR_ERROR_OPTION_PARSING,
			  "Failed to find the "+logname+"::stream option inside the option file");
}

void printLog( const string& optfile,
	       const string& logname, 
	       const string& mrname ) throw(MineruleException) {
  /*  MineruleOptions& mr = MineruleOptions::getSharedOptions();
  mr.setMineruleSourceName(mrname);
  mr.setMineruleName(mrname);
  mr.readFromFile(optfile.c_str());

  // Searching for the correct Logger object
  
  const MRLogger* logger = NULL;
  

  if( logname=="log" ) {
    logger = &mr.getLogStreamObj().getLogger();
  } else if(logname=="err") {
    logger = &mr.getErrStreamObj().getLogger();
  } else if(logname=="warn") {
    logger = &mr.getWarnStreamObj().getLogger();
  } else if(logname=="debug") {
    logger = &mr.getDebugStreamObj().getLogger();
  } else {
    throw MineruleException(MR_ERROR_OPTION_PARSING, "The log type must be one of {log,err,warn,debug}");
  }

  // Finding out to which file it is attached

  const map< std::string, MRLogger*>&
    knownStreams = mr.getKnownStreams();

  map< std::string, MRLogger*>::const_iterator it =
    knownStreams.begin();

  while( it!=knownStreams.end() &&
	 it->second!=logger ) {
    it++;
  }

  if(it==knownStreams.end()) {
    throw MineruleException( MR_ERROR_INTERNAL, "Could not find the correct logger object, "
			     "this should not happen (it is a bug!)." );
  } */

  string logfile = getLogfileName(optfile,logname);

  // Expanding %m and %i characters

  size_t markerPos;
  if((markerPos=logfile.find("%m"))!=logfile.npos) {
    logfile.erase(markerPos,2);
    logfile.insert(markerPos,mrname );
  }

  if((markerPos=logfile.find("%i"))!=logfile.npos) {
    logfile.erase(markerPos,2);
    logfile.insert(markerPos,mrname );
  }

  // further error checkings 

  if( logfile=="<stdout>" ||
      logfile=="<stderr>" ||
      logfile=="/dev/null") {
    throw MineruleException( MR_ERROR_UNKNOWN, "Cannot print the log file you requested. It is"
			     " attached to the stream '"+logfile+"' which cannot be read.");
  }

  // finally we can open the stream and output its contents
  ifstream infile( logfile.c_str() );
  if(!infile) {
    throw MineruleException( MR_ERROR_UNKNOWN, "Cannot print the log file you requested. It is"
			     " attached to the stream '"+logfile+"' which I cannot read.");
  }

  char buf[2048];
  while(infile) {
    infile.getline(buf, 2048);
    cout << buf << endl;
  }
}

void printVersion() {
  cout << "MRShowLog v:" << MR_VERSION << endl;
}


int
main(int argc, char** argv) {
  if( argc>1 && string(argv[1])=="-h" ) {
    printHelp(argc, argv);
    exit(0);
  }

  if( argc>1 && string(argv[1])=="-v") {
    printVersion();
    exit(0);
  }

  try {
    if( argc<=3 ) {
      throw MineruleException( MR_ERROR_OPTION_PARSING, string("Wrong number of parameters! type: '") 
			       + argv[0] + " -h' for usage notes. ");
    }

    printLog( argv[1], argv[2], argv[3]);
  } catch (std::exception& e) {
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

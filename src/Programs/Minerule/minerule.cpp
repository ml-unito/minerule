#include<string>
#include<iostream>
#include<unistd.h>
#include<assert.h>
#include<errno.h>

#include "Utils/MineruleException.h"
#include "Utils/MineruleErrors.h"
#include "Utils/StringUtils.h"
#include "Utils/MineruleLogs.h"

using namespace minerule;

void execSubCommand( std::string cmd, int argc, char** argv ) {
	const char** cmdArgv = (const char**) malloc( sizeof(char*[argc+2]) );
	assert(cmdArgv != NULL );
	std::string executableName = std::string("mr-")+cmd;
	
	cmdArgv[0] = executableName.c_str();
	for(size_t i=1; i<argc+1; ++i) {
		cmdArgv[i] = argv[i-1];
	}
	cmdArgv[argc+1] = NULL;
	
	errno = 0;
	execvp(executableName.c_str(), (char* const*) cmdArgv);
	if(errno!=0) {
		MRLog() << strerror(errno) << std::endl;
		exit(10);
	}
}

void printMRHelp() {
	std::cout << StringUtils::to_bold("Usage:") << std::endl
		<< StringUtils::to_bold("mr [-h] <sub-command> [-h] [options]") << std::endl
		<< "   Available sub-commands are: query, print, catalogue, and defaults" << std::endl
		<< StringUtils::to_bold("   -h ") << " -- shows help for the given command if given after a sub-command name." << std::endl
		<< "          it shows this message if given right after the 'mr' command" << std::endl;
}


void checkSubCommands(int argc, char** argv) {
	if(argc >= 2 && std::string(argv[1]) == "-h") {
		printMRHelp();
		exit(0);
	}
	
	if(argc < 2) 
		throw MineruleException( MR_ERROR_OPTION_PARSING, "Command not recognized, see help for a list of available commands." );
	
	std::string subCmd( argv[1] );
	if(subCmd == "print") {
		execSubCommand(subCmd, argc-2, &argv[2]);
	}
	
	if(subCmd == "catalogue") {
		execSubCommand(subCmd, argc-2, &argv[2]);
	}
	
	if(subCmd == "defaults") {
		execSubCommand(subCmd, argc-2, &argv[2]);
	}
	
	if(subCmd == "query") {
		execSubCommand(subCmd, argc-2, &argv[2]);
	}
	
	throw MineruleException( MR_ERROR_OPTION_PARSING, "Command not recognized, see help for a list of available commands." );
}

int main (int argc, char *argv[])
{	
	try {		
		checkSubCommands(argc, argv);
	} catch (MineruleException& e) {
		MRErr() << "Minerule Exception:" << e.what() << std::endl;
		exit( e.getErrorCode() );
	} catch (std::exception& e) {
		MRErr() << "Exception:" << e.what() << std::endl;
		exit( MR_ERROR_UNKNOWN );
	}
	catch(...) {
		MRErr() << "An unknown exception has been thrown..." << std::endl;
		exit( MR_ERROR_UNKNOWN );
	}

	return 0;
}



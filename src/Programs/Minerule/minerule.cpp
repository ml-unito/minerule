#include<string>
#include<iostream>
#include<unistd.h>
#include<assert.h>
#include<errno.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

#include "Utils/MineruleException.h"
#include "Utils/MineruleErrors.h"
#include "Utils/StringUtils.h"
#include "Utils/MineruleLogs.h"

using namespace minerule;

std::string buildPath(std::string cmdName) {
	size_t last_slash_pos = cmdName.find_last_of("/");
	if(last_slash_pos == std::string::npos)
		return "";
	
	return cmdName.substr(0, last_slash_pos+1);
}


void execSubCommand( std::string cmd, int argc, char** argv, std::string cmdPath ) {
	const char** cmdArgv = (const char**) malloc( sizeof(char*[argc+2]) );
	assert(cmdArgv != NULL );
	std::string executableName = cmdPath + std::string("mr-")+cmd;
	
	cmdArgv[0] = executableName.c_str();
	for(size_t i=1; i<argc+1; ++i) {
		cmdArgv[i] = argv[i-1];
	}
	cmdArgv[argc+1] = NULL;
	
	errno = 0;
	execvp(executableName.c_str(), (char* const*) cmdArgv);
	if(errno!=0) {
		MRErr() << "Could not launch command:" + executableName << std::endl
			<< "       The system reported error: " << strerror(errno) << std::endl;
		exit(1);
	}
}

void printMRHelp() {
	std::cout << StringUtils::to_bold("Usage:") << std::endl
		<< StringUtils::to_bold("   mr -h                          ") << " - shows this message." << std::endl
		<< StringUtils::to_bold("   mr <sub-command> [-h] [options]") 
			<< " - executes a sub-command. Use the -h option for further information." << std::endl
		<< std::endl
		<< StringUtils::to_bold("Available sub-commands are: ") 
		<< "query, catalogue, print, and defaults."
		<< std::endl
		<< StringUtils::to_bold("Note: ") << "Sub-commands can be shortened at will" 
		<< ", e.g.: 'mr pr' is equivalent to 'mr print'" << std::endl;
}


std::string expandSubCommand(std::string cmdPrefix) {
	static std::string commands[] = { "query", "catalogue", "print", "defaults" };
	
	for(size_t i=0; i<4; ++i) {
		if( commands[i].find(cmdPrefix) == 0 ) // cmdPrefix is a prefix of commands[i]
			return commands[i];
	}
	
	return cmdPrefix;
}


void checkSubCommands(int argc, char** argv) {
	if(argc < 2 || std::string(argv[1]) == "-h") {
		printMRHelp();
		exit(0);
	}
		
	std::string subCmd( expandSubCommand(argv[1]) );
	if(subCmd == "print") {
		execSubCommand(subCmd, argc-2, &argv[2], buildPath(argv[0]));
	}
	
	if(subCmd == "catalogue") {
		execSubCommand(subCmd, argc-2, &argv[2], buildPath(argv[0]));
	}
	
	if(subCmd == "defaults") {
		execSubCommand(subCmd, argc-2, &argv[2], buildPath(argv[0]));
	}
	
	if(subCmd == "query") {
		execSubCommand(subCmd, argc-2, &argv[2], buildPath(argv[0]));
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



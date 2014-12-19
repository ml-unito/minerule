#include "minerule/Utils/Progress.hpp"
#include "minerule/Utils/MineruleLogs.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include <unistd.h>
#include <signal.h>

namespace minerule {
	
Progress::UpdateHandler Progress::defaultHandler_(clockHandsUpdateHandler);

void abort_handler(int s){
	std::string msg = "Ctrl-C or Ctrl-Z caught \e[?25h";
	write(1, msg.c_str(), msg.size());
  exit(1); 
}


void install_abort_handler() {
	static bool handlerInstalled = false;
	if(handlerInstalled) {
		return;
	}
		
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = abort_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

   handlerInstalled = !sigaction(SIGINT, &sigIntHandler, NULL);
}
	
bool Progress::handleStartStop(std::ostream& logger, int n) {
	install_abort_handler();

	if(n == -1) {
		MRLog() << "  ";
		logger << "\e[?25l"; 
		return true;
	}
		
	if(n == -2) {
		logger << std::endl;
		logger << "\e[?25h";
		return true;
	}
	
	return false;
}

void angledSpeenWheelUpdateHandler(int n, int count, int num) {
  static unsigned int step = 0;
  static char spinWheel[] = { '^', '>', 'v', '<' };
	
	static std::ostream& logger = 
		*MineruleOptions::getSharedOptions()
			.getLogStreamObj()
			.getLogger()
			.getStream();
	
	if(Progress::handleStartStop(logger, n))
		return;
	
  if((count+n) % num) {
    logger << "\b" << spinWheel[step++ % 4];
		logger.flush();
  }
}

void barSpeenWheelUpdateHandler(int n, int count, int num) {
  static unsigned int step = 0;
  static char spinWheel[] = { '|', '/', '-', '\\' };

	static std::ostream& logger = 
		*MineruleOptions::getSharedOptions()
			.getLogStreamObj()
			.getLogger()
			.getStream();
	
	if(Progress::handleStartStop(logger, n))
		return;
	
  if((count+n) % num) {
    logger << "\b" << spinWheel[step++ % 4];
		logger.flush();
  }
}

void tickNumberUpdateHandler(int n, int count, int num) {
	if(n<0) {
		return;
	}
	
	if((count+n) % num == 0) {
		MRLog() << count+n << std::endl;
	}
}

void clockHandsUpdateHandler(int n, int count, int num) {
  static unsigned long int step = 0;
  static std::string spinWheel[] = 
	  { "🕐", "🕑", "🕒", "🕓",
	 	"🕔", "🕕", "🕖", "🕗",
		"🕘", "🕙", "🕚", "🕛" };
	
	static std::ostream& logger = 
		*MineruleOptions::getSharedOptions()
			.getLogStreamObj()
			.getLogger()
			.getStream();
	
	if(Progress::handleStartStop(logger, n)) {
		return;
	}
	
  if((count+n) % num == 0) {
    logger << "\b\b" << spinWheel[step++ % 12] << " ";
		logger.flush();
  }
}



}

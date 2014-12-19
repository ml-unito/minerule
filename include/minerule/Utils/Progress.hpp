#ifndef MR_PROGRESS_HPP_K3MX32K3U2L__
#define MR_PROGRESS_HPP_K3MX32K3U2L__

#include <iostream>

namespace minerule {

// Implements an indeterminate progress indicator
class Progress {
public:
  typedef void (*UpdateHandler)(int n, int count, int num);

private:
  static UpdateHandler defaultHandler_;
	
	UpdateHandler handler_;
  int count_;
  int num_;
  
public:
  Progress(int num) : count_(0), num_(100), handler_(defaultHandler_) {}
  ~Progress() {}

  void start() {
	count_=0;
	handler_(-1, 0, 0);
  }
  
  void end() {
    handler_(-2, 0, 0);
  }
  

  void tick(int n=1) {
    handler_(n, count_, num_);
    count_+=n;;
  }
  
	// helper function for default behavior on start/stop call
	static bool handleStartStop(std::ostream&, int n);

  static UpdateHandler setDefaultHandler(UpdateHandler handler) {
    UpdateHandler tmp = defaultHandler_;
    defaultHandler_ = handler;
    return tmp;
  }
	
	void setHandler(UpdateHandler handler) {
		handler_ = handler;
	}
};


extern void clockHandsUpdateHandler(int,int,int);
extern void tickNumberUpdateHandler(int,int,int);
extern void barSpeenWheelUpdateHandler(int,int,int);
extern void angledSpeenWheelUpdateHandler(int,int,int);

} /* minerule */

#endif /* end of include guard: MR_PROGRESS_HPP_K3MX32K3U2L__ */

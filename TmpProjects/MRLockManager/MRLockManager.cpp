#include <iostream>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


/**
 * MRLockManager 
 * This class ensure an easy to use, but secure locking method on
 * files. It uses the atomic unix flock sys call to lock the files
 * (hence it is NOT possible that two different processes lock the
 * file at the same time). Moreover, the lock is automatically removed
 * in case the process dies (hence, it is not possible that a file
 * remains locked because another program died unexpectadly).
 */


using namespace std;

class MRLockManager {
  int fd;

  void writePid() throw (int) {
    char buf[255];
    sprintf(buf, "%d", getpid());
    if(write(fd, buf, strlen(buf))==-1) {
      cout << "Cannot write process pid into locked file." 
	   << "Error: " << strerror(errno) << endl;
      throw(4);
    }
  }

  bool
  fileExists(const char* str) {
    struct stat st;
    if( stat(str,&st)!=-1 ) {
      return true;
    } else {
      return false;
    }
  }


  bool
  fileLocked(const char* str) {
    bool retVal=false;
    int fd = open(str, O_RDONLY );
    if(fd==-1) {
      cout << "fileLocked:";
      cout << "could not open the file, the reason is:" 
	   << strerror(errno) << endl;
      return false;
    }
    
    if( ::flock(fd, LOCK_EX | LOCK_NB)==-1 ) {
      if( errno == EWOULDBLOCK )
	retVal = true;
      else {
	cout << "some kind of error occurred, the strerror is: "
	     << strerror(errno) << endl;
      }
    } else {
      ::flock( fd, LOCK_UN );
    }
    
    close(fd);
    return retVal;
  }

 public:
  void lock(const std::string& fname) throw (int) {
    if(fileExists(fname.c_str())) {
      cout << "file exists..." << endl;
      // Looking if I can get control of the file...
      if(fileLocked(fname.c_str())) {
	cout << "file locked!" << endl;
	throw 1; // file locked!
      } else {
	cout << "truncating the file..." << endl; 
	fd=open(fname.c_str(), 
		O_TRUNC | O_RDWR,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
	if(fd!=-1) {
	  if( flock( fd, LOCK_EX)!=-1 ) { 
	    writePid();
	  } else {
	    cout << "Could not lock the file!" << endl;
	  }
	} else {
	  cout << "Some error occurred while trying to lock file:"
	       << fname << endl << "reported error:" << strerror(errno)
	       << endl;
	  throw 2;
	}
      }
    } else {
      cout << "creating the file!" << endl;
      fd=open(fname.c_str(),
	      O_CREAT | O_RDWR,
	      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
      if(fd==-1) {
	cout << "Some error occurred in creating the file, the"
	     << " reported error is:" << strerror(errno) << endl;
	throw 3;
      } else {
	if( flock( fd, LOCK_EX)!=-1 ) { 
	  writePid();
	} else {
	  cout << "Could not lock the file!" << endl;
	}
      }
    }
  }

  void unlock() throw (int) {
    if(flock(fd, LOCK_UN )==-1) {
      cout << "Cannot unlock the file! Error: " 
	   << strerror(errno) << endl;
    }
  }
};


int main() {
  MRLockManager lm;
  try {
    lm.lock("xxx");
    cout << "Locked!" << endl;
    sleep(10);
    lm.unlock();
    cout << "Unlocked!" << endl;
  } catch (int i) {
    cout << "i:" << i << endl;
    return i;
  }

  return 0;
}

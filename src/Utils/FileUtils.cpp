#include "minerule/Utils/FileUtils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace minerule {
	bool FileUtils::fileExists(const std::string& fname) {
		struct stat trash;
		if( stat(fname.c_str(),&trash)==0 ) 
			return true;
		else
			return false;
	}
}
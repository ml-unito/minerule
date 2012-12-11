#ifndef FILEUTILS_H_MLEUV1SZ
#define FILEUTILS_H_MLEUV1SZ

#include <string>

namespace minerule {
	class FileUtils {
	public:
		static bool fileExists(const std::string& filename);
	};
}

#endif /* end of include guard: FILEUTILS_H_MLEUV1SZ */

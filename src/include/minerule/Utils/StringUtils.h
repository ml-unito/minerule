#ifndef STRINGUTILS_H_7KCWVFQO
#define STRINGUTILS_H_7KCWVFQO

#include <vector>
#include <string>

namespace minerule {
	class StringUtils {
	public:
		static std::vector<std::string>* split_to_length(const std::string& str, size_t len);
		static std::string join(const std::vector<std::string>&, const std::string& sep );
	};
}

#endif /* end of include guard: STRINGUTILS_H_7KCWVFQO */

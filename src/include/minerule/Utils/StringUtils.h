#ifndef STRINGUTILS_H_7KCWVFQO
#define STRINGUTILS_H_7KCWVFQO

#include <vector>
#include <string>

namespace minerule {
	class StringUtils {
	private:		
		static std::string colorize(const std::string& str, const std::string& color) { return color + str + CLOSE; }
		
	public:
		static std::vector<std::string>* split_to_length(const std::string& str, size_t len);
		static std::string join(const std::vector<std::string>&, const std::string& sep );
		
		static const std::string RED;
		static const std::string BOLD;
		static const std::string GREEN;
		static const std::string YELLOW;
		static const std::string BLUE;
		static const std::string WHITE;
		static const std::string CLOSE;
		
		static std::string to_bold(const std::string& str){ return colorize(str, BOLD); }
		static std::string to_red(const std::string& str)   { return colorize(str,RED);   }
		static std::string to_green(const std::string& str) { return colorize(str,GREEN); }
		static std::string to_white(const std::string& str) { return colorize(str,WHITE); }
		static std::string to_blue(const std::string& str) 	{ return colorize(str,BLUE); }
		static std::string to_yellow(const std::string& str){ return colorize(str,YELLOW); }
		static std::string to_orange(const std::string& str){ return colorize(colorize(str,YELLOW), BOLD); }
		
	};
}

#endif /* end of include guard: STRINGUTILS_H_7KCWVFQO */

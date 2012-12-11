#ifndef STRINGUTILS_H_7KCWVFQO
#define STRINGUTILS_H_7KCWVFQO

#include <vector>
#include <string>

namespace minerule {
	class StringUtils {
	private:
		static bool enableColors;		
		static std::string colorize(const std::string& str, const std::string& color) { return enableColors ? color + str + CLOSE : str; }
		
	public:
		static std::vector<std::string>* splitToLength(const std::string& str, size_t len);
		static std::string join(const std::vector<std::string>&, const std::string& sep );
		
		static const std::string RED;
		static const std::string BOLD;
		static const std::string GREEN;
		static const std::string YELLOW;
		static const std::string BLUE;
		static const std::string WHITE;
		static const std::string CLOSE;
		
		static void setColorsEnabled(bool newVal) { enableColors = newVal; }
		static std::string toBold(const std::string& str){ return colorize(str, BOLD); }
		static std::string toRed(const std::string& str)   { return colorize(str,RED);   }
		static std::string toGreen(const std::string& str) { return colorize(str,GREEN); }
		static std::string toWhite(const std::string& str) { return colorize(str,WHITE); }
		static std::string toBlue(const std::string& str) 	{ return colorize(str,BLUE); }
		static std::string toYellow(const std::string& str){ return colorize(str,YELLOW); }
		static std::string toOrange(const std::string& str){ return colorize(colorize(str,YELLOW), BOLD); }
		
	};
}

#endif /* end of include guard: STRINGUTILS_H_7KCWVFQO */

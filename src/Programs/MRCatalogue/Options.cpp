#include "Options.h"

namespace mrc {

	void Options::setListFormat(const char* format) throw (Exception) {
		if(format==NULL)
			return;

		for(int i=0; format[i]!='\0'; i++) {
			switch( format[i] ) {
				case 's':
					listFormat.size=true;
					break;
				case 't':
					listFormat.text = true;
					break;
				case 'r':
					listFormat.result = true;
					break;
				default:
					throw Exception(mrc::ERROR_OPTION_PARSING, std::string("Unknown flag:")+format[i]);
			}
		}
	}

}
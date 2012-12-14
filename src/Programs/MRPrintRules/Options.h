#ifndef OPTIONS_H_IK79Q6DV
#define OPTIONS_H_IK79Q6DV

#include <string>
#include "Utils/MineruleOptions.h"

namespace mrprint {
	enum MRPRINT_ERRORS { MRPRINT_OPTION_PARSING_ERROR=1 };
	
	void printError(const std::string& str);
	
	class Options	{
	public:
		Options(int argc, char** argv) : _argc(argc), _argv(argv), _mroptFileName(minerule::MineruleOptions::DEFAULT_FILE_NAME), _noLogArtifacts(false), _noLowConfidenceFilter(false), _sortOrder("no"), _queryNumber(0), _queryName("") {};
		virtual ~Options () {};
		
		void parse();
		void printHelp() const;
		
		// accessors
		std::string mroptFileName() const { return _mroptFileName; }
		bool noLogArtifacts() const { return _noLogArtifacts; }
		bool noLowConfidenceFilter() const { return _noLowConfidenceFilter; }
		std::string sortOrder() const { return _sortOrder; }
		size_t queryNumber() const { return _queryNumber; }
		std::string queryName() const { return _queryName; }

				
	private:
		char** _argv;
		int _argc;
		
		std::string _mroptFileName;
		bool _noLogArtifacts;
		bool _noLowConfidenceFilter;
		std::string _sortOrder;
		size_t _queryNumber;
		std::string _queryName;
	};


} 

#endif /* end of include guard: OPTIONS_H_IK79Q6DV */

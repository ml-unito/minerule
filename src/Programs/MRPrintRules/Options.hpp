//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef OPTIONS_H_IK79Q6DV
#define OPTIONS_H_IK79Q6DV

#include <string>
#include "minerule/Utils/MineruleOptions.hpp"

namespace mrprint {
	enum MRPRINT_ERRORS { MRPRINT_OPTION_PARSING_ERROR=1 };

	void printError(const std::string& str);

	class Options	{
	public:
		Options(int argc, char** argv) :
				_argv(argv),
				_argc(argc),
				_mroptFileName(minerule::MineruleOptions::DEFAULT_FILE_NAME),
				_noLogArtifacts(false),
				_noLowConfidenceFilter(false),
				_sortOrder("no"),
				_queryNumber(0),
				_queryName("") {};
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

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
#include "minerule/Utils/MineruleOptions.h"

#include "ErrorCodes.h"
#include "MRCUtils.h"
#include "Options.h"

using namespace minerule;

int
main(int argc, char** argv) {
	if( argc < 2 ) {
		mrc::printHelp(argc, argv);
		exit(0);
	}

	int resultVal;
	try {
	  
		MineruleOptions& mr = MineruleOptions::getSharedOptions();
		mrc::Options options;
		mrc::parseOptions(argc, argv, mr, options);

		resultVal=mrc::performCommands(options);
	} catch (mrc::Exception& e) {
		resultVal = e.getResultID();
		if(resultVal!=mrc::NOTHING_TO_DO)
			std::cout << e.what() << std::endl;
	} catch (std::exception& e) {
		std::cout << "An error occurred, the error msg is:" << std::endl;
		std::cout << e.what() << std::endl;
		resultVal=mrc::ERROR_EXCEPTION_THROWN;
	}

	return resultVal;
}

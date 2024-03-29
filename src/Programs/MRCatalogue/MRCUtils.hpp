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
#ifndef MRCATALOGUE_H_89D0JOQH
#define MRCATALOGUE_H_89D0JOQH

#include "Options.hpp"
#include "minerule/Utils/MineruleOptions.hpp"

namespace mrc { // minerule catalogue
  
	// --------------------------------------------------------------------------------
	// Functions 
	// --------------------------------------------------------------------------------
	void printVersion();
	void printHelp(int argc, char** argv);
	void parseOptions(int argc, char** argv, minerule::MineruleOptions& mopt, Options& popt);
	int  performCommands(Options& options);
}

#endif /* end of include guard: MRCATALOGUE_H_89D0JOQH */

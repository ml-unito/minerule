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
#ifndef MRMATCH_H_MP1T5YIM
#define MRMATCH_H_MP1T5YIM

#include <vector>
#include "minerule/Result/Rule.h"

namespace mrmatch {
	typedef enum { SUCCESS=0, MRMATCH_OPTION_PARSING_ERROR  } ExitCodes;
	
	// A matcher spec is the bit and of a match kind and a matchoutput.
	// the values of the two types have been designed so that one can
	// combine a matchkind and a matchoutput using the or bit-operator
	// and retrieve the components by anding the result with the appropriate 
	// matchmask.
	// Eg. GidRules | OutOnDb --> 10
	//     10 & MatchKindMask --> 2 == GidRules
	//     10 & MatchOutputMask --> 8 ==- OutOnDb
	// Alternatively, one can check if a particular flag is set by anding the
	// result with one of the MatchKind/MatchOutput.
	// Eg. GidRules | OutOnDb --> 10
	//     10 & GidRules --> true
	//     10 & RuleGids --> false
	//     10 & OutOnConsole --> false
	//     10 & OutOnDb --> true
	typedef enum { None = 0, RuleGids = 1, GidRules = 2, MatchKindMask = 3 } MatchKind;
	typedef enum { OutOnConsole = 4, OutOnDB = 8, MatchOutputMask = 12 } MatchOutput;
	typedef unsigned int MatcherSpecs;	
}


#endif /* end of include guard: MRMATCH_H_MP1T5YIM */

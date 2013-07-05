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
#ifndef SOURCETABLEREQUIREMENTS_H_E3HLQ5I6
#define SOURCETABLEREQUIREMENTS_H_E3HLQ5I6

namespace minerule {
	
	// Stores the information about how the source table needs to be constructed.
	// Usage example:
	//		/* Initialize it, by or-ing the constants SortedGids and CrossProducts */
	// 		SourceTableRequirements reqs( SourceTableRequirements::SortedGids );
	//
	//		/* access to the requirements using the provided accessors */
	//		reqs.sortedGids(); 			/* returns true */
	//		reqs.crossProduct(); 		/* returns false */	
	class SourceTableRequirements {
	public:
		typedef enum { SortedGids = 1, CrossProduct = 2 } Requirements;
		typedef unsigned short RequirementsSet;
	
		SourceTableRequirements() : _requirements(0) {}
		SourceTableRequirements(RequirementsSet requirements) : _requirements(requirements) { }
		virtual ~SourceTableRequirements () {}

		bool sortedGids() 	const { return _requirements & SortedGids;   }
		bool crossProduct() const { return _requirements & CrossProduct; }
		
		void set(RequirementsSet requirements) { _requirements = requirements; }
	private:
		RequirementsSet _requirements;
	};

}

#endif /* end of include guard: SOURCETABLEREQUIREMENTS_H_E3HLQ5I6 */

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

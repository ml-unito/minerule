#ifndef SOURCETABLE_H_GE1S8NL9
#define SOURCETABLE_H_GE1S8NL9


namespace minerule {
	class SourceTable {
	public:
		class Iterator {
		private:
			SourceRow* _sourceRow;
			
		public:
			bool next();
			bool isAfterLast() const;
			SourceRow* get();
		};
		
		
		SourceTable(const MiningAlgorithmBase& algorithm) : _algorithm(algorithm) {};
		virtual ~SourceTable ();

	// Creates the mining source table
		void init();
		size_t getTotGroups() const;
	
		Iterator* getIterator();
	private:
	/* data */
		const MiningAlgorithmBase& _algorithm;
	};
}
#endif /* end of include guard: SOURCETABLE_H_GE1S8NL9 */

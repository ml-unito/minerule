#ifndef PRINTER_H_LDJGVI61
#define PRINTER_H_LDJGVI61

#include <iostream>
#include "Options.h"
#include "Optimizer/OptimizerCatalogue.h"

namespace mrc {
	class Printer {
		std::ostream& _out;
		const Options& _options;		
		size_t _result_index;
		static const size_t OUTPUT_MAX_LEN;
	public:
		Printer(std::ostream& out, const Options& options) : _out(out), _options(options) {}
		
		void print(const minerule::CatalogueInfo& info);
		void print(const std::vector<minerule::CatalogueInfo>& list);
	private:
		void printIndex() const;
		void format(std::string header, std::string info);				
		void format(std::string header, size_t info);				
	};
}


#endif /* end of include guard: PRINTER_H_LDJGVI61 */

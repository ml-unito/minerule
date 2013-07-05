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
#ifndef PRINTER_H_LDJGVI61
#define PRINTER_H_LDJGVI61

#include <iostream>
#include "Options.h"
#include "minerule/Optimizer/OptimizerCatalogue.h"

namespace mrc {
	class Printer {
		std::ostream& _out;
		const Options& _options;		
		size_t _result_index;
		static const size_t OUTPUT_MAX_LEN;
	public:
		Printer(std::ostream& out, const Options& options) : _out(out), _options(options), _result_index(1) {}
		
		void print(const minerule::CatalogueInfo& info);
		void print(const std::vector<minerule::CatalogueInfo>& list);
	private:
		void printIndex() const;
		void format(std::string header, std::string info);				
		void format(std::string header, size_t info);				
	};
}


#endif /* end of include guard: PRINTER_H_LDJGVI61 */

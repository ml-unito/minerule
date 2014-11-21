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
#ifndef MYSQLCATALOGUEINSTALLER_H_3QIF6B9O
#define MYSQLCATALOGUEINSTALLER_H_3QIF6B9O

#include "minerule/Optimizer/Installers/PostgresCatalogueInstaller.hpp"

namespace minerule {

	// right now the postgres installer works also for mysql.
	// so the mysql installer simply inherits from it.
	class MySqlCatalogueInstaller : public PostgresCatalogueInstaller {
	public:
		MySqlCatalogueInstaller() : PostgresCatalogueInstaller() {} ;
		virtual ~MySqlCatalogueInstaller () {};
	};
}

#endif /* end of include guard: MYSQLCATALOGUEINSTALLER_H_3QIF6B9O */

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
#ifndef POSTGRESCATALOGUEINSTALLER_H_M0QRV1OU
#define POSTGRESCATALOGUEINSTALLER_H_M0QRV1OU

#include "minerule/Optimizer/CatalogueInstaller.hpp"

namespace minerule {
	class PostgresCatalogueInstaller : public CatalogueInstaller {
	public:
		PostgresCatalogueInstaller() : CatalogueInstaller() {}
		virtual ~PostgresCatalogueInstaller () {};
	
		virtual void installMRQuery() ;
		virtual void installMRAttList() ;
		virtual void installMREqKeys() ;
		virtual void installMREqKeysCol() ;
		virtual void installMRDepFun() ;
		virtual void installMRDepFunCol() ;
		virtual void installMRAutoincrement() ;
		virtual void initializeAutoincrement() ;	

		virtual void dropMRQuery() ;
		virtual void dropMRAttList() ;
		virtual void dropMREqKeys() ;
		virtual void dropMREqKeysCol() ;
		virtual void dropMRDepFun() ;
		virtual void dropMRDepFunCol() ;
		virtual void dropMRAutoincrement() ;

	};
	
} /* minerule */

#endif /* end of include guard: POSTGRESCATALOGUEINSTALLER_H_M0QRV1OU */

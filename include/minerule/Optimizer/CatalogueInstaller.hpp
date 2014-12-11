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
#ifndef CATALOGUEINSTALLER_H_1YBJZ8TY
#define CATALOGUEINSTALLER_H_1YBJZ8TY

#include "minerule/mrdb/Connection.hpp"
#include "minerule/mrdb/Statement.hpp"
#include "minerule/Utils/MineruleException.hpp"

namespace minerule {
	class CatalogueInstaller {
	protected:
		mrdb::Connection* _connection;
		mrdb::Statement* _statement;
	public:
		typedef enum { MySql, Postgres } SupportedDbms;

		CatalogueInstaller();
		virtual ~CatalogueInstaller() {
			delete _statement;
		};

		static CatalogueInstaller* newInstaller(SupportedDbms dbms);
		static CatalogueInstaller* newInstaller();

		virtual void installMRQuery() 			= 0;
		virtual void installMRAttList() 		= 0;
		virtual void installMREqKeys() 			= 0;
		virtual void installMREqKeysCol() 		= 0;
		virtual void installMRDepFun() 			= 0;
		virtual void installMRDepFunCol() 		= 0;
		virtual void installMRAutoincrement() 	= 0;
		virtual void initializeAutoincrement() 	= 0;

		virtual void dropMRQuery() 			= 0;
		virtual void dropMRAttList() 		= 0;
		virtual void dropMREqKeys() 		= 0;
		virtual void dropMREqKeysCol() 		= 0;
		virtual void dropMRDepFun() 		= 0;
		virtual void dropMRDepFunCol() 		= 0;
		virtual void dropMRAutoincrement() 	= 0;


		virtual void install() {
			uninstall();

			installMRQuery();
			installMRAttList();
			installMREqKeys();
			installMREqKeysCol();
			installMRDepFun();
			installMRDepFunCol();
			installMRAutoincrement();
			initializeAutoincrement();
		}

		virtual void uninstall() {
			dropMRQuery();
			dropMRAttList();
			dropMREqKeys();
			dropMREqKeysCol();
			dropMRDepFun();
			dropMRDepFunCol();
			dropMRAutoincrement();
		}
	};
}
#endif /* end of include guard: CATALOGUEINSTALLER_H_1YBJZ8TY */

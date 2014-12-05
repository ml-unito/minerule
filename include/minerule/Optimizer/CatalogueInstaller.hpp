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

		virtual void installMRQuery() 			throw(MineruleException, mrdb::SQLException) = 0;
		virtual void installMRAttList() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void installMREqKeys() 			throw(MineruleException, mrdb::SQLException) = 0;
		virtual void installMREqKeysCol() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void installMRDepFun() 			throw(MineruleException, mrdb::SQLException) = 0;
		virtual void installMRDepFunCol() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void installMRAutoincrement() 	throw(MineruleException, mrdb::SQLException) = 0;
		virtual void initializeAutoincrement() 	throw(MineruleException, mrdb::SQLException) = 0;

		virtual void dropMRQuery() 			throw(MineruleException, mrdb::SQLException) = 0;
		virtual void dropMRAttList() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void dropMREqKeys() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void dropMREqKeysCol() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void dropMRDepFun() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void dropMRDepFunCol() 		throw(MineruleException, mrdb::SQLException) = 0;
		virtual void dropMRAutoincrement() 	throw(MineruleException, mrdb::SQLException) = 0;


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

		virtual void uninstall() throw( MineruleException, mrdb::SQLException ) {
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
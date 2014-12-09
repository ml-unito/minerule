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
#include "minerule/Optimizer/CatalogueInstaller.hpp"
#include "minerule/Optimizer/Installers/MySqlCatalogueInstaller.hpp"
#include "minerule/Optimizer/Installers/PostgresCatalogueInstaller.hpp"
#include "minerule/Utils/MineruleOptions.hpp"

namespace minerule {
	
	CatalogueInstaller::CatalogueInstaller() : _connection( MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection() ), _statement( _connection->createStatement() ) {}
	
	CatalogueInstaller* CatalogueInstaller::newInstaller(SupportedDbms dbms) {
		switch(dbms) {
		case MySql:
			return new MySqlCatalogueInstaller();
		case Postgres:
			return new PostgresCatalogueInstaller();
		}
	}
	
	CatalogueInstaller* CatalogueInstaller::newInstaller() {
		std::string dbmsStr = MineruleOptions::getSharedOptions().getMRDB().getDBMS();

		if(dbmsStr == "mysql") return newInstaller(MySql);
		if(dbmsStr == "postgres") return newInstaller(Postgres);
		
		throw MineruleException(MR_ERROR_OPTION_CONFIGURATION,std::string("Option mrdb::dbms is set to an unsupported value.") +
			" Current value is:"+dbmsStr+ ". Supported values are 'postgres' and 'mysql'.");
	}

	
}

#include "Optimizer/CatalogueInstaller.h"
#include "Optimizer/Installers/MySqlCatalogueInstaller.h"
#include "Optimizer/Installers/PostgresCatalogueInstaller.h"
#include "Utils/MineruleOptions.h"

namespace minerule {
	
	CatalogueInstaller::CatalogueInstaller() : _connection( MineruleOptions::getSharedOptions().getODBC().getODBCConnection() ), _statement( _connection->createStatement() ) {}
	
	CatalogueInstaller* CatalogueInstaller::newInstaller(SupportedDbms dbms) {
		switch(dbms) {
		case MySql:
			return new MySqlCatalogueInstaller();
		case Postgres:
			return new PostgresCatalogueInstaller();
		}
	}
}

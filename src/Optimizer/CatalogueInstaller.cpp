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
	
	CatalogueInstaller* CatalogueInstaller::newInstaller() {
		std::string dbmsStr = MineruleOptions::getSharedOptions().getODBC().getDBMS();

		if(dbmsStr == "mysql") return newInstaller(MySql);
		if(dbmsStr == "postgres") return newInstaller(Postgres);
		
		throw MineruleException(MR_ERROR_OPTION_CONFIGURATION,std::string("Option odbc::dbms is set to an unsupported value.") +
			" Current value is:"+dbmsStr+ ". Supported values are 'postgres' and 'mysql'.");
	}

	
}

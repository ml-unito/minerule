#ifndef CATALOGUEINSTALLER_H_1YBJZ8TY
#define CATALOGUEINSTALLER_H_1YBJZ8TY

#include "odbc++/connection.h"
#include "odbc++/statement.h"
#include "Utils/MineruleException.h"

namespace minerule {
	class CatalogueInstaller {
	protected:
		odbc::Connection* _connection;
		odbc::Statement* _statement;
	public:
		typedef enum { MySql, Postgres } SupportedDbms;
	
		CatalogueInstaller();
		virtual ~CatalogueInstaller() {
			delete _statement;
		};
	
		static CatalogueInstaller* newInstaller(SupportedDbms dbms);
		static CatalogueInstaller* newInstaller();
	
		virtual void installMRQuery() 			throw(MineruleException, odbc::SQLException) = 0;
		virtual void installMRAttList() 		throw(MineruleException, odbc::SQLException) = 0;	
		virtual void installMREqKeys() 			throw(MineruleException, odbc::SQLException) = 0;
		virtual void installMREqKeysCol() 		throw(MineruleException, odbc::SQLException) = 0;
		virtual void installMRDepFun() 			throw(MineruleException, odbc::SQLException) = 0;
		virtual void installMRDepFunCol() 		throw(MineruleException, odbc::SQLException) = 0;
		virtual void installMRAutoincrement() 	throw(MineruleException, odbc::SQLException) = 0;
		virtual void initializeAutoincrement() 	throw(MineruleException, odbc::SQLException) = 0;

		virtual void dropMRQuery() 			throw(MineruleException, odbc::SQLException) = 0;
		virtual void dropMRAttList() 		throw(MineruleException, odbc::SQLException) = 0;	
		virtual void dropMREqKeys() 		throw(MineruleException, odbc::SQLException) = 0;
		virtual void dropMREqKeysCol() 		throw(MineruleException, odbc::SQLException) = 0;
		virtual void dropMRDepFun() 		throw(MineruleException, odbc::SQLException) = 0;
		virtual void dropMRDepFunCol() 		throw(MineruleException, odbc::SQLException) = 0;
		virtual void dropMRAutoincrement() 	throw(MineruleException, odbc::SQLException) = 0;

		
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
		
		virtual void uninstall() throw( MineruleException, odbc::SQLException ) {
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

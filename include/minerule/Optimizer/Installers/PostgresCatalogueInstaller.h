#ifndef POSTGRESCATALOGUEINSTALLER_H_M0QRV1OU
#define POSTGRESCATALOGUEINSTALLER_H_M0QRV1OU

#include "Optimizer/CatalogueInstaller.h"

namespace minerule {
	class PostgresCatalogueInstaller : public CatalogueInstaller {
	public:
		PostgresCatalogueInstaller() : CatalogueInstaller() {}
		virtual ~PostgresCatalogueInstaller () {};
	
		virtual void installMRQuery() throw(MineruleException, odbc::SQLException);
		virtual void installMRAttList() throw(MineruleException, odbc::SQLException);
		virtual void installMREqKeys() throw(MineruleException, odbc::SQLException);
		virtual void installMREqKeysCol() throw(MineruleException, odbc::SQLException);
		virtual void installMRDepFun() throw(MineruleException, odbc::SQLException);
		virtual void installMRDepFunCol() throw(MineruleException, odbc::SQLException);
		virtual void installMRAutoincrement() throw(MineruleException, odbc::SQLException);
		virtual void initializeAutoincrement() throw(MineruleException, odbc::SQLException);	

		virtual void dropMRQuery() throw(MineruleException, odbc::SQLException);
		virtual void dropMRAttList() throw(MineruleException, odbc::SQLException);
		virtual void dropMREqKeys() throw(MineruleException, odbc::SQLException);
		virtual void dropMREqKeysCol() throw(MineruleException, odbc::SQLException);
		virtual void dropMRDepFun() throw(MineruleException, odbc::SQLException);
		virtual void dropMRDepFunCol() throw(MineruleException, odbc::SQLException);
		virtual void dropMRAutoincrement() throw(MineruleException, odbc::SQLException);

	};
	
} /* minerule */

#endif /* end of include guard: POSTGRESCATALOGUEINSTALLER_H_M0QRV1OU */

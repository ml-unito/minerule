#ifndef MYSQLCATALOGUEINSTALLER_H_3QIF6B9O
#define MYSQLCATALOGUEINSTALLER_H_3QIF6B9O

#include "minerule/Optimizer/Installers/PostgresCatalogueInstaller.h"

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

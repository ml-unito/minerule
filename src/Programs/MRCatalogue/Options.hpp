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
#ifndef OPTIONS_H_C27RB72E
#define OPTIONS_H_C27RB72E
	
#include "Exception.hpp"
#include "minerule/Optimizer/CatalogueInstaller.hpp"

namespace mrc {
	
	class Options {
	public:
		typedef enum {NoCommand,ShowList,SearchQry,DeleteQry, CheckCatalogue, InstallCatalogue, UninstallCatalogue, AddDerivedQuery} Command;

		class ListFormat {
		public:
			bool size;
			bool text;
			bool result;

			ListFormat() : size(false),	text(false), result(false) {}
		};

		typedef enum {QryName=0,LastParam} QryParams;

	protected:
		Command command;

		std::string searchParam[LastParam];
		std::string originalQuery;
		std::string derivedQuery;
		
		ListFormat listFormat;
	public:

		Options():	command(NoCommand)  {  }

		void setListFormat(const char* format) throw (Exception);

		void setShowList() {
			if(command!=NoCommand && command!=ShowList) {
				throw Exception( mrc::ERROR_OPTION_PARSING,	"You can specify either -l or -n option, but not both!" );
			}

			command = ShowList;
		}

		void setSearchQry() {
			if(command!=NoCommand && command!=SearchQry) {
				throw Exception( mrc::ERROR_OPTION_PARSING, "You can specify either -l or -n option, but not both!" );
			}

			command = SearchQry;
		}

		void setDeleteQry() {
			if(command!=NoCommand && command!=DeleteQry) {
				throw Exception( mrc::ERROR_OPTION_PARSING, "Too many different options have been given, check out the parameters");
			}

			command = DeleteQry;
		}
		
		void setCheckCatalogue() {
			if(command!=NoCommand && command != CheckCatalogue) {
				throw Exception( mrc::ERROR_OPTION_PARSING, "Too many different options have been given, check out the parameters");
			}
			
			command = CheckCatalogue;
		}
		
		void setInstallCatalogue() {
			if(command!=NoCommand && command != InstallCatalogue) {
				throw Exception( mrc::ERROR_OPTION_PARSING, "Too many different options have been given, check out the parameters");				
			}
			
			command = InstallCatalogue;	
		}
		
		void setUninstallCatalogue() {
			if(command!=NoCommand && command != UninstallCatalogue) {
				throw Exception( mrc::ERROR_OPTION_PARSING, "Too many different options have been given, check out the parameters");				
			}
			
			command = UninstallCatalogue;			
		}
		
		void setSearchParam(QryParams qryParam, const std::string& param) {
			searchParam[qryParam]=param;
		}

		const std::string& getSearchParam(QryParams qryParam) const {
			return searchParam[qryParam];
		}
		
		void setAddDerivedQuery(const std::string& original, const std::string& derived) {
			command = AddDerivedQuery;
			originalQuery = original;
			derivedQuery = derived;
		}
		
		const std::string& getOriginalQuery() const { 
			return originalQuery;
		}
		
		const std::string& getDerivedQuery() const {
			return derivedQuery;
		}

		Command getCommand() const {
			return command;
		}

		const ListFormat& getListFormat() const {
			return listFormat;
		}
	};

}; // namespace mrc

#endif /* end of include guard: OPTIONS_H_C27RB72E */

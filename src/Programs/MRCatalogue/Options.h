#ifndef OPTIONS_H_C27RB72E
#define OPTIONS_H_C27RB72E
	
#include "Exception.h"
#include "Optimizer/CatalogueInstaller.h"

namespace mrc {
	
	class Options {
	public:
		typedef enum {NoCommand,ShowList,SearchQry,DeleteQry, CheckCatalogue, InstallCatalogue, UninstallCatalogue} Command;

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
		ListFormat listFormat;
		std::string sepString; // the string that should be used to separate output elements
	public:

		Options():	command(NoCommand), sepString(" ")  {  }

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

		void setSepString(const std::string& s) {
			sepString=s;
		}

		const std::string& getSepString() const {
			return sepString;
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

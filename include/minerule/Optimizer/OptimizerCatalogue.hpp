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
#ifndef __OPTIMIZER_CATALOGUE_H__
#define __OPTIMIZER_CATALOGUE_H__


#include <set>
#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "minerule/mrdb/Types.hpp"
#include "minerule/Utils/MineruleException.hpp"
#include "minerule/Parsers/ParsedMinerule.hpp"
#include "minerule/Database/ItemType.hpp"
#include "minerule/Result/QueryResult-header.hpp"
#include "minerule/Optimizer/CatalogueInstaller.hpp"




/**
 * TODO:
 *
 * 1) Memorizzare da qualche parte (ParsedMinerule?) le sostituzioni
 * effettuate sulla rule attribute delle minerule processate.
 * Servira' per poi "tornare" indietro quando restituiremo i
 * risultati.
 *
 * 2) Eliminare la cal quando inclusa nella gal. Le eventuali cluster
 * conditions, vengono poste in and con le mining cond. (nota: bisogna
 * ri-eseguire
 */

namespace minerule {

  class CatalogueInfo {
  public:
    std::string qryName;
    std::string qryText;
	std::string resName;
    std::vector<std::string> resTables; // name of the tables used by the result
    size_t resSize;

    void updateQrySize() ;
  };


  class OptimizerCatalogue {
    /* ---------- Public types ---------- */
  public:
    typedef enum { EQKEYS, DEPFUN } CatalogueType;
    typedef enum { Equal, Reversed, None } OrderType;

    // a KeyCols instance will store the name of columns that can
    // be grouped to form a key
    typedef std::set<std::string> KeyCols;

    // A CatalogueEntry is a mapping originalAttrList->substAttrList
    // with the additional information about the type of ordering that
    // exists between riginalAttrList and substAttrList
    typedef std::multimap< KeyCols,std::pair<KeyCols,OrderType> > CatalogueEntry;

    // A catalogue is a mapping tableName->CatalogueEntry
	class Catalogue : public std::map<std::string, CatalogueEntry> {
	public:
		Catalogue() : std::map<std::string, CatalogueEntry>() {}
		Catalogue(const Catalogue& catalogue) :
		map<std::string, CatalogueEntry>(catalogue) {}
		virtual ~Catalogue() {}

		void insertMapping(const std::string& table,const KeyCols& origKeyCols,int refKeyId,OrderType orderType) ;
		static OrderType stringToOrder(const std::string&);
		void initialize();
		virtual const std::string& getSchemaInfo(const std::string& schemaKey) const=0;
	};

    class EqKeysCatalogue : public Catalogue {
      std::map<std::string,std::string> schema;
    public:
      EqKeysCatalogue() : Catalogue() {
		  schema["tab_main"]="mr_eq_keys";
		  schema["tab_main_tab_name"]="tab_name";
		  schema["tab_main_lhs_key_id"]="key_id";
		  schema["tab_main_rhs_key_id"]="ref_key_id";
		  schema["tab_main_order_type"]="order_type";
		  schema["tab_cols"]="mr_eq_keys_col";
		  schema["tab_cols_col_name"]="col_name";
		  schema["tab_cols_key_id"]="key_id";
		  initialize();
	  }

      EqKeysCatalogue(const EqKeysCatalogue& catalogue) : Catalogue(catalogue), schema(catalogue.schema) { }
      virtual ~EqKeysCatalogue() {}

      virtual const std::string& getSchemaInfo(const std::string& schemaKey) const {
		  std::map<std::string,std::string>::const_iterator it;
		  it=schema.find(schemaKey);
		  if(it==schema.end())
			  throw MineruleException( MR_ERROR_CATALOGUE_ERROR,
				  "Requested for the unknown schema key:"+schemaKey);
		  return it->second;
      }
    };

    class DepFunCatalogue : public Catalogue {
      std::map<std::string,std::string> schema;
    public:
      DepFunCatalogue() : Catalogue() {
		  schema["tab_main"]="mr_dep_fun";
		  schema["tab_main_tab_name"]="lhs_tab_name";
		  schema["tab_main_lhs_key_id"]="lhs_att_list_id";
		  schema["tab_main_rhs_key_id"]="rhs_att_list_id";
		  schema["tab_main_order_type"]="order_type";
		  schema["tab_cols"]="mr_dep_fun_col";
		  schema["tab_cols_col_name"]="col_name";
		  schema["tab_cols_key_id"]="col_id";
		  initialize();
      }

      DepFunCatalogue(const DepFunCatalogue& catalogue) :
	Catalogue(catalogue), schema(catalogue.schema) { }

      virtual ~DepFunCatalogue() {}
      virtual const std::string& getSchemaInfo(const std::string& schemaKey) const {
	std::map<std::string,std::string>::const_iterator it;
	it=schema.find(schemaKey);
	if(it==schema.end())
	  throw MineruleException( MR_ERROR_CATALOGUE_ERROR,
				   "Requested for the unknown schema key:"+schemaKey);
	return it->second;
      }
    };

    // Input of addMineruleResult. We use a class since it is likely
    // that the input of this function will vary as more optimization
    // politics are implemented. In this way the interface of the
    // function will stay the same while we add functionality
    // to the system.
    class MineruleResultInfo : public ParsedMinerule {
    public:
		std::string resultset;

		MineruleResultInfo(const ParsedMinerule& mr) : ParsedMinerule(mr), resultset(mr.tab_result) {};
		MineruleResultInfo(const MineruleResultInfo& mri) : ParsedMinerule(mri), resultset(mri.resultset) {};
	};

  private:
    EqKeysCatalogue eqKeysCatalogue;
    DepFunCatalogue depFunCatalogue;

	static void setMRQueryInfoFromResultSet( mrdb::ResultSet* rs, CatalogueInfo& info, bool includeResultSize );
  public:
    /* ---------- Public Methods ---------- */

    //void addEquivalentKey( const KeyCols&, const KeyCols& );
    OptimizerCatalogue() {
      /*     eqKeysCatalogue.initialize();
	     depFunCatalogue.initialize();*/
    }

    const Catalogue& getCatalogue(CatalogueType ct=EQKEYS) const {
		switch( ct ) {
			case EQKEYS:
				return eqKeysCatalogue;
			case DEPFUN:
				return depFunCatalogue;
			default:
				throw MineruleException(MR_ERROR_CATALOGUE_ERROR, "Requests for an unknown catalogue type!");
		}
    }

    bool isIDAttribute(const std::string& tableName,
		       const std::vector<std::string>& itemCols,
		       const std::string& attribute) const ;


   /**
    * The method returns true if the constraints in the query
    * are item dependent. Notice that, cross predicates are
    * implicitly context dependent, even if the attributes depends
    * on the item.
    */
		static bool hasIDConstraints(const ParsedMinerule& mineruel) ;


    // ==================== STATIC METHODS ====================

	/**
	 * @return true if the catalogue tables appear to be present.
	 */
	static bool checkInstallation();
    static void install() ;
    static void uninstall() ;


    static void addMineruleResult( const MineruleResultInfo& mri ) ;
	static void addDerivedResult(const std::string& original, const std::string derived) ;
    static std::string addMineruleAttributeList(const ParsedMinerule::AttrVector& l) ;
    static std::string getNewAutoincrementValue(const std::string& tableName) ;

    /**
     * @param queryname
     * @return astd::string containing the name of the result set of the
     *  given query. It throws a MineruleException(MR_ERROR_CATALOGUE_ERROR,"...")
     *  in case queryname is not in the catalogue.
     */
    static std::string getResultsetName(const std::string& queryname) ;

    /**
     * @param mrname
     * @return true if a query named ``mrname'' has been executed and a result
     *   can be found in the catalogue. it returns false otherwise.
     */
    static bool existsMinerule(const std::string& mrname);

    static void deleteMinerule(const std::string& mrname)
      ;

	/**
	 * Returns the name of the i-th query in the catalogue.
	 */
	static std::string getMRQueryName(size_t i) ;

    /**
     * The procedure fills the vector nameVec with the names of all
     * queries that have been executed.
     */
    static void getMRQueryNames(std::vector<std::string>& nameVec)
      ;

    /**
     * It behaves as getMRQueryNames, but provide more information to the
     * caller.
     */
    static void getMRQueryInfos(std::vector<CatalogueInfo>& catInfoVec, bool includeResultSize=true)
      ;

    /**
     * As getMRQueryInfos, but the user specify a single qryName to be searched.
     */
    static void getMRQueryInfo(const std::string& qryName, CatalogueInfo& catInfo, bool includeResultSize=true) ;

    /**
     * It updates the QueryResultIterator so that it iterates through the
     * results of query qryName
     * @param qryName the name of the query whose results the user want to
     *  iterate through
     * @param it a user allocated QueryResultIterator which will be initialized
     *        in this function.
     * @param sup Support threshold to be used to filter queries (the default is
     *            -1 and it means that no filter is needed)
     * @param con Confidence threshold to be used to filter queries (the default is
     *            -1 and it means that no filter is needed)
     */
    static void getMRQueryResultIterator(const std::string& qryName,
					 QueryResult::Iterator& it,
					 double sup=-1,
					 double con=-1)
      ;
  };


} // namespace

#endif

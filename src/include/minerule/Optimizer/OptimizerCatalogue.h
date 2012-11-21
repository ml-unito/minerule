#ifndef __OPTIMIZER_CATALOGUE_H__
#define __OPTIMIZER_CATALOGUE_H__


#include <set>
#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <odbc++/types.h>
#include "Utils/MineruleException.h"
#include "Parsers/ParsedMinerule.h"
#include "Database/ItemType.h"
#include "Database/QueryResult-header.h"




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

    void updateQrySize() throw( odbc::SQLException, MineruleException );
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

		void insertMapping(const std::string& table,const KeyCols& origKeyCols,int refKeyId,OrderType orderType) throw(MineruleException);
		static OrderType stringToOrder(const std::string&);
		void initialize() throw (MineruleException);
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

	static void setMRQueryInfoFromResultSet( odbc::ResultSet* rs, CatalogueInfo& info, bool includeResultSize );
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
		       const std::string& attribute) const throw(MineruleException);
    

    // ==================== STATIC METHODS ====================


    static void addMineruleResult( const MineruleResultInfo& mri ) throw(odbc::SQLException, MineruleException);
    static std::string addMineruleAttributeList(const ParsedMinerule::AttrVector& l) throw(odbc::SQLException, MineruleException);
    static std::string getNewAutoincrementValue(const std::string& tableName) throw(odbc::SQLException, MineruleException);

    /**
     * @param queryname
     * @return astd::string containing the name of the result set of the 
     *  given query. It throws a MineruleException(MR_ERROR_CATALOGUE_ERROR,"...")
     *  in case queryname is not in the catalogue.
     */
    static std::string getResultsetName(const std::string& queryname) throw(odbc::SQLException, MineruleException);

    /**
     * @param mrname
     * @return true if a query named ``mrname'' has been executed and a result
     *   can be found in the catalogue. it returns false otherwise.
     */
    static bool existsMinerule(const std::string& mrname);

    static void deleteMinerule(const std::string& mrname) 
      throw(MineruleException, odbc::SQLException);

    /**
     * The procedure fills the vector nameVec with the names of all
     * queries that have been executed.
     */
    static void getMRQueryNames(std::vector<std::string>& nameVec) 
      throw(odbc::SQLException, MineruleException);

    /**
     * It behaves as getMRQueryNames, but provide more information to the
     * caller.
     */
    static void getMRQueryInfos(std::vector<CatalogueInfo>& catInfoVec, bool includeResultSize=true)
      throw(odbc::SQLException, MineruleException);

    /**
     * As getMRQueryInfos, but the user specify a single qryName to be searched.
     */
    static void getMRQueryInfo(const std::string& qryName, CatalogueInfo& catInfo, bool includeResultSize=true) throw(odbc::SQLException, MineruleException);

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
      throw(odbc::SQLException, MineruleException);
  };


} // namespace

#endif

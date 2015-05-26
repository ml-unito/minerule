#ifndef STSMiner_H
#define STSMiner_H

#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Database/MRResultSet.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"
#include "minerule/Utils/Converter.hpp"
#include "minerule/Utils/AlgorithmTypes.hpp"
#include "minerule/Utils/Bitstring.hpp"
#include "minerule/Utils/StringUtils.hpp"
#include <fstream>
#include <locale>
#include <map>
#include <math.h>
#include <syscall.h>
#include <unistd.h>

namespace minerule {

typedef std::map<long, int> GroupMap;
typedef std::pair<long, int> GroupPair;

struct Attr_def {
    std::string attr_name;
    std::string attr_type;

    Attr_def(std::string n, std::string t): attr_name(n), attr_type(t) { }
};

/**
  Struct to manage context_dependent BEM constraints
  */
struct Constraints {
    std::vector<BitString> bem;
    std::vector<std::vector<std::pair<int,int> > > mid_counters;

    void clear() {
        bem.clear();
        mid_counters.clear();
    }

    Constraints() {}
};

class STSMiner : public MiningAlgorithm{

private:
    int seq_count;
    AlgorithmsOptions options;
    ParsedMinerule pm;

    std::string groupAttrList;
    std::string ordAttrList;
    std::string bodyAttrList;
    std::vector<Attr_def> attr_types;
    std::vector<std::string> attr_list;
    GroupMap hashtable;
    std::vector<std::vector<std::string> > hash_vect;
    std::vector<BitString*> bem_vect;
    std::vector<std::vector<Constraints*> >  constr_vect;
    std::vector<std::vector<Constraints*> >  constr_vect_singleton;
    std::vector<BitString*> bvptr;
    std::vector<MinMaxPair> min_max_mid_count;
    std::vector<std::vector<std::pair<int,int> > > mid_count;
    std::vector<std::vector<std::string> > results;
    std::vector<std::vector<std::vector<int> > > prefix_pos_lists;//[id-1][pos_list_entry]
    std::vector<std::vector<std::pair<int,int>* > > pos_lists;
    std::vector<std::vector<std::pair<int,int>* > > pos_lists_singleton;
    std::vector<std::vector<int> > pos_d_lists;

    bool context_dep_dist;
    bool context_dep_BEM;
    bool context_free_BEM;
    bool active_count;
    bool DISTINCT;
    bool log;
    int max_MIN_COUNT;
    int grpId_count;//number of values found for group by attribute in source table
    int bemCD_constraints;
    int bemCF_constraints;
    int bvptr_length;
    int min_trackid;
    int max_trackid;
    int bem_vect_length;
    int max_seq_length;
    int min_seq_length;

    std::ofstream matcher_cache;
    std::ofstream seqicache;

    bool extractKSequences(int k);
    bool extract2Sequences();
    bool extract1Sequences();
    std::string union_rows();
    void createBitVectors();
    void createBitVectors_context_dep();
    void saveSequenceData(int k,int& id1, int& id2, bool check_bem,
                          BitString tmp0,
                          std::vector<std::pair<int,int>* > ris_l,
                          std::vector<int> pos_d_l,
                          BitString* new_bem_string,
                          std::vector<bool>* closed_seq,
                          std::vector<std::vector<int> > prefix_pos_l,
                          std::vector<std::pair<int,int> > new_mid_count_vect,
                          std::vector<Constraints*> c_v);
    Constraints* init_c_v(std::vector<std::string> AttrList, bool* check_bem_cd);
    std::vector<std::vector<std::string> > init_dist_val_vect(std::vector<std::string> AttrList);
    BitString initbemvect_context_dep(std::vector<std::string> bodyAttrVal);
    void initbemvect(int id,std::vector<std::string> attrVal);
    bool checkBem(int id, int k);
    int pruning1Seq();
    bool pruningSeq2(std::string sqlQuery);
    bool pruningSeqK(int k,std::string sqlQuery);
    void updateMidCounters_singleton(Constraints* c);
    void updateMidCounters(Constraints* constr,int id1,int id2, int i, int j);
    bool check_pos_lists_with_gap(int id1, int id2,
                                  std::vector<std::pair<int,int>* >* ris_l,
                                  BitString* bv_constr,
                                  std::vector<std::vector<int> >* new_prefix_pos_list,
                                  std::vector<int>* new_pos_d_l,
                                  bool* BEM_context_free,
                                  std::vector<Constraints*>* c_v,
                                  BitString* bem_cd);
    bool sourceFilter();
    void check_go_on(Constraints* c,BitString* ris);
    int findIndex(int id, int pos, int grpId);
    bool init_bemCD(int id1, int index_id1, int id2, int pos2, int grpId, Constraints* output,BitString* bem_cd);
    BitString inferBemCD(BitString bs1,BitString bs2);
    BitString inferBem(int id1,int id2,int i, int j);
    void init_bem_bit_vector(int k,int id1,int id2,bool* check_bem,
                             BitString* bem_cf,
                             BitString* new_bem_vect,
                             std::vector<std::pair<int,int> >* new_count);
    bool checkMidCounters(std::vector<std::pair<int,int> >mc_v);
    void updateBemData(Constraints* constr,int id1,int id2,int prefix_pos,
                                 int pos,int grp,bool* check_bem_cd,BitString* bem_cd);
    void saveSequences(int k);

    //clean memory functions
    void delete_all(int k);
    void dropResultsTables(int k);
    void resetStructs(int k,
                                int size_before_results,
                                int size_before_bvptr,
                                int size_before_pos,
                                int size_before_prefix_pos,
                                int size_before_bem_vect,
                                int size_before_mid_count,
                                int size_before_pos_d,
                                int size_before);

public:
    STSMiner(const OptimizedMinerule& mr, const AlgorithmsOptions& opts) :
        MiningAlgorithm(mr),pm(ParsedMinerule(mr.getParsedMinerule())) {
        MRDebug()<<"STSMiner initialization..."<<std::endl;
       /* double vm, rss;
        process_mem_usage(vm, rss);
        std::cout << "VM: " << vm << "; RSS: " << rss << std::endl;*/

        this->initialize();
        connection.createResultTables();

        std::string temp = "/tmp/" + pm.tab_result + ".s";
        seqicache.open(temp.c_str(), std::ofstream::trunc);

        temp = "/tmp/" + pm.tab_result + ".m";
        matcher_cache.open(temp.c_str(), std::ofstream::trunc);

        options = opts;
        log=true;
        max_seq_length = pm.length.getMax();
        min_seq_length = pm.length.getMin();
        active_count=false;

        groupAttrList = "";
        ordAttrList = "";
        bodyAttrList = "";

        for(int i=0; i<pm.ga.size()-1; ++i)
            groupAttrList+=pm.ga[i]+",";
        groupAttrList+=pm.ga[pm.ga.size()-1];

        for(int i=0; i<pm.oa.size()-1; ++i)
            ordAttrList+=pm.oa[i]+",";
        ordAttrList+=pm.oa[pm.oa.size()-1];

        for(int i=0; i<pm.ba.size()-1; ++i)
            bodyAttrList+=pm.ba[i]+",";
        bodyAttrList+=pm.ba[pm.ba.size()-1];

        seq_count=1;
        DISTINCT=pm.distinct;
        context_dep_dist=false;
        context_dep_BEM=false;
        context_free_BEM=false;
        max_MIN_COUNT=1;
        bemCD_constraints=0;
        bemCF_constraints=0;

        initAttrNameList();

        for(int i=0;i<pm.seq_bem_vect.size();++i) {
            Bem_cond* bc = pm.seq_bem_vect[i];
            bool cf=false;
            bool cd=false;

            do {
                if(!containedIn(bc->attr,pm.ba)) {
                    context_dep_BEM=true;
                    cd=true;
                }
                else {
                    context_free_BEM=true;
                    cf=true;
                }
                bc=bc->and_c;
            }while(bc);
            if(cd)
                bemCD_constraints++;
            if(cf)
                bemCF_constraints++;
        }

        for(int i=0;i<pm.seq_dist_vect.size();++i) {
            for(int j=0; j<pm.seq_dist_vect[i]->attr.size();++j) {
                if(!containedIn(pm.seq_dist_vect[i]->attr[j],pm.ba))
                    context_dep_dist=true;
            }
        }

        int x=0;
        for(int i=0;i<pm.seq_bem_vect.size();++i)
            if(pm.seq_bem_vect[i]->type.compare("MID")==0)
                x++;
        bem_vect_length= 3 >= 2+x ? 3 : 2+x;

        if(bem_vect_length>0) {

            int max_int_val=std::numeric_limits<int>::max();
            for(int i=1;i<bem_vect_length-1;++i)
                min_max_mid_count.push_back(MinMaxPair(1,max_int_val));
            x=0;
            for(int i=0;i<pm.seq_bem_vect.size();++i) {
                if(pm.seq_bem_vect[i]->type.compare("MID")==0) {
                    Bem_cond* temp=pm.seq_bem_vect[i];
                    do {
                        max_MIN_COUNT=std::max(max_MIN_COUNT,temp->count_min);
                        min_max_mid_count[x].setMin(std::max(min_max_mid_count[x].getMin(),temp->count_min));
                        min_max_mid_count[x].setMax(std::min(min_max_mid_count[x].getMax(),temp->count_max));
                        temp = temp->and_c;
                    } while(temp);

                    active_count= active_count || min_max_mid_count[x].getMin()!=1
                         || min_max_mid_count[x].getMax()!=std::numeric_limits<int>::max();
                    ++x;
                }
            }
        }
        initAttrTypesList(pm.tab_source);
    }

    virtual bool canHandleMinerule() const {
      return pm.miningTask == MTMineSequences;
    }

    virtual void mineRules();

    virtual ~STSMiner() { }

    virtual SourceTableRequirements sourceTableRequirements() const {
            return SourceTableRequirements(SourceTableRequirements::SortedGids);
    }


private:

    void initAttrNameList() {
        attr_list.clear();
        for(int i=0;i<pm.ga.size();++i)
            attr_list.push_back(pm.ga[i]);

        for(int i=0;i<pm.ba.size();++i)
            if(!containedIn(pm.ba[i],attr_list))
                attr_list.push_back(pm.ba[i]);
        for(int i=0;i<pm.seq_bem_vect.size();++i) {
            Bem_cond* bc = pm.seq_bem_vect[i];
            do {
                if(!containedIn(bc->attr,attr_list))
                    attr_list.push_back(bc->attr);
                bc=bc->and_c;
            }while(bc);
        }
        for(int i=0;i<pm.seq_dist_vect.size();++i)
            for(int j=0; j<pm.seq_dist_vect[i]->attr.size();++j)
                if(!containedIn(pm.seq_dist_vect[i]->attr[j],attr_list))
                    attr_list.push_back(pm.seq_dist_vect[i]->attr[j]);
    }

    void initAttrTypesList(std::string src) {
        attr_types.clear();
        std::string sqlQuery = "SELECT "+toString(attr_list)+" FROM " + src + " LIMIT 1";
        mrdb::ResultSet* rs = execQr(sqlQuery);
        mrdb::ResultSetMetaData* meta = rs->getMetaData();

        if(rs->next()) {
            for(int i=0; i<attr_list.size();++i) {
                Attr_def tmp(attr_list[i],meta->getColumnTypeName(i+1));
                attr_types.push_back(tmp);
            }
        }
        else exit(1);
        delete rs;
    }

    std::string find_val(std::string attr_name,std::vector<std::string> AttrVal){

        for(int i = 0; i < attr_list.size() ; ++i)
            if(attr_list[i].compare(attr_name) == 0)
                return AttrVal[i];
        return "";
    }


    std::string pos_list_string(std::vector<std::pair<int,int>* > pos_l) {
        std::string str = "";
        for(std::vector<std::pair<int, int>* >::iterator it = pos_l.begin();
            it != pos_l.end(); ++it) {
            std::pair<int, int>* pair = *it;
            str += "(" + Converter(pair->first).toString()+ ","
                + Converter(pair->second).toString() + ") - ";
        }
        return str;
    }

    static std::string toString(std::vector<std::string> v) {
        std::string ris="";
        if(v.size()>0) {
            ris += v[0];
            for(int i=1;i<v.size();i++)
                ris += ","+v[i];
        }
        return ris;
    }

    std::string get_type(std::string attr_n){
        for(int i=0;i<attr_types.size();++i)
            if(attr_n.compare(attr_types[i].attr_name)==0)
                return attr_types[i].attr_type;
        return (std::string)"unknown";
    }

    bool textAttr(std::string attr) {
        std::string type= get_type(attr);
        return (type.compare("varchar")==0 || type.compare("VARCHAR")==0 || type.compare("text")==0 || type.compare("TEXT")==0);
    }

    bool containedIn(std::string el,std::vector<std::string> list){
        for(int i=0;i<list.size();++i)
            if(list[i].compare(el)==0)
                return true;
        return false;
    }

    mrdb::ResultSet* execQr(std::string sqlQuery) {

      MRDebug() << "Executing query: " + sqlQuery << std::endl;

      mrdb::Statement *statement = connection.getMRDBConnection()->createStatement();
      mrdb::ResultSet *result = statement->executeQuery(sqlQuery.c_str());

      return result;
    }

    void execQuery(std::string sqlQuery) {

      MRDebug() << "Executing statement: " + sqlQuery << std::endl;

      mrdb::Statement *statement =
          connection.getMRDBConnection()->createStatement();
      statement->execute(sqlQuery);

      delete statement;
    }

    void setSeqIDProp() {

        std::string query = "SELECT COUNT(DISTINCT (" + groupAttrList + ")) FROM "+ pm.tab_source ;
        mrdb::ResultSet* rs = execQr(query);
        rs->next();
        grpId_count = rs->getInt(1);
        delete rs;

        query= "ALTER TABLE "+ pm.tab_result + "_Source ADD COLUMN GRPID NUMERIC;";
        execQuery(query);

        try {

            query= "CREATE TABLE TEMP_GRPID(GRPID SERIAL PRIMARY KEY";
            for(int i=0; i<pm.ga.size(); ++i)
                query+= ", " + pm.ga[i] + " " +get_type(pm.ga[i]);
            query+=");";
            execQuery(query);

            query= "INSERT INTO TEMP_GRPID(" + groupAttrList + ")"
                    " SELECT DISTINCT(" + groupAttrList + ") FROM "+ pm.tab_result + "_Source";
            execQuery(query);
            execQuery("CREATE INDEX temp_grpid_index ON TEMP_GRPID("+groupAttrList+");");
            query="UPDATE "+ pm.tab_result + "_Source src"
                    " SET GRPID=(SELECT s.GRPID FROM TEMP_GRPID s WHERE ";
            for(int i=0; i<pm.ga.size()-1; ++i)
                query+=" s."+pm.ga[i]+"=src."+pm.ga[i]+" AND ";
            query+=" s."+pm.ga[pm.ga.size()-1]+"=src."+pm.ga[pm.ga.size()-1]+");";
            execQuery(query);

            //std::vector<std::vector<std::string> > hash_vect;
            std::vector<std::string> grpAttrVals;
            query= "SELECT DISTINCT GRPID," + groupAttrList + " FROM TEMP_GRPID ORDER BY GRPID";
            rs= execQr(query);
            while(rs->next()) {
                int grpId= rs->getInt(1);
                for(int i=0; i<pm.ga.size(); ++i)
                    grpAttrVals.push_back(rs->getString(2+i));
                hash_vect.push_back(grpAttrVals);
                grpAttrVals.clear();
            }
            delete rs;

            dropTable("TEMP_GRPID");

        } catch(...) {
            dropTable("TEMP_GRPID");
            throw std::exception();
        }

        pm.ga.clear();
        pm.ga.push_back("GRPID");
        groupAttrList="GRPID";
        //ADD GRPID NUMERIC IN ATTR_TYPE_LIST PER LE VARIE GET_TYPE
        initAttrNameList();
        initAttrTypesList(pm.tab_result + "_Source");

        query = "SELECT MIN("+ groupAttrList + ") FROM "+ pm.tab_result + "_Source;";
        rs = execQr(query.c_str());
        rs->next();
        min_trackid=rs->getInt(1);
        delete rs;
        query = "SELECT MAX(" + groupAttrList + ") FROM "+ pm.tab_result + "_Source;";
        rs = execQr(query);
        rs->next();
        max_trackid=rs->getInt(1);
        delete rs;
    }


    bool emptyTable(std::string tableName) {
        std::string query = "SELECT * FROM " + tableName + " LIMIT 1";
        mrdb::ResultSet* rs = execQr(query);
        bool x=!(rs->next());
        delete rs;
        return x;
    }

    bool tableExists(const char * tableName){
  			std::string table(tableName, strlen(tableName));
  			std::string query = "SELECT relname FROM pg_class WHERE upper(relname) = upper('"+table+"');";
  			mrdb::ResultSet* rs = execQr(query);
  			bool result = rs->next();
  			delete rs;
  			return result;
  	}

    void dropTable(std::string table) {
            execQuery("DROP TABLE IF EXISTS "+table+";");
    }

    bool checkConstraint(std::string op, std::string val, std::string range) {
      //  std::cout<<"checking: "+val+op+range<<std::endl;
        if(op.compare("=") == 0)
            return (val.compare(range)) == 0;
        if(op.compare("<") == 0)
            return (val.compare(range)) < 0;
        if(op.compare(">") == 0)
            return (val.compare(range)) > 0;
        if(op.compare("<=") == 0)
            return (val.compare(range)) <= 0;
        if(op.compare(">=") == 0)
            return (val.compare(range)) >= 0;
        if(op.compare("!=") == 0 || op.compare("<>") == 0)
            return (val.compare(range)) != 0;
        if(op.compare("BETWEEN") == 0) {
            //range contiene: "const1 AND const2"
            unsigned pos = range.find("AND");
            std::string const1 = range.substr(0,pos-1);
            std::string const2 = range.substr(pos+4);
            return ((val.compare(const1)) >= 0 && (val.compare(const2)) <= 0);
        }
    }

    std::vector<std::string> copy_and_merge(std::vector<std::string> v1,std::vector<std::string> v2) {
        std::vector<std::string> v;
        for(int i=0; i<v1.size();++i)
            v.push_back(v1[i]);
        v.push_back(v2[v2.size()-1]);
        return v;
    }

    void updateMatcher(int seqId,int id_el) {
        for(int i=0;i<bvptr[id_el-1]->length();++i){
            if(bvptr[id_el-1]->test(i)==true){
                    matcher_cache << seqId ;
                    for(int j=0;j<hash_vect[i].size();++j)
                        matcher_cache << "\t"<< hash_vect[i][j] ;
                    matcher_cache << std::endl;
            }
        }
    }

    void finalize_matcher(bool at_least_one){
        matcher_cache.close();
        std::string loadstr = "/tmp/" + pm.tab_result + ".m";
        if(at_least_one) {
            const std::string& dbms = MineruleOptions::getSharedOptions().getMRDB().getDBMS();
            if( dbms == "postgres" ) {
                loadstr = "COPY " + pm.tab_result  +"_matcher FROM '/tmp/" + pm.tab_result + ".m'";
            }
            else  {
                throw MineruleException( MR_ERROR_OPTION_CONFIGURATION,
                                         "Option for key mrdb::dbms is not set properly, it is set to "+dbms+
                                         ", but only 'postgres' is supported." );
            }
            mrdb::Statement* state = connection.getMRDBConnection()->createStatement();
            state->execute(loadstr.c_str());
            delete state;
        }
        loadstr = "/tmp/" + pm.tab_result + ".m";
        unlink(loadstr.c_str());
    }

    void finalize_seqi(int k) {
        seqicache.close();
        std::string loadstr = "/tmp/" + pm.tab_result + ".s";
        if(k>1){
            const std::string& dbms = MineruleOptions::getSharedOptions().getMRDB().getDBMS();
            if( dbms == "postgres" ) {
                loadstr = "COPY " + pm.tab_result  +"_Seq"+Converter(k).toString()+"(ID_1,ID_2) FROM '/tmp/" + pm.tab_result + ".s'";
            }
            else  {
                throw MineruleException( MR_ERROR_OPTION_CONFIGURATION,
                                         "Option for key mrdb::dbms is not set properly, it is set to "+dbms+
                                         ", but only 'postgres' is supported." );
            }
            mrdb::Statement* state = connection.getMRDBConnection()->createStatement();
            state->execute(loadstr.c_str());
            delete state;
        }
        loadstr = pm.tab_result + "_"+Converter(k).toString()+".s";
        unlink(loadstr.c_str());
    }

    void reset_seqi_cache(int k) {
        std::string temp = "/tmp/" + pm.tab_result + ".s";
        seqicache.open(temp.c_str(), std::ofstream::trunc);
    }


    void saveSingleton(int i) {
        //seq_count counts the returned sequences (seq_count-1==0->no sequences generated
        double sup = bvptr[i-1]->count(true)/(double)grpId_count;
        connection.insert(seq_count,i,sup,0,true);//pos=0 && first=true perche' sono singoletti
        updateMatcher(seq_count,i);
        seq_count++;
    }

    void saveSequence(int id, std::vector<std::string> elements, double sup) {
        for(int h = 0; h < elements.size(); ++h)
            connection.insert(seq_count,Converter(elements[h]).toLong(),sup,h,(h==0));
        updateMatcher(seq_count,id);
        seq_count++;
    }


    template<typename FwdIterator>
    void deleter(FwdIterator from, FwdIterator to)
    {
       while ( from != to )
       {
           delete *from;
           from++;
       }
    }

    void show_Results(bool show) {
        if(!show)
            return;
        if(seq_count==1)
            MRLog()<<"NO SEQUENCES EXTRACTED"<<std::endl;
        else
            printSeq(printSeq1());
     }

    std::vector<std::string> printSeq1() {

        std::vector<std::string> seq1_str;
        std::string el_val = "";
        mrdb::ResultSet* rs = execQr("SELECT * FROM "+ pm.tab_result + "_Seq1");

        MRDebug() << "Seq1 : " << std::endl;

        while(rs->next()) {

            int id = rs->getInt(1);

                el_val = "";
                for(int i = 0 ; i < pm.ba.size() ; ++i){
                    el_val += rs->getString(i+2);
                    if(i<pm.ba.size()-1) el_val += ",";
                }
                seq1_str.push_back("[" + el_val + "] ");
       }
        delete rs;
        return seq1_str;
    }

    void printSeq(std::vector<std::string> seq_ris) {
        std::string q;
        mrdb::ResultSet* rs;
        int id_el, id,id_old;
        std::vector<float> sup;

        q = "SELECT * FROM " + pm.tab_result + "_Seq_support ORDER BY ID";
        rs= execQr(q);
        while(rs->next())
            sup.push_back(rs->getFloat(2));

        delete rs;

        q = "SELECT * FROM " + pm.tab_result + "_Seq ORDER BY ID,POS";
        rs= execQr(q);
        MRDebug() << "Sequences extracted : " << std::endl;
        id_old=-1;

        while(rs->next()) {
            id = rs->getInt(1);
            id_el = rs->getInt(2);
            if(id>id_old)
                std::cout << std::endl << std::endl <<  "ID: " + Converter(id).toString() << + " with support: "
                             + Converter(round(sup[id-1]*grpId_count)).toString()
                             + "/" + Converter(grpId_count).toString() + " elements: " << std::endl;

            std::cout << seq_ris[id_el-1]  ;
            id_old=id;
        }
        delete rs;
    }

/*****************  UTILITIES   ********************/

void print_singleton_data() {
    /********************/
    for(int i = 0; i < bvptr_length; ++i) {
            std::cout<<"Bitvectors for Seq1 "+Converter(i).toString()+" of ID "+Converter(i+1).toString()+":"<<std::endl;
            operator<<(std::cout,*bvptr[i]);
            std::cout<<std::endl<<std::endl;
    }
    /********************/
    for(int i = 0; i < pos_lists_singleton.size(); ++i) {
        std::cout<<"POS_LIST for Seq1 "+Converter(i).toString()+" of ID "+Converter(i+1).toString()+":"<<std::endl;
        std::cout<<pos_list_string(pos_lists_singleton[i]);
        std::cout<<std::endl;
    }
    /********************/
    if(context_dep_BEM)
    for(int i = 0; i < constr_vect_singleton.size(); ++i) {
        std::cout<<"Constr_vectors for Seq1 "+Converter(i).toString()+" of ID "+Converter(i+1).toString()+": "<<std::endl;
        for(int k = 0; k < constr_vect_singleton[i].size(); ++k)
            for(int j = 0; j < constr_vect_singleton[i][k]->bem.size(); ++j) {
                operator<<(std::cout,(constr_vect_singleton[i][k]->bem[j]));
                if(active_count) {
                    std::cout<<std::endl<<"Mid counters: ";
                    for(int h=0;h<constr_vect_singleton[i][k]->mid_counters[j].size();++h)
                        std::cout<<Converter(constr_vect_singleton[i][k]->mid_counters[j][h].first
                                             +constr_vect_singleton[i][k]->mid_counters[j][h].second).toString()+", ";
                }
            std::cout<<std::endl;
        }
    }
    /********************/
    if(context_free_BEM)
    for(int i = 0; i < bem_vect.size(); ++i) {
        std::cout<<"BEMvectors for Seq1 "+Converter(i).toString()+" of ID "+Converter(i+1).toString()+": "<<std::endl;
            operator<<(std::cout,(*bem_vect[i]));
            std::cout<<std::endl;
    }
    /********************/
    if(context_free_BEM && active_count)
        for(int i=0;i<mid_count.size();++i) {
            std::cout<<"Mid counters for id: "+ Converter(i+1).toString()<<std::endl;
            for(int j=0;j<mid_count[i].size();++j)
                std::cout<<Converter(mid_count[i][j].first).toString()+","+Converter(mid_count[i][j].second).toString()+"-";
            std::cout<<std::endl;
        }
    /*********************/
}

void print_structs_values() {

    /********************/
    for(int i = 0; i < bvptr.size(); ++i){
        std::cout<<"Bitvectors for Seq "+Converter(i).toString()+" of ID "+Converter(i+1).toString()+":"<<std::endl;
        operator<<(std::cout,*bvptr[i]);
        std::cout<<std::endl;
    }

    /********************/
    if(context_free_BEM)
    for(int i = 0; i < bem_vect.size(); ++i){
            std::cout<<"BEMvectors of ID "+Converter(i+1).toString()+":"<<std::endl;
            operator<<(std::cout,*bem_vect[i]);
            std::cout<<std::endl;
    }
    /********************/
    if(context_dep_BEM) {
        for(int k=0;k<constr_vect.size();++k) {
            std::cout<<"Constr_vectors for Seq of ID "+Converter(k+1).toString()+": "<<std::endl;
            for(int j =0; j<constr_vect[k].size();++j)
                for(int i=0;i<constr_vect[k][j]->bem.size();++i) {
                    operator<<(std::cout,constr_vect[k][j]->bem[i]);
                    if(active_count) {
                        std::cout<<std::endl<<"Mid counters: ";
                        for(int h=0;h<constr_vect[k][j]->mid_counters[i].size();++h)
                            std::cout<<Converter(constr_vect[k][j]->mid_counters[i][h].first
                                                 +constr_vect[k][j]->mid_counters[i][h].second).toString()+", ";

                    }
                    std::cout<<std::endl;
                }
        }
    }
    /********************/
    for(int i = 0; i < pos_lists.size(); ++i){
        std::cout<<"POS_LIST for Seq of ID "+Converter(i+1).toString()+":"<<std::endl;
        std::cout<<pos_list_string(pos_lists[i])<<std::endl;
       // std::cout<<"pos_length: "+Converter(pos_lists[i].size()).toString();
        std::cout<<std::endl;
    }
    /**************************/
    if(context_dep_dist)
    for(int i = 0; i<prefix_pos_lists.size(); ++i){
        std::cout<< "Prefix-list for id:"+Converter(i+1).toString()<<std::endl;
        for(int j = 0; j<prefix_pos_lists[i].size(); ++j){
            std::cout<< "pos entry "+Converter(j).toString()<<std::endl;
            for(int h = 0; h<prefix_pos_lists[i][j].size(); ++h){
                    std::cout<<Converter(prefix_pos_lists[i][j][h]).toString()+",";
            }
            std::cout<<std::endl;
        }
    }
    /**************************/
    if(context_free_BEM && active_count)
        for(int i=0;i<mid_count.size();++i) {
            std::cout<<"Mid BEM_CF counters for id: "+ Converter(i+1).toString()<<std::endl;
            for(int j=0;j<mid_count[i].size();++j)
                std::cout<<Converter(mid_count[i][j].first).toString()+","+Converter(mid_count[i][j].second).toString()+"-";
            std::cout<<std::endl;
        }
    /*********************/
}

};//end class
}//end namespace
#endif // STSMiner_H

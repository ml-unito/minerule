#ifndef STSMINERWITHBITVECTOR_H
#define STSMINERWITHBITVECTOR_H

#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Database/Connection.hpp"
#include "minerule/Database/MRResultSet.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"
#include "minerule/Utils/Converter.hpp"
#include "minerule/Utils/AlgorithmTypes.hpp"
#include "minerule/Utils/Bitstring.hpp"

namespace minerule{

class STSMinerWithBitVector : public MiningAlgorithm{

private:
    AlgorithmsOptions options;
    const ParsedMinerule& pm;
    PrepareDataUtils pdu;
    std::vector<std::string> AttrList;//lista di tutti gli attr coinvolti nella query senza ripetizioni
    std::string groupAttrList;
    std::string ordAttrList;
    std::string bodyAttrList;
    std::vector<std::vector<BitString*> > begin_end_vect;
    int seqid_count;
    std::vector<std::vector<BitString*> > bvptr;
    int bvptr_length;
    int bv_length;
    int min_trackid;

    /*il seguente blocco andrebbe realizzato nel parser*/
    int min_g; //gap min
    int max_g; //gap max
    std::string begin_e;
    std::string begin_t;
    std::string end_e;
    std::string end_t;
    std::string begin_t_operator;
    std::string begin_e_operator;
    std::string end_t_operator;
    std::string end_e_operator;
    int max_space_gap;
    int min_space_gap;

    bool extractKSequences(int k);
    bool extract2Sequences();
    bool extract1Sequences();
    void createBitVectors();
    bool pruning1Seq();
    bool pruningSeqK(int k);
    void endFilterOn(int k);
    void sourceFilter();

public:
    STSMinerWithBitVector(const OptimizedMinerule& mr, const AlgorithmsOptions& opts) :
      MiningAlgorithm(mr),pm(mr.getParsedMinerule()),pdu(pm, this->sourceTableRequirements()){
        options = opts;

        min_g=pm.sequenceAllowedGaps.getMin();
        max_g=pm.sequenceAllowedGaps.getMax();
        max_space_gap=0;
        min_space_gap=0;
        /*************************************/
        //il parser va modificato perchè legga le clausole begin/end per la seguente inzializzazione
        //direi di fargliele memorizzare direttamente nella pm
        begin_e="";
        begin_t="";
        end_e="";
        end_t="";
        begin_t_operator="=";//ammettiamo: = , < , > , >= , <= , !=
        begin_e_operator="=";
        end_t_operator="=";
        end_e_operator="=";

        if(!check(begin_e_operator)){
            MRDebug()<< "Wrong operator definition in begin clause!"<<std::endl<<"Allowed operator: = , > , < , >= , <= , !="<<std::endl;
            std::cout<<std::endl<<"Using default operator : ="<<std::endl;
            begin_e_operator="=";
        }
        if(!check(begin_t_operator)){
            MRDebug()<< "Wrong operator definition in begin clause!"<<std::endl<<"Allowed operator: = , > , < , >= , <= , !="<<std::endl;
            std::cout<<std::endl<<"Using default operator : ="<<std::endl;
            begin_t_operator="=";
        }
        if(!check(end_t_operator)){
            MRDebug()<<"Wrong operator definition in end clause!"<<std::endl<<"Allowed operator: = , > , < , >= , <= , !=";
            std::cout<<std::endl<<"Using default operator : ="<<std::endl;
            end_t_operator="=";
        }
        if(!check(end_e_operator)){
            MRDebug()<<"Wrong operator definition in end clause!"<<std::endl<<"Allowed operator: = , > , < , >= , <= , !=";
            std::cout<<std::endl<<"Using default operator : ="<<std::endl;
            end_e_operator="=";
        }
        /**************************************/

        for(int i=0; i<pm.ga.size();i++)
            AttrList.push_back(pm.ga.at(i));
        bool found=false;
        for(int i=0; i<pm.oa.size();i++){
            for(int j=0; j<AttrList.size();j++)
                //se l'attributo era già tra quelli del body lo segnamo come found e non lo aggiungiamo
                if((AttrList.at(j)).compare(pm.oa.at(i))==0) found=true;
            if(!found)AttrList.push_back(pm.oa.at(i));
            found=false;
        }
        found=false;
        for(int i=0; i<pm.ba.size();i++){
              for(int j=0; j<AttrList.size();j++)
                  //se l'attributo è già stato inserito lo segnamo come found e non lo aggiungiamo
                  if((AttrList.at(j)).compare(pm.ba.at(i))==0)found=true;
              if(!found)AttrList.push_back(pm.ba.at(i));
              found=false;
        }

        groupAttrList = pdu.buildAttrListDescription(pm.ga);
        ordAttrList = pdu.buildAttrListDescription(pm.oa);
        bodyAttrList=pdu.buildAttrListDescription(pm.ba);

        //stampe di controllo
     /* operator<<(MRDebug(),pm);
        MRDebug()<<"GroupAttrListString: "<<groupAttrList<<std::endl;
        MRDebug()<<"OrdAttrListStriing: "<<ordAttrList<<std::endl;
        MRDebug()<<"bodyAttrListStriing: "<<bodyAttrList<<std::endl;
        MRDebug()<<"TotalAttrListStriing: ";
        for(int j=0; j<AttrList.size();j++)
           std::cout<< AttrList.at(j)<<" , ";
        std::cout<<std::endl;*/
    }

    virtual bool canHandleMinerule() const {
      return pm.miningTask == MTMineSequences;
    }

    virtual void mineRules();

    virtual ~STSMinerWithBitVector() {}

    virtual SourceTableRequirements sourceTableRequirements() const {
            return SourceTableRequirements(SourceTableRequirements::SortedGids);
    }


    /*****UTILITIES*******************/

    void createSeqQ(int idseq){
        std::string id_s;
        if(idseq==0)  id_s="src";
        else id_s = Converter(idseq).toString();
        std::string createSeqIDQuery = " CREATE SEQUENCE id"+id_s+"_sq";
        createSeqIDQuery += " INCREMENT 1 MINVALUE 1 MAXVALUE 9223372036854775807";
        createSeqIDQuery += " START 1 CACHE 1; ALTER TABLE id"+id_s+"_sq OWNER TO postgres;";
        execQr(createSeqIDQuery);
    }

    int readSeqVal(int id_seq){

        std::string id_s = Converter(id_seq).toString();
        if(emptyTable("Seq"+id_s)) return 1;
        std::string seqIDQuery = " SELECT currval('id"+id_s+"_sq');";
        mrdb::ResultSet* rs= execQr(seqIDQuery);
        rs->next();
        int val=rs->getInt(1);
        return val;
    }

    mrdb::ResultSet* execQr(std::string sqlQuery){
        options.setStatement(options.getMRDBConnection()->prepareStatement(sqlQuery.c_str()));
        return options.getStatement()->executeQuery();
    }

    int getTrackidMaxValue(){
        std::string query = "SELECT MAX(trackid) FROM Source;";
        mrdb::ResultSet* rs=options.getMRDBConnection()->prepareStatement(query.c_str())->executeQuery();
        rs->next();
        int val=rs->getInt(1);
        return val;
    }

    int getTrackidMinValue(){
        std::string query = "SELECT MIN(trackid) FROM Source;";
        mrdb::ResultSet* rs=options.getMRDBConnection()->prepareStatement(query.c_str())->executeQuery();
        rs->next();
        int val=rs->getInt(1);
        return val;
    }

    int getTrackidCount(){
        std::string query = "SELECT COUNT(DISTINCT trackid) FROM Source;";
        mrdb::ResultSet* rs=options.getMRDBConnection()->prepareStatement(query.c_str())->executeQuery();
        rs->next();
        int val=rs->getInt(1);
        return val;
    }

    bool emptyTable(std::string tableName){//equivale a return readSeqVal==1
        std::string query="SELECT * FROM "+tableName+" LIMIT 1";
        mrdb::ResultSet* rs=options.getMRDBConnection()->prepareStatement(query.c_str())->executeQuery();
        return !(rs->next());
    }

    void dropAllTables(){

        std::string dropQuery = "DROP INDEX IF EXISTS src_index;";
        dropQuery += "DROP INDEX IF EXISTS src_full_index;";
        dropQuery += "DROP INDEX IF EXISTS seq2_ex_index; ";
        dropQuery += "DROP TABLE IF EXISTS Source; ";
        dropQuery += "DROP TABLE IF EXISTS Tmp_Table; ";
        dropQuery += "DROP TABLE IF EXISTS Seq2Extensions; ";
        for(int i=1;i<600;++i){//100?! è da vedere come farlo questo...
            dropQuery += "DROP TABLE IF EXISTS Seq"+Converter(i).toString()+";";
            dropQuery += "DROP SEQUENCE IF EXISTS id"+Converter(i).toString()+"_sq;";
        }
        execQr(dropQuery);
    }

    //return true if the relation el.op.val is true
    bool checkConstraint(std::string el, std::string val, std::string op){
        if(op.compare("=")==0)
            return (el.compare(val))==0;
        if(op.compare("<")==0)
            return (el.compare(val))<0;
        if(op.compare(">")==0)
            return (el.compare(val))>0;
        if(op.compare("<=")==0)
            return (el.compare(val))<=0;
        if(op.compare(">=")==0)
            return (el.compare(val))>=0;
        if(op.compare("!=")==0)
            return (el.compare(val))!=0;

        throw MineruleException(MR_ERROR_INTERNAL, "Operator ("+op+") not supported");
    }

    bool check(std::string op){
        if(op.compare("=")==0)
            return true;
        if(op.compare("<")==0)
            return true;
        if(op.compare(">")==0)
            return true;
        if(op.compare("<=")==0)
            return true;
        if(op.compare(">=")==0)
            return true;
        if(op.compare("!=")==0)
            return true;
        return false;
    }

    bool existsTable(int k){
        std::string strK=Converter(k).toString();
        std::string q="IF EXISTS Seq"+strK+"SELECT * FROM Seq"+strK+"LIMIT 1";
        mrdb::ResultSet* rs= execQr(q);
        return rs->next();
    }

    void showResults(int k){
        std::string q;
        mrdb::ResultSet* rs;
        int d_time;
        int id_1;
        int id_2;
        std::string subarea_id;
        std::vector<std::vector<std::string> >seq_ris;
        //stampiamo seq1 a parte perchè ha uno schema diverso
        std::vector<std::string> seq1_str;
        q="SELECT * FROM Seq1";
        rs=execQr(q);
        std::cout<<"Seq1 : " << std::endl;
        int j=0;
        while(rs->next()) {
            d_time=rs->getInt(1);
            subarea_id= rs->getString(2);
            seq1_str.push_back("<"+subarea_id+","+Converter(d_time).toString()+"> ");
            if(begin_end_vect.at(0).at(j)->count(true)==2 ){
                std::cout<< "<"+subarea_id+","+Converter(d_time).toString()+"> "<< std::endl;
            }
            ++j;
        }
        seq_ris.push_back(seq1_str);
        int i=2;

        while(i<k){
            std::vector<std::string> seqi_str;
            std::string str= Converter(i).toString();
            q="SELECT * FROM Seq"+str;
            rs= execQr(q);
            std::cout<<"Seq"+str+" : " << std::endl;
            j=0;

            while(rs->next()){
                id_1=rs->getInt(1);
                id_2=rs->getInt(2);
                std::string s=seq_ris[i-2][id_2-1];
                s=s.substr(s.find_last_of('<'));
                std::string tmp= seq_ris[i-2][id_1-1]+ " " +s;
                seqi_str.push_back(tmp);
                ++j;
            }
            seq_ris.push_back(seqi_str);

            for(int h=0; h<seqi_str.size();h++)
                if(begin_end_vect.at(i-1).at(h)->count(true)==2 )
                    std::cout<<seqi_str[h]<<std::endl;
            ++i;
        }
     }

};//end class
}//end namespace
#endif // STSMINERWITHBITVECTOR_H

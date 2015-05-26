/*************************************/
/*	       STSequences miner         */
/*************************************/

/* La seguente classe crea un oggetto STSMiner il quale data la MINE SEQUENCE in input elabora
in automatico delle query SQL per estrarre le sequenze frequenti dal db delegandogli così il grosso del lavoro*/

/*
L'algoritmo accetta un attributo per l'ordinamento ed un attributo per gli elementi e restituisce
sequenze di coppie <el_attr,ord_attr> che rispettano il vincolo del supporto e del gap inteso come gap
tra i valori dell'attributo di ordinamento tra due elementi(coppie) successivi della sequenza.

QUERY SYNTAX:

MINE SEQUENCE l_min..l_max dest_table AS
SELECT [DISTINCT] bodyAttrList, SUPPORT
WITH GAP min..max
FROM table
[HAVING CONDITIONS]
[WHERE SQL_BOOL_COND]
GROUP BY groupAttrList
ORDER BY ordAttrList

CONDITIONS -> CONDITIONS AND CONDITIONS | ( CONDITIONS ) | COND
COND -> DISTANCE RANGE | BEM_CLAUSE RANGE
DISTANCE -> FUN_NAME(ELEM1,ELEM2)
RANGE -> BETWEEN const1 AND const2 | OP const
OP -> = | > | < | >= | <= | !=
BEM_CLAUSE -> BEGIN.ATTR | END.ATTR | [MIN..MAX] MID.ATTR
ATTR è un qualunque attributo della table sorgente
SQL_BOOL_COND-> ....
*/

#include "minerule/Algorithms/STSMiner.hpp"

namespace minerule {

/******************************************
 subquery= (SELECT row_number() OVER(ORDER BY trackid,sighting_event_id) AS rn, trackid, subarea_of_reference, sighting_event_id
                   FROM (SELECT trackid, subarea_of_reference , sighting_event_id
                         FROM mission_M_003
                         UNION
                             (SELECT (SELECT MIN(trackid)-1 FROM mission_M_00i) AS trackid,
                                     (SELECT MIN(subarea_of_reference) FROM mission_M_00i) AS subarea_of_reference,
                                     (SELECT MIN(sighting_event_id) FROM mission_M_00i) AS sighting_event_id
                              UNION
                              SELECT (SELECT MAX(trackid)+1 FROM mission_M_00i) AS trackid,
                                     (SELECT MAX(subarea_of_reference) FROM mission_M_00i) AS subarea_of_reference,
                                     (SELECT MAX(sighting_event_id) FROM mission_M_00i) AS sighting_event_id)
                         ) AS SRC_SELECTION
                         GROUP BY trackid, subarea_of_reference, sighting_event_id
                         ORDER BY trackid,sighting_event_id
                    )

INSERT INTO test1_Source

SELECT pos_not_distinct.trackid, pos_not_distinct.subarea_of_reference, pos_not_distinct.pos, pos_distinct.pos
FROM
(SELECT trackid, subarea_of_reference,  row_number() OVER(PARTITION BY trackid ORDER BY trackid, sighting_event_id) AS POS , sighting_event_id
FROM mission_M_00i
GROUP BY trackid, subarea_of_reference, sighting_event_id
ORDER BY trackid,sighting_event_id) AS pos_not_distinct

JOIN     <--iff DISTINCT=true

( SELECT  _s1.trackid,  _s1.subarea_of_reference,  _s2.sighting_event_id_MIN,  _s1.sighting_event_id_MAX, _s1.pos
  FROM (SELECT s.*,row_number() OVER(PARTITION BY  s.trackid) AS POS
        FROM (SELECT  s1.trackid, s1.subarea_of_reference,  s1.sighting_event_id AS sighting_event_id_MAX
              FROM subquery AS s1,
                   subquery AS s2
                    WHERE s1.rn+1=s2.rn
                        AND (
                                ( (  s1.trackid=s2.trackid) AND (  s1.subarea_of_reference!=s2.subarea_of_reference ))
                             OR ( s1.trackid!=s2.trackid)
                             )
                    ORDER BY  s1.trackid, s1.sighting_event_id
              ) AS s
       ) AS _s1

       JOIN

      (SELECT s.*,row_number() OVER(PARTITION BY  s.trackid) AS POS
       FROM (SELECT  s2.trackid, s2.subarea_of_reference,  s2.sighting_event_id AS sighting_event_id_MIN
             FROM subquery AS s1,
                  subquery AS s2
                     WHERE s1.rn+1=s2.rn
                                AND (
                                     ( (  s1.trackid=s2.trackid) AND (  s1.subarea_of_reference!=s2.subarea_of_reference ))
                                     OR ( s1.trackid!=s2.trackid)
                                    )
                     ORDER BY  s1.trackid, s1.sighting_event_id
            ) AS s
     ) AS _s2

      ON  _s1.trackid=_s2.trackid
        AND _s1.subarea_of_reference=_s2.subarea_of_reference
        AND _s1.pos=_s2.pos
      ORDER BY trackid,pos
) AS pos_distinct

ON
pos_not_distinct.trackid=pos_distinct.trackid
AND  pos_not_distinct.subarea_of_reference=pos_distinct.subarea_of_reference
AND  pos_not_distinct.sighting_event_id>=pos_distinct.sighting_event_id_MIN
AND pos_not_distinct.sighting_event_id<=pos_distinct.sighting_event_id_MAX;
  *****************************************/

/** _SOURCE TABLE:
 * The algorithm needs to pre-process source data in order to create an indexed table containing just the
 * usefull informations.
 * We tried using a view but this table is involved in the pair generation query, which becomes too slow without
 * indexes.
 */
bool STSMiner::extract1Sequences() {

    std::string sqlQuery;
    std::string src_schema = "";
    std::string attr_string = "";
    std::string attr_string2 = "";

    for(int i = 0; i < attr_list.size(); ++i){
        src_schema += attr_list[i] + " " + get_type(attr_list[i]) + ", " ;
        if(containedIn(attr_list[i],pm.ga) || containedIn(attr_list[i],pm.oa) || containedIn(attr_list[i],pm.ba))
            attr_string += ", " + attr_list[i];
        attr_string2 += attr_list[i] + ", ";
    }

    std::string o_not_b="";
    for(int i = 0; i < pm.oa.size(); ++i)
        if(!containedIn(pm.oa[i],attr_list))
            o_not_b += ", " + pm.oa[i];

    std::string src_selection= "(SELECT ";
    if(containedIn(attr_list[0],pm.ga)
      || containedIn(attr_list[0],pm.oa)
      || containedIn(attr_list[0],pm.ba))
        src_selection+= attr_list[0];
    for(int i = 1; i < attr_list.size(); ++i)
        if(containedIn(attr_list[i],pm.ga) || containedIn(attr_list[i],pm.oa) || containedIn(attr_list[i],pm.ba))
            src_selection+= ", "+attr_list[i];
    src_selection+= " "+o_not_b+" FROM "+pm.tab_source+" UNION "
            + " ("+union_rows()+")) AS SRC_SELECTION";//le due righe fittizie di cui non dobbiamo più fare la insert

    sqlQuery = "CREATE TABLE " + pm.tab_result + "_Source(" + src_schema + " POS INTEGER";

    if(DISTINCT)
        sqlQuery += ", POS_D INTEGER);";
    else
        sqlQuery += ");";

    execQuery(sqlQuery); //creo la src

    //la select di cui fare la insert con logica not_distinct!
    sqlQuery= "SELECT " + attr_string2 + " row_number() OVER(PARTITION BY "
            + groupAttrList + " ORDER BY " + groupAttrList + ", " + ordAttrList+ ") AS POS ";
    if(DISTINCT)
        sqlQuery += o_not_b;
    sqlQuery +=  " FROM " + pm.tab_source;

    if(pm.filter_condition.size()>0) {
        sqlQuery += " WHERE " + pm.filter_condition;
        if(DISTINCT){
            //nel caso distinct per calcolare la tabella pos_distinct mi serve aggiungere due righe fittizie
            //che nella tabella pos_distinct non compaiono ma per non includerle in questa aggiungo una condizione
            //che esclude la track minima e la track massima, cioè quelle che aggiungo
            for(int i = 0; i < pm.ga.size(); ++i) {
                sqlQuery += " AND " + pm.ga[i] + ">(SELECT MIN(" + pm.ga[i] + ") FROM " + src_selection + ")";
                sqlQuery += " AND " + pm.ga[i] + "<(SELECT MAX(" + pm.ga[i] + ") FROM " + src_selection + ")";
            }
        }
    }
    sqlQuery += " GROUP BY ";
    for(int i = 0; i < attr_list.size()-1; ++i)
        sqlQuery += attr_list[i] + ", ";
    sqlQuery += attr_list[attr_list.size()-1] + o_not_b;
    sqlQuery += " ORDER BY " + groupAttrList + ","+ordAttrList;

    std::string pos_not_distinct = " (" + sqlQuery + ") AS pos_not_distinct ";
    std::string pos_distinct = "";

    //se siamo con logica distinct dobbiamo fare il join con la tabella che incrementa la pos al cambio di body
    if(DISTINCT) {
        sqlQuery= "SELECT s.*,row_number() OVER(PARTITION BY ";
        for(int i = 0; i < pm.ga.size()-1; ++i)
            sqlQuery += " s."+pm.ga[i]+", ";
        sqlQuery += " s."+pm.ga[pm.ga.size()-1]+") AS POS FROM (SELECT ";

        std::string select_s1=sqlQuery;
        std::string select_s2=sqlQuery;

        for(int i = 0; i < pm.ga.size(); ++i){
            select_s1 += " s1."+pm.ga[i]+",";
            select_s2 += " s2."+pm.ga[i]+",";
        }
        for(int i = 0; i < pm.ba.size(); ++i){
            select_s1 += " s1."+pm.ba[i]+", ";
            select_s2 += " s2."+pm.ba[i]+", ";
        }
        for(int i = 0; i < pm.oa.size()-1; ++i){
            select_s1 += " s1."+pm.oa[i]+" AS "+pm.oa[i]+"_MAX, ";
            select_s2 += " s2."+pm.oa[i]+" AS "+pm.oa[i]+"_MIN, ";
        }
        select_s1 += " s1."+pm.oa[pm.oa.size()-1]+" AS "+pm.oa[pm.oa.size()-1]+"_MAX FROM ";
        select_s2 += " s2."+pm.oa[pm.oa.size()-1]+" AS "+pm.oa[pm.oa.size()-1]+"_MIN FROM ";

        std::string sqlSubQuery = "(SELECT row_number() OVER(ORDER BY "
                + groupAttrList+","+ordAttrList
                + ") AS rn" + attr_string + o_not_b
                + "  FROM " + src_selection;
        sqlSubQuery += " GROUP BY ";
        for(int i = 0; i < pm.ga.size(); ++i)
            sqlSubQuery+= pm.ga[i]+", ";
        for(int i = 0; i < pm.ba.size(); ++i)
            if(!containedIn(pm.ba[i],pm.ga) && !containedIn(pm.ba[i],pm.oa))
                sqlSubQuery+= pm.ba[i]+", ";
        sqlSubQuery+= pm.oa[0];
        for(int i = 1; i < pm.oa.size() ; ++i)
            if(!containedIn(pm.oa[i],pm.ga))
                sqlSubQuery+= ", "+pm.oa[i];

        sqlSubQuery += " ORDER BY " + groupAttrList+","+ordAttrList + ")" ;

        sqlQuery = sqlSubQuery+" AS s1, ";
        sqlQuery += sqlSubQuery+" AS s2 WHERE s1.rn+1=s2.rn AND ((";
        sqlQuery += " ( ";
        for(int i = 0; i < pm.ga.size()-1; ++i)
            sqlQuery += " s1."+pm.ga[i]+"=s2."+pm.ga[i]+" AND ";
        sqlQuery += " s1."+pm.ga[pm.ga.size()-1]+"=s2."+pm.ga[pm.ga.size()-1];
        sqlQuery += ") AND ( ";

        for(int i = 0; i < pm.ba.size()-1; ++i)
            sqlQuery += " s1."+pm.ba[i]+"!=s2."+pm.ba[i]+" OR ";
        sqlQuery += " s1."+pm.ba[pm.ba.size()-1]+"!=s2."+pm.ba[pm.ba.size()-1]+" )) OR (";

        for(int i = 0; i < pm.ga.size()-1; ++i)
            sqlQuery += " s1."+pm.ga[i]+"!=s2."+pm.ga[i]+" OR ";
        sqlQuery += " s1."+pm.ga[pm.ga.size()-1]+"!=s2."+pm.ga[pm.ga.size()-1]+"))";

        sqlQuery += " ORDER BY ";
        for(int i = 0; i < pm.ga.size(); ++i)
            sqlQuery += " s1."+pm.ga[i]+",";
        for(int i = 0; i < pm.oa.size()-1; ++i)
            sqlQuery += " s1."+pm.oa[i]+", ";
        sqlQuery += " s1."+pm.oa[pm.oa.size()-1];

        sqlQuery += ") AS s ";

        select_s1 += sqlQuery;
        select_s2 += sqlQuery;

        select_s1 = "("+select_s1+") AS _s1";
        select_s2 = "("+select_s2+") AS _s2";

        sqlQuery =" SELECT ";
        //tutto da _s1 e da _s2 solo oa[i]_MIN

        for(int i = 0; i < pm.ga.size(); ++i)
            sqlQuery += " _s1."+pm.ga[i]+", ";

        for(int i = 0; i < pm.ba.size() ; ++i)
            sqlQuery += " _s1."+pm.ba[i]+", ";

        for(int i = 0; i < pm.oa.size(); ++i) {
            sqlQuery += " _s2."+pm.oa[i]+"_MIN, ";
            sqlQuery += " _s1."+pm.oa[i]+"_MAX, ";
        }
        sqlQuery += "_s1.pos FROM "+select_s1+" JOIN "+select_s2+" ON ";

        for(int i = 0; i < pm.ga.size(); ++i)
            sqlQuery += " _s1."+pm.ga[i]+"=_s2."+pm.ga[i];

        for(int i = 0; i < pm.ba.size(); ++i)
            sqlQuery += " AND _s1."+pm.ba[i]+"=_s2."+pm.ba[i];

        sqlQuery += " AND _s1.pos=_s2.pos ORDER BY "+groupAttrList+",pos ";

        pos_distinct = " (" + sqlQuery + ") AS pos_distinct ";

        //QUERY FINALE
        sqlQuery = "INSERT INTO " + pm.tab_result + "_Source SELECT ";
        for(int i = 0; i < attr_list.size(); ++i)
            sqlQuery += "pos_not_distinct."+attr_list[i]+", ";
        sqlQuery += "pos_not_distinct.pos, pos_distinct.pos FROM "
                + pos_not_distinct + "JOIN " + pos_distinct + " ON ";
        for(int i = 0; i < pm.ga.size(); ++i)
            sqlQuery += " pos_not_distinct." + pm.ga[i] + "=pos_distinct." + pm.ga[i] + " AND ";
        for(int i = 0; i < pm.ba.size(); ++i)
            sqlQuery += " pos_not_distinct." + pm.ba[i] + "=pos_distinct." + pm.ba[i] + " AND ";
        for(int i = 0; i < pm.oa.size()-1; ++i)
            sqlQuery += " pos_not_distinct." + pm.oa[i] + ">=pos_distinct." + pm.oa[i]
                    + "_MIN AND pos_not_distinct." + pm.oa[i] + "<=pos_distinct." + pm.oa[i] + "_MAX AND";
        sqlQuery += " pos_not_distinct." + pm.oa[pm.oa.size()-1] + ">=pos_distinct." + pm.oa[pm.oa.size()-1]
                + "_MIN AND pos_not_distinct." + pm.oa[pm.oa.size()-1]
                + "<=pos_distinct." + pm.oa[pm.oa.size()-1] + "_MAX;";
    }//end if DISTINCT
    else
        sqlQuery = "INSERT INTO "+pm.tab_result+"_Source " + sqlQuery;

    //eseguiamo la query main
    execQuery(sqlQuery);

    bool at_least_one=!emptyTable(pm.tab_result + "_Source");
    sqlQuery="";

    if(pm.seq_bem_vect.size()>0 && at_least_one)
        at_least_one = !sourceFilter();

    if(at_least_one) {
        dropTable(pm.tab_result + "_Seq1");
        dropTable(pm.tab_result + "_matcher");

        sqlQuery= "CREATE TABLE " + pm.tab_result + "_Seq1(ID SERIAL PRIMARY KEY ";
        for(int i = 0; i < pm.ba.size(); ++i)
            sqlQuery += ", " + pm.ba[i] + " " + get_type(pm.ba[i]);
        sqlQuery += ");";
        sqlQuery += "CREATE TABLE " + pm.tab_result + "_matcher(SEQ_ID numeric";
        for(int i = 0; i < pm.ga.size(); ++i)
            sqlQuery += ", "+pm.ga[i]+" "+ get_type(pm.ga[i]);
        sqlQuery += "); CREATE INDEX " + pm.tab_result + "_Source_full_index ON "
                + pm.tab_result + "_Source(" + bodyAttrList + ",pos);";
        execQuery(sqlQuery);
        setSeqIDProp();
        return false;
    }
    return true;
}

/**
 * if begin and/or end clauses have been specified we reduce reasearch space
 * deleting from source table all tracks that doesn't contain any element which respects given constraints
 */
bool STSMiner::sourceFilter() {
    if(pm.seq_bem_vect.size()==0)
        return emptyTable(pm.tab_result + "_Source");

    std::string sq = "DELETE FROM "+ pm.tab_result + "_Source WHERE (" + groupAttrList + ") NOT IN( SELECT";
    for(int i = 0; i < pm.ga.size()-1; ++i)
        sq += " s1." + pm.ga[i] + ",";
    sq += " s1." + pm.ga[pm.ga.size()-1] + " FROM ";
    for(int i=0;i<pm.seq_bem_vect.size();++i){
        sq += pm.tab_source + " s"+Converter(i+1).toString();
        if(i<pm.seq_bem_vect.size()-1) sq += ", ";
    }
    sq += " WHERE ";
    for(int i = 0; i < pm.ga.size(); ++i) {
        for(int j = 1; j<pm.seq_bem_vect.size(); ++j)
            sq += " s1." + pm.ga[i] + "=s"+Converter(j+1).toString()+"." + pm.ga[i] + " AND ";
    }
    for(int i = 0; i < pm.seq_bem_vect.size(); ++i) {
        Bem_cond* it = pm.seq_bem_vect[i];
        do {
            //cerchiamo l'attr per capire se è un varchar e aggiungere gli apostrofi
            std::string tmp = it->attr;
            bool varchar_type = false;
            if(textAttr(it->attr))
                varchar_type = true;
            if(varchar_type){
                if(it->op.compare("BETWEEN") == 0) {
                    unsigned pos = tmp.find("AND");
                    std::string const1 = tmp.substr(0,pos-1);
                    std::string const2 = tmp.substr(pos+4);
                    tmp = "'" + const1 + "' AND '" + const2 + "'";
                }
                else tmp = "'" + it->val + "'";
            }
            else tmp = it->val;
            sq += " s" + Converter(i+1).toString() + "." + it->attr + " " + it->op + " " + tmp;
            it = it->and_c;
            if(it != NULL)
                sq += " AND ";
        } while(it);
        if(i < pm.seq_bem_vect.size()-1)
            sq += " AND ";
    }
    sq += ");";

    execQuery(sq);

    return emptyTable(pm.tab_result + "_Source");
}

/**
 * We need those additional rows in order to infer the POS_D attribute (as consequence just in DISTINCT case),
 * that is those attribute which tells to the algorithm which occurences have
 * to be considered useless repetitions just looking at the bodyAttrList
 */
std::string STSMiner::union_rows() {

    std::string insert_q_min="SELECT ";
    std::string insert_q_max="SELECT ";
    bool semicolon= false;

    for(int i=0; i<attr_list.size(); ++i) {
        if(semicolon) {
            insert_q_min += ",";
            insert_q_max += ",";
            semicolon= false;
        }
        if(containedIn(attr_list[i],pm.ga)) {
            if(!textAttr(attr_list[i])) {
                insert_q_min += "(SELECT MIN("+attr_list[i]+")-1 FROM "+pm.tab_source+") AS "+attr_list[i];
                insert_q_max += "(SELECT MAX("+attr_list[i]+")+1 FROM "+pm.tab_source+") AS "+attr_list[i];
            }
            else {
                insert_q_min += "(SELECT SUBSTRING(MIN("+attr_list[i]+") FROM 1 FOR CHAR_LENGTH(MIN("
                        +attr_list[i]+"))-1) FROM "+pm.tab_source+") AS "+attr_list[i];
                insert_q_max += "(SELECT SUBSTRING(MAX("+attr_list[i]+") FROM 1 FOR CHAR_LENGTH(MAX("+attr_list[i]
                        +"))-1)|| 'Z' FROM "+pm.tab_source+") AS "+attr_list[i];
            }
            semicolon= true;
        }
        else if(containedIn(attr_list[i],pm.ba))
        {
            insert_q_min += "(SELECT MIN("+attr_list[i]+") FROM "+pm.tab_source+") AS "+attr_list[i];
            insert_q_max += "(SELECT MAX("+attr_list[i]+") FROM "+pm.tab_source+") AS "+attr_list[i];
            semicolon= true;
        }
    }

    for(int i = 0; i < pm.oa.size() ; ++i) {
        if(!containedIn(pm.oa[i],attr_list)) {
            if(semicolon) {
                insert_q_min += ",";
                insert_q_max += ",";
            }
            insert_q_min += "(SELECT MIN("+pm.oa[i]+") FROM "+pm.tab_source+") AS "+pm.oa[i];
            insert_q_max += "(SELECT MAX("+pm.oa[i]+") FROM "+pm.tab_source+") AS "+pm.oa[i];

        }
    }

    return insert_q_min+" UNION "+insert_q_max;
}

/*********************************
INSERT INTO example_Seq1(subarea_id)
SELECT subarea_id
FROM example_Source
GROUP BY subarea_id
HAVING COUNT( DISTINCT (trackid)) >=0.02;
CREATE INDEX example_Seq1_index ON example_Seq1(ID,subarea_id);
  ********************************/
int STSMiner::pruning1Seq(){

        std::string sqlQuery = "INSERT INTO " + pm.tab_result + "_Seq1(" + bodyAttrList + ") SELECT "
                + bodyAttrList + " FROM "+ pm.tab_result + "_Source GROUP BY " + bodyAttrList;
        if(pm.sup>(1.0/grpId_count))
            sqlQuery  += " HAVING COUNT(DISTINCT GRPID) >=" + Converter(pm.sup*grpId_count).toString();

        sqlQuery += "; CREATE INDEX " + pm.tab_result + "_Seq1_index ON " + pm.tab_result + "_Seq1(ID," + bodyAttrList + ");";
        execQuery(sqlQuery);

        mrdb::ResultSet* r = execQr("SELECT MAX(ID) FROM " + pm.tab_result + "_Seq1");
        if(r->next())
            bvptr_length = r->getInt(1);
        else
            bvptr_length = 0;

        delete r;

        return bvptr_length;
}


/*********************************
  SELECT s1.id,s_1.groupAttr[, s_1.bodyAttr]
  FROM Source s_1 JOIN Seq1 s1 ON
    s_1.bodyAttr = s1.bodyAttr
  GROUP BY s1.id[, s_1.bodyAttr]
  ORDER BY s1.id;
 ********************************/
/**
  *This function initialize bem_context_free bitString and
  *support bitvectors
  *It creates one bem bitstring and one support bitstring for each frequent singleton
**/
void STSMiner::createBitVectors() {

    int id_old= -1;
    int seq_id_i;
    int id;
    std::string query;

    for(int i = 0; i < bvptr_length; ++i)
       bvptr.push_back(new BitString(max_trackid-min_trackid+1));

    if(context_free_BEM) {
        for(int i = 0; i < bvptr_length; ++i)
            bem_vect.push_back(new BitString(bem_vect_length));
        if(active_count) {
            std::vector<std::pair<int,int> > tmp;
            for(int i = 0; i<bem_vect_length-2; ++i)
                tmp.push_back(std::pair<int,int>(0,0));
            for(int i = 0; i < bvptr_length; ++i)
                mid_count.push_back(tmp);
        }
    }
    query ="SELECT s1.id, s_1.GRPID";
    if(context_free_BEM)
        for(int i = 0; i < pm.ba.size(); ++i)
            query += ", s_1." + pm.ba[i];
    query += " FROM " + pm.tab_result + "_Source s_1 JOIN " + pm.tab_result + "_Seq1 s1 ON ";
    for(int i = 0; i < pm.ba.size()-1; ++i)
        query += " s_1." + pm.ba[i] + "=s1." + pm.ba[i] + " AND ";
    query += " s_1." + pm.ba[pm.ba.size()-1] + "=s1." + pm.ba[pm.ba.size()-1]
            + " GROUP BY s1.id,s_1.GRPID";
    if(context_free_BEM)
        for(int i = 0; i < pm.ba.size(); ++i)
            query += ",s_1." + pm.ba[i];
    query += " ORDER BY s1.id";
    mrdb::ResultSet* ris =execQr(query);

    while(ris->next()) {

        id = ris->getInt(1);
        seq_id_i= ris->getInt(2)-min_trackid;

        if(context_free_BEM && id_old!=id) {
            std::vector<std::string> attrVect;
            for(int i = 2; i < 3+pm.ba.size(); ++i)
                attrVect.push_back(ris->getString(i));
            initbemvect(id,attrVect);
        }
        if(((context_free_BEM && bem_vect[id-1]->count(true)==bemCF_constraints) || !context_free_BEM)
                && !context_dep_BEM)
                (bvptr[id-1])->set(seq_id_i,true);

        id_old=id;
    }
    //USCITI DAL CICLO DEVO ANCORA SETTARE IL BIT PER L'ULTIMA COPPIA grpId,elId!
    if( id_old!=-1 &&
        ((context_free_BEM && bem_vect[id-1]->count(true)==bemCF_constraints) || !context_free_BEM)
            && !context_dep_BEM)
            (bvptr[id-1])->set(seq_id_i,true);

    delete ris;

    if(context_dep_BEM)
        createBitVectors_context_dep();
}

void STSMiner::createBitVectors_context_dep() {

    std::vector<std::pair<int, int>* > tmp_pos_l;
    std::vector<Constraints*> c_v;
    std::string query;

    query ="SELECT s1.id, s_1.POS ";
    for(int i = 0; i < attr_list.size(); ++i)
        query += ",s_1." + attr_list[i];

    query += " FROM " + pm.tab_result + "_Source s_1 JOIN " + pm.tab_result + "_Seq1 s1 ON ";

    for(int i = 0; i < pm.ba.size()-1; ++i)
            query += " s_1." + pm.ba[i] + "=s1." + pm.ba[i] + " AND ";

    query += " s_1." + pm.ba[pm.ba.size()-1] + "=s1." + pm.ba[pm.ba.size()-1]
            + " ORDER BY s1.id,s_1.GRPID,s_1.pos";

    mrdb::ResultSet* ris = execQr(query);

    int id_old= -1;

    while(ris->next()) {

        int id = ris->getInt(1);
        int pos = ris->getInt(2);

        if(id>id_old && id_old>0) {
            pos_lists_singleton.push_back(tmp_pos_l);
            tmp_pos_l.clear();
            constr_vect_singleton.push_back(c_v);
            c_v.clear();
        }
        id_old=id;

        int seq_id_i = ris->getInt(3)-min_trackid;
        tmp_pos_l.push_back(new std::pair<int,int>(seq_id_i+min_trackid, pos));

        bool check_bem_cd= false;
        std::vector<std::string> attrVect;
        for(int i = 3; i < 3+attr_list.size(); ++i)
            attrVect.push_back(ris->getString(i));
        c_v.push_back(init_c_v(attrVect, &check_bem_cd));
        if(((context_free_BEM && bem_vect[id-1]->count(true)==bemCF_constraints) || !context_free_BEM) &&
               check_bem_cd )
            (bvptr[id-1])->set(seq_id_i,true);
    }
    constr_vect_singleton.push_back(c_v);
    pos_lists_singleton.push_back(tmp_pos_l);

    /****************
    print_singleton_data();
    /*****************/
    delete ris;
}

Constraints* STSMiner::init_c_v(std::vector<std::string> AttrList, bool* check_bem_cd) {
    Constraints* c = new Constraints();
    c->bem.push_back(initbemvect_context_dep(AttrList));
    *check_bem_cd = *check_bem_cd || c->bem[c->bem.size()-1].count(true)==bemCD_constraints;
    if(active_count)
        updateMidCounters_singleton(c);
    return c;
}

/**
  *This method updates the mid counters the vector of the last bitString inserted in bem
  */
void STSMiner::updateMidCounters_singleton(Constraints* c) {
    std::vector<std::pair<int, int> > mid_c_tmp;
    for(int j=1;j<bem_vect_length-1;++j) {
        int tmp = c->bem[c->bem.size()-1].test(j) ? 1 : 0;
        mid_c_tmp.push_back(std::pair<int,int>(tmp,0));
    }
    c->mid_counters.push_back(mid_c_tmp);
}

BitString STSMiner::initbemvect_context_dep(std::vector<std::string> attrVal) {

    BitString output(bem_vect_length);

    if(pm.seq_bem_vect.size()>0){
        std::string attr_curr_val="";
        int mid_list_counter=0;

        for(int j=0;j<pm.seq_bem_vect.size();++j) {

            attr_curr_val="";
            bool ok=true;
            Bem_cond* it=pm.seq_bem_vect[j];
            bool context_dep = false;
            bool context_free = false;

            do {
                if(!containedIn(it->attr,pm.ba)) {
                    context_dep = true;
                    attr_curr_val = find_val(it->attr,attrVal);
                    ok = ok && checkConstraint(it->op,attr_curr_val,it->val);
                }
                else
                    context_free = true;
                it = it->and_c;
            } while(it);

            /*x vale 0 se i vincoli erano sul BEGIN,bem_vect_length-1 se erano sull'end,
            tra 1 e bem_vect_length-2 se erano su un mid */
            int x;
            if(pm.seq_bem_vect[j]->type.compare("MID")==0)
                x=++mid_list_counter;
            else if (pm.seq_bem_vect[j]->type.compare("BEGIN")==0)
                x = 0;
            else x = bem_vect_length-1;

            if(!context_dep && context_free)
                output.set(x,false);
            else
                output.set(x,ok);
            delete it;
        }
    }
    return output;
}

void STSMiner::initbemvect(int id,std::vector<std::string> attrVal) {

    if(pm.seq_bem_vect.size()>0){
        std::string attr_curr_val="";
        int mid_list_counter=0;

        for(int j=0;j<pm.seq_bem_vect.size();++j) {

            attr_curr_val="";
            bool ok=true;
            Bem_cond* it=pm.seq_bem_vect[j];
            bool context_dep = false;
            bool context_free = false;

            do {
                if(containedIn(it->attr,pm.ba)) {
                    context_free = true;
                    attr_curr_val = find_val(it->attr,attrVal);
                    ok = ok && checkConstraint(it->op,attr_curr_val,it->val);
                }
                else
                    context_dep = true;
                it = it->and_c;
            } while(it);

            /*vale 0 se i vincoli erano sullo BEGIN,bem_vect_length-1 se erano sull'end,
            tra 1 e bem_vect_length-2 se erano su un mid */
            int x;
            if(pm.seq_bem_vect[j]->type.compare("MID")==0)
                x=++mid_list_counter;
            else if (pm.seq_bem_vect[j]->type.compare("BEGIN")==0)
                x = 0;
            else x = bem_vect_length-1;

            if(context_dep && !context_free)
                (bem_vect[id-1])->set(x,false);
            else {
                (bem_vect[id-1])->set(x,ok);
                if(active_count && x!=0 && x!=bem_vect_length-1 && ok) {//siamo in un caso mid
                    mid_count[id-1][mid_list_counter-1].first++;
                    mid_count[id-1][mid_list_counter-1].second++;
                }
            }
        }
    }
}

/**************************************
  SELECT _s1.ID AS ID_1, _s2.ID AS ID_2, _s1.trackid, _s2.POS, _s1.POS [,_s2.POS_D]
  FROM
     (SELECT s_1.id,s1.*
      FROM example_Seq1 s_1
          JOIN
           example_Source s1
          ON  s_1.subarea_id=s1.subarea_id
      ) AS _s1,
      (SELECT s_2.id,s2.*
       FROM example_Seq1 s_2
           JOIN
            example_Source s2
           ON  s_2.subarea_id=s2.subarea_id
       ) AS _s2
    WHERE  _s1.trackid=_s2.trackid AND
           _s2.pos_d-_s1.pos_d>=1+0 AND
           _s2.pos_d-_s1.pos_d<=1+100
    ORDER BY ID_1, ID_2,  _s1.trackid,  _s2.POS, _s1.POS
  *************************************/
bool STSMiner::extract2Sequences() {

    std::string sqlQuery = "CREATE TABLE " + pm.tab_result
            + "_Seq2 (ID SERIAL PRIMARY KEY,ID_1 integer,ID_2 integer);";
    execQuery(sqlQuery);

    sqlQuery = "SELECT _s1.ID AS ID_1, _s2.ID AS ID_2, _s1.GRPID, _s2.POS, _s1.POS ";
    if(DISTINCT)
        sqlQuery += ",_s2.POS_D ";
    sqlQuery += " FROM (SELECT s_1.id,s1.* FROM "
              + pm.tab_result + "_Seq1 s_1 JOIN "+ pm.tab_result + "_Source s1 ON " ;
    for(int i = 0; i < pm.ba.size()-1; ++i)
        sqlQuery += " s_1." + pm.ba[i] + "=s1." + pm.ba[i] + " AND ";
    sqlQuery += " s_1." + pm.ba[pm.ba.size()-1] + "=s1." + pm.ba[pm.ba.size()-1]
              + " ) AS _s1, (SELECT s_2.id,s2.* FROM " + pm.tab_result
              + "_Seq1 s_2 JOIN "+ pm.tab_result + "_Source s2 ON ";
    for(int i = 0; i < pm.ba.size()-1; ++i)
        sqlQuery += " s_2." + pm.ba[i] + "=s2." + pm.ba[i] + " AND ";
    sqlQuery += " s_2." + pm.ba[pm.ba.size()-1] + "=s2." + pm.ba[pm.ba.size()-1] + " ) AS _s2 WHERE "
              + " _s1.GRPID=_s2.GRPID AND ";
    if(DISTINCT)
        sqlQuery += " _s2.pos_d-_s1.pos_d>=1+" + Converter(pm.sequenceAllowedGaps.getMin()).toString()
                + " AND _s2.pos_d-_s1.pos_d-1<=" + Converter(pm.sequenceAllowedGaps.getMax()).toString();
    else
        sqlQuery += " _s2.pos-_s1.pos-1>=" + Converter(pm.sequenceAllowedGaps.getMin()).toString()
                + " AND _s2.pos-_s1.pos-1<=" + Converter(pm.sequenceAllowedGaps.getMax()).toString();

    if(pm.seq_dist_vect.size()>0)
        for(int i=0; i< pm.seq_dist_vect.size();++i) {
            sqlQuery += " AND " + pm.seq_dist_vect[i]->function + "(";
            for(int j=0; j<pm.seq_dist_vect[i]->attr.size();j++)
                sqlQuery += "_s1." + pm.seq_dist_vect[i]->attr[j]+", ";
            for(int j=0; j<pm.seq_dist_vect[i]->attr.size()-1;j++)
                sqlQuery += "_s2." + pm.seq_dist_vect[i]->attr[j]+", ";
            sqlQuery+= "_s2." + pm.seq_dist_vect[i]->attr[pm.seq_dist_vect[i]->attr.size()-1]
                     + ") " + pm.seq_dist_vect[i]->range;
        }
    sqlQuery += " ORDER BY ID_1, ID_2, _s1.GRPID, _s2.POS, _s1.POS ";

    return pruningSeq2(sqlQuery);
}

/***************************************
SELECT s1.ID AS ID_1, s2.ID AS ID_2
FROM example_Seq2 s1, example_Seq2 s2
WHERE s1.ID_2=s2.ID_1
GROUP BY s1.ID,s2.ID
ORDER BY ID_1,ID_2
  *************************************/
bool STSMiner::extractKSequences(int k) {

        std::string strk_1 = Converter(k-1).toString();

        std::string query = "CREATE TABLE " + pm.tab_result + "_Seq" + Converter(k).toString()
                + " (ID SERIAL PRIMARY KEY,ID_1 integer,ID_2 integer)";
        execQuery(query);

        query = "SELECT s1.ID AS ID_1, s2.ID AS ID_2 FROM " + pm.tab_result + "_Seq" + strk_1 + " s1, "
                 + pm.tab_result + "_Seq" + strk_1
                 + " s2 WHERE s1.ID_2=s2.ID_1 GROUP BY s1.ID,s2.ID ORDER BY ID_1,ID_2 ";

        return pruningSeqK(k,query);
}

bool STSMiner::pruningSeq2(std::string sqlQuery){
    std::vector<int> pos_d_l;
    std::vector<std::vector<int> > prefix_pos_l;
    std::vector<int> new_prefix_list;
    Constraints* constr;
    std::vector<Constraints*> c_v;
    std::vector<bool> closed_seq;
    std::vector<std::pair<int,int>* > ris_l;
    BitString new_bem_string;
    std::vector<std::pair<int,int> > new_mid_count_vect;
    int size_before = constr_vect.size();
    int size_before_results = results.size();
    int size_before_bvptr = bvptr.size();
    int size_before_pos = pos_lists.size();
    int size_before_prefix_pos = prefix_pos_lists.size();
    int size_before_bem_vect = 0;
    int size_before_mid_count = 0;
    if(context_free_BEM) {
        size_before_bem_vect = bem_vect.size();
        if(active_count)
            size_before_mid_count = mid_count.size();
    }
    int size_before_pos_d = 0;
    if(DISTINCT)
        size_before_pos_d = pos_d_lists.size();

    int id1_old = -1;
    int id2_old = -1;
    int pos_old = -1;
    int grp_old = -1;
    int prefix_pos_old = -1;
    int id1,id2;
    bool check_bem=true;

    BitString bem_cf(bem_vect_length);
    BitString bem_cd(bem_vect_length);

    mrdb::ResultSet* rs = execQr(sqlQuery);

    for(int i = 0; i < bvptr.size(); ++i)
       closed_seq.push_back(true);

    bool first = true ;
    bool check_bem_cd = false;
    BitString tmp0(max_trackid-min_trackid+1);//tmp0 lo setto solo se i vincoli sono rispettati
    int frequence=0;//mi conta il supporto visto che devo salvare anche le seq che non rispettano i vicnoli ma hanno enough support
    if(context_dep_BEM)
        constr = new Constraints();

    while(rs->next()) {

        id1 = rs->getInt(1);
        id2 = rs->getInt(2);
        int grp=rs->getInt(3)-min_trackid;
        int pos = rs->getInt(4);
        int prefix_pos = rs->getInt(5);
        int pos_d;

        if(DISTINCT)
            pos_d = rs->getInt(6);

        bool pair_change=false;

        if(id1!=id1_old || id2!=id2_old) {
            bool save_pattern = !first && (frequence>=pm.sup*grpId_count);
            pair_change=true;//this was into following if... but the pair has changed anyway...

            if(save_pattern) {
                if(context_dep_BEM)
                    c_v.push_back(constr);
                if(context_dep_dist)
                    prefix_pos_l.push_back(new_prefix_list);
                saveSequenceData(2,id1_old,id2_old,&check_bem,tmp0,ris_l,pos_d_l,&new_bem_string,
                                 &closed_seq,prefix_pos_l,new_mid_count_vect,c_v);
            }
            frequence=0;
            new_mid_count_vect.clear();
            ris_l.clear();
            new_prefix_list.clear();
            prefix_pos_l.clear();
            if(context_dep_BEM)
                constr= new Constraints();
            c_v.clear();
            tmp0.clear();
            pos_d_l.clear();
            check_bem=true;
            check_bem_cd=false;
        }
        id1_old = id1;
        id2_old = id2;

        if(context_free_BEM && ( first || pair_change))
            init_bem_bit_vector(2,id1,id2, &check_bem, &bem_cf,&new_bem_string,&new_mid_count_vect);

        if(pair_change || grp != grp_old || pos != pos_old) {
            frequence++;
            ris_l.push_back(new std::pair<int,int>(grp+min_trackid,pos));
            if(DISTINCT)
                pos_d_l.push_back(pos_d);
            if(!pair_change && !first) {
                if(context_dep_dist) {
                    prefix_pos_l.push_back(new_prefix_list);
                    new_prefix_list.clear();
                }
                if(context_dep_BEM) {
                    c_v.push_back(constr);
                    constr= new Constraints();
                }
            }
            if(context_dep_dist)
                new_prefix_list.push_back(prefix_pos);
            if(context_dep_BEM)
                updateBemData(constr,id1,id2,prefix_pos,pos,grp,&check_bem_cd,&bem_cd);
            if(check_bem && (check_bem_cd || !context_dep_BEM))
                tmp0.set(grp,true);
        }
        else if(prefix_pos_old != prefix_pos) {
            if(context_dep_dist)
                new_prefix_list.push_back(prefix_pos);
            if(context_dep_BEM)
                updateBemData(constr,id1,id2,prefix_pos,pos,grp,&check_bem_cd,&bem_cd);
        }
        grp_old = grp;
        pos_old = pos;
        first = false ;
    }

    if(frequence>=pm.sup*grpId_count) {
        if(context_dep_BEM)
            c_v.push_back(constr);
        if(context_dep_dist)
            prefix_pos_l.push_back(new_prefix_list);
        saveSequenceData(2,id1_old,id2_old,&check_bem,tmp0,ris_l,pos_d_l,
                         &new_bem_string,&closed_seq,prefix_pos_l,new_mid_count_vect,c_v);
    }

    if(min_seq_length<2 && pm.seq_dist_vect.size()==0 && max_MIN_COUNT==1)
        for(int i = 0; i < closed_seq.size() ; ++i)
            if(closed_seq[i] && bvptr[i]->count(true)>=pm.sup*grpId_count)
                saveSingleton(i+1);

    delete rs;

    resetStructs(2,size_before_results,
                 size_before_bvptr,
                 size_before_pos,
                 size_before_prefix_pos,
                 size_before_bem_vect,
                 size_before_mid_count,
                 size_before_pos_d,
                 size_before);

    /********************
    print_structs_values();
    /********************/

    bool go_on= bem_cf.count(true)==bemCF_constraints && bem_cd.count(true)==bemCD_constraints;

    if(bvptr.size()>0 && go_on)
       return false;

    return true;
}

void STSMiner::updateBemData(Constraints* constr,int id1,int id2,int prefix_pos,
                             int pos,int grp,bool* check_bem_cd,BitString* bem_cd) {

    int i = findIndex(id1,prefix_pos,grp+min_trackid);
    int j = findIndex(id2,pos,grp+min_trackid);
    constr->bem.push_back(inferBem(id1,id2,i,j));
    bool check_counters = true;
    if(active_count) {
        updateMidCounters(constr,id1,id2,i,j);
        check_counters = checkMidCounters(constr->mid_counters[constr->mid_counters.size()-1]);
     }
    *check_bem_cd = *check_bem_cd || (constr->bem[constr->bem.size()-1].count(true)==bemCD_constraints && check_counters);
    check_go_on(constr,bem_cd);
}

/**
  *This method infers the new pair of mid counters for the pattern
  *composed by id1 and id2
  */
void STSMiner::updateMidCounters(Constraints* constr,int id1,int id2, int i, int j) {
    std::vector<std::pair<int,int> > tmp;
    for(int h =1; h< bem_vect_length-1; ++h) {
        int mc_1 = constr_vect_singleton[id1-1][i]->mid_counters[0][h-1].first;
        int mc_2 = constr_vect_singleton[id2-1][j]->mid_counters[0][h-1].first;
        tmp.push_back(std::pair<int,int>(mc_1,mc_2));
    }
    constr->mid_counters.push_back(tmp);
}

bool STSMiner::checkMidCounters(std::vector<std::pair<int,int> >mc_v) {
    int mc_ok=true;
    for(int i=0;i<mc_v.size();++i) {
        mc_ok = mc_ok && ((mc_v[i].first+mc_v[i].second)<=min_max_mid_count[i].getMax())
                && ((mc_v[i].first+mc_v[i].second)>=min_max_mid_count[i].getMin());
    }
    return mc_ok;
}

/**
  *This method checks the last bem bitsString inserted in c->bem and eventually its mid counters
  */
void STSMiner::check_go_on(Constraints* c,BitString* ris) {
    BitString bs_cd = c->bem[c->bem.size()-1];
    if(!active_count)
        ris->operator|=(bs_cd);
    else {
        ris->set(0,(ris->test(0) || bs_cd.test(0)));
        for(int i=1; i<bs_cd.size()-1; ++i) {
            int mc = c->mid_counters[c->mid_counters.size()-1][i-1].first
                       + c->mid_counters[c->mid_counters.size()-1][i-1].second;
            bool mc_ok = mc<=min_max_mid_count[i-1].getMax();
            ris->set(i,(ris->test(i) || (bs_cd.test(i) && mc_ok)));
        }
        ris->set(bs_cd.size()-1,(ris->test(bs_cd.size()-1) || bs_cd.test(bs_cd.size()-1)));
    }
}

BitString STSMiner::inferBem(int id1,int id2, int i, int j) {

    BitString output(bem_vect_length);
    BitString t(constr_vect_singleton[id1-1][i]->bem[0]);
    BitString t2(constr_vect_singleton[id2-1][j]->bem[0]);
    output.set(0,t.test(0));
    output.set(bem_vect_length-1,t2.test(bem_vect_length-1));
    for(int k=1;k<bem_vect_length-1;++k)
        output.set(k,(t.test(k) || t2.test(k)));
    return output;
}

int STSMiner::findIndex(int id, int pos, int grpId) {
    for(int j = 0; j < pos_lists_singleton[id-1].size(); ++j)
        if(pos_lists_singleton[id-1][j]->first==grpId && pos_lists_singleton[id-1][j]->second==pos)
            return j;
    return -1;
}

bool STSMiner::pruningSeqK(int k,std::string sqlQuery) {

    std::vector<Constraints*> c_v;
    std::vector<std::vector<int> > new_prefix_pos_list;
    std::vector<bool> closed_seq;
    std::vector<std::pair<int,int>* > ris_l;
    BitString new_bem_string;
    std::vector<std::pair<int,int> > new_mid_count_vect;
    int size_before = constr_vect.size()>0 ? constr_vect.size() : 0;
    int size_before_results = results.size();
    int size_before_bvptr = bvptr.size();
    int size_before_pos = pos_lists.size();
    int size_before_prefix_pos = context_dep_dist ? prefix_pos_lists.size() : 0;
    int size_before_bem_vect = 0;
    int size_before_mid_count = 0;
    if(context_free_BEM) {
        size_before_bem_vect = bem_vect.size();
        if(active_count)
            size_before_mid_count = mid_count.size();
    }
    int size_before_pos_d = DISTINCT ? pos_d_lists.size() : 0;
    int id1,id2;
    bool check_bem = false;
    BitString bem_cf(bem_vect_length);
    BitString bem_cd(bem_vect_length);

    mrdb::ResultSet* rs = execQr(sqlQuery);

    reset_seqi_cache(k);

    for(int i = 0; i < results.size(); ++i)
            closed_seq.push_back(true);

    BitString tmp0(max_trackid-min_trackid+1);
    std::vector<int> new_pos_d_l;

    while(rs->next()) {

        id1 = rs->getInt(1);
        id2 = rs->getInt(2);

        if(context_free_BEM)
            init_bem_bit_vector(k,id1,id2, &check_bem, &bem_cf,&new_bem_string,&new_mid_count_vect);
        check_bem = !context_free_BEM || check_bem;

        if(check_pos_lists_with_gap(id1,id2,&ris_l, &tmp0, &new_prefix_pos_list, &new_pos_d_l, &check_bem,&c_v, &bem_cd) ) {
            saveSequenceData(k,id1,id2,check_bem,tmp0,ris_l,new_pos_d_l,&new_bem_string,
                             &closed_seq,new_prefix_pos_list,new_mid_count_vect,c_v);
        }
        new_prefix_pos_list.clear();
        new_pos_d_l.clear();
        new_mid_count_vect.clear();
        tmp0.clear();
        ris_l.clear();
        c_v.clear();
    }

    if(min_seq_length<k)
     for(int i = 0; i < closed_seq.size() ; ++i) {
         if(closed_seq[i] && bvptr[i]->count(true) >= pm.sup*grpId_count) {
             double sup = bvptr[i]->count(true)/(double)grpId_count;
             saveSequence(i+1,results[i],sup);
         }
     }
     delete rs;

     resetStructs(k,
                  size_before_results,
                  size_before_bvptr,
                  size_before_pos,
                  size_before_prefix_pos,
                  size_before_bem_vect,
                  size_before_mid_count,
                  size_before_pos_d,
                  size_before);

    /********************
    print_structs_values();
    /********************/
    bool go_on= (bem_cf.count(true)==bemCF_constraints) && (bem_cd.count(true)==bemCD_constraints);

    if(bvptr.size()>0 && go_on)
        return false;
    return true;
}

void STSMiner::resetStructs(int k,
                            int size_before_results,
                            int size_before_bvptr,
                            int size_before_pos,
                            int size_before_prefix_pos,
                            int size_before_bem_vect,
                            int size_before_mid_count,
                            int size_before_pos_d,
                            int size_before) {
    if(k>0)
        finalize_seqi(k);
    results.erase(results.begin(),results.begin()+size_before_results);

    deleter(bvptr.begin(),bvptr.begin()+size_before_bvptr); //delete objects
    bvptr.erase(bvptr.begin(),bvptr.begin()+size_before_bvptr);

    for(int i = 0; i < size_before_pos; i++) {
        deleter(pos_lists[i].begin(),pos_lists[i].end()); //delete objects
    }
    pos_lists.erase(pos_lists.begin(),pos_lists.begin()+size_before_pos);

    if(context_dep_dist)
        prefix_pos_lists.erase(prefix_pos_lists.begin(),prefix_pos_lists.begin()+size_before_prefix_pos);

    if(context_free_BEM) {
        deleter(bem_vect.begin(),bem_vect.begin()+size_before_bem_vect); //delete objects
        bem_vect.erase(bem_vect.begin(),bem_vect.begin()+size_before_bem_vect);
        if(active_count)
            mid_count.erase(mid_count.begin(),mid_count.begin()+size_before_mid_count);
    }

    if(DISTINCT)
        pos_d_lists.erase(pos_d_lists.begin(),pos_d_lists.begin()+size_before_pos_d);

    if(context_dep_BEM) {
        for(int i = 0; i < size_before; i++) {
            deleter(constr_vect[i].begin(),constr_vect[i].end()); //delete objects
        }
        constr_vect.erase (constr_vect.begin(),constr_vect.begin()+size_before);
    }
    if(k>2)
        dropTable(pm.tab_result+"_Seq"+Converter(k-1).toString());
    else
        dropTable(pm.tab_result+"_Source");
}

void STSMiner::saveSequenceData(int k,int& id1, int& id2, bool check_bem,
                      BitString tmp0,
                      std::vector<std::pair<int,int>* > ris_l,
                      std::vector<int> pos_d_l,
                      BitString* new_bem_string,
                      std::vector<bool>* closed_seq,
                      std::vector<std::vector<int> > prefix_pos_l,
                      std::vector<std::pair<int,int> > new_count,
                      std::vector<Constraints*> c_v) {

    BitString tmp(tmp0);
    if(check_bem || (!check_bem && !checkBem(id1,k))) {
        BitString tmp_x(*bvptr[id1-1]);
        if(tmp_x.count(true)==tmp.count(true))
            (*closed_seq)[id1-1] = false;
    }
    if(check_bem || (!check_bem && !checkBem(id2,k))) {
        BitString tmp_y(*bvptr[id2-1]);
        if(tmp_y.count(true)==tmp.count(true))
            (*closed_seq)[id2-1] = false;
    }

    if(k==2) {
        std::vector<std::string> t;
        t.push_back(Converter(id1).toString());
        t.push_back(Converter(id2).toString());
        results.push_back(t);
    }
    else
        results.push_back(copy_and_merge(results[id1-1],results[id2-1]));

    bvptr.push_back(new BitString(tmp0));
    pos_lists.push_back(ris_l);

    if(context_dep_dist)
        prefix_pos_lists.push_back(prefix_pos_l);
    if(DISTINCT)
        pos_d_lists.push_back(pos_d_l);
    if(context_free_BEM) {
        bem_vect.push_back(new BitString(*new_bem_string));
        if(active_count)
            mid_count.push_back(new_count);
    }
    if(context_dep_BEM)
        constr_vect.push_back(c_v);

    seqicache << id1 << "\t"<< id2 << std::endl;
}


bool STSMiner::checkBem(int id, int k) {

    bool check_mid_count= true;
    bool cf = true;
    if(context_free_BEM) {
        if(active_count) {
            for(int i=1;i<bem_vect_length-1;++i) {
                int mc = mid_count[id-1][i-1].first + mid_count[id-1][i-1].second;
                check_mid_count = check_mid_count && mc>=min_max_mid_count[i-1].getMin() && mc<=min_max_mid_count[i-1].getMax();
            }
        }
        cf = ((bem_vect[id-1]->count(true) == bemCF_constraints) && check_mid_count);
    }

    bool cd = context_dep_BEM ? false : true;
    if(context_dep_BEM) {//aggiungi check sul count!!!!!
        if(k>2){
            int i=0;
            do {
                for(int j=0; j<constr_vect[id-1][i]->bem.size(); ++j) {
                    bool mc_ok = true;
                    if(active_count)
                        for(int h=1; h<constr_vect[id-1][i]->mid_counters.size(); ++h) {
                            int mc = constr_vect[id-1][i]->mid_counters[j][h-1].first
                                   + constr_vect[id-1][i]->mid_counters[j][h-1].second;
                            mc_ok = mc_ok && mc<=min_max_mid_count[h-1].getMax();
                        }
                    cd = cd || ((constr_vect[id-1][i]->bem[j]).count(true)==bemCD_constraints && mc_ok);
                }
                ++i;
            } while(!cd && i<constr_vect[id-1].size());
        }
        else{
            int i=0;
            do {
                for(int j=0; j<constr_vect_singleton[id-1][i]->bem.size(); ++j) {
                    bool mc_ok = true;
                    if(active_count)
                        for(int h=1; h<constr_vect_singleton[id-1][i]->mid_counters.size(); ++h) {
                            int mc = constr_vect_singleton[id-1][i]->mid_counters[j][h-1].first
                                   + constr_vect_singleton[id-1][i]->mid_counters[j][h-1].second;
                            mc_ok = mc_ok && mc<=min_max_mid_count[i-1].getMax();
                        }
                    cd = cd || ((constr_vect_singleton[id-1][i]->bem[j]).count(true)==bemCD_constraints && mc_ok);
                }
                ++i;
            } while(!cd && i<constr_vect_singleton[id-1].size());
        }
    }

    return cf && cd;
}


void STSMiner::init_bem_bit_vector(int k,int id1,int id2,bool* check_bem,
                         BitString* bem_cf,
                         BitString* new_bem_vect,
                         std::vector<std::pair<int,int> >* new_count) {

    BitString t(*bem_vect[id1-1]);
    BitString t2(*bem_vect[id2-1]);

    new_bem_vect->set(0,t.test(0));
    new_bem_vect->set(bem_vect_length-1,t2.test(bem_vect_length-1));
    for(int i=1;i<bem_vect_length-1;++i){
        new_bem_vect->set(i,(t.test(i) || t2.test(i)));//se c'è almeno un mid-i...
        if(!active_count)
            bem_cf->set(i,bem_cf->test(i) || (t.test(i) || t2.test(i)));
    }
    bem_cf->set(0,bem_cf->test(0) || t.test(0));
    bem_cf->set(bem_vect_length-1,bem_cf->test(bem_vect_length-1) || t2.test(bem_vect_length-1));

    bool check_mid_count=true;
    if(active_count) {
        int mc;

        for(int i=1;i<bem_vect_length-1;++i) {
            std::pair<int,int> temp(0,0);
            //CALCOLIAMO IL COUNT DEL MID-i
            if(k==2) {
                if(t.test(i))
                    temp.first = 1;
                if(t2.test(i))
                    temp.second = 1;
            }
            else {
                temp.first = mid_count[id1-1][i-1].first + mid_count[id1-1][i-1].second;
                temp.second = mid_count[id2-1][i-1].second;
            }
            new_count->push_back(temp);
            mc=temp.first+temp.second;
            check_mid_count = check_mid_count && mc>=min_max_mid_count[i-1].getMin() && mc<=min_max_mid_count[i-1].getMax();
            //se tutti gli el.i hanno già passato il count non si potranno formare altre seq valide!
            bem_cf->set(i, bem_cf->test(i) || ((t.test(i) || t2.test(i)) && mc<=min_max_mid_count[i-1].getMax()));
        }
    }
    //non solo deve esserci almeno un mid ma deve avere almeno un elemento con count <= al max count
    //else significa che ha elemnti mid ma che ne ha troppi e non genererà mai una seq valida!
   *check_bem=(new_bem_vect->count(true) == bemCF_constraints) && check_mid_count;
}


bool STSMiner::check_pos_lists_with_gap(int id1, int id2,std::vector<std::pair<int,int>* >* ris_l,
                                        BitString* bv_constr,
                                        std::vector<std::vector<int> >* new_prefix_pos_list,
                                        std::vector<int>* new_pos_d_l,
                                        bool* BEM_context_free,
                                        std::vector<Constraints*>* c_v,
                                        BitString* bem_cd) {
/*
        std::cout<< "Checking pos lists..." <<std::endl;
        std::cout<<"POS_LIST of ID "+Converter(id1).toString()+":"<<std::endl;
        std::cout<<pos_list_string(pos_lists[id1-1]);
        std::cout<<std::endl;
        std::cout<<"POS_LIST of ID "+Converter(id2).toString()+":"<<std::endl;
        std::cout<<pos_list_string(pos_lists[id2-1]);
        std::cout<<std::endl;
*/
    std::vector<std::pair<int,int>* >::iterator it1 = pos_lists[id1-1].begin();
    std::vector<std::pair<int,int>* >::iterator it2 = pos_lists[id2-1].begin();
    std::vector<std::pair<int,int>* >::iterator end1 = pos_lists[id1-1].end();
    std::vector<std::pair<int,int>* >::iterator end2 = pos_lists[id2-1].end();
    std::vector<int >::iterator it1d, it2d;
    if(DISTINCT) {
        it1d = pos_d_lists[id1-1].begin();
        it2d = pos_d_lists[id2-1].begin();
    }
    int grp1, grp2;
    int min_gap = pm.sequenceAllowedGaps.getMin();
    int max_gap = pm.sequenceAllowedGaps.getMax();
    int it2_c = 0;
    bool check_bem=false;
    BitString bv(max_trackid-min_trackid+1);
    std::vector<int> prefix_l;

    while(it2 != end2) {
        grp2 = (*it2)->first;

        while(it1 != end1 && (*it1)->first < grp2) {
            it1++;
            it1d++;
        }
        if(it1 == end1)
            break;

        grp1 = (*it1)->first;

        while(it2 != end2 && grp1 > (*it2)->first ) {
            it2++;
            it2_c++;
            it2d++;
        }
        if(it2 == end2)
            break;

        grp2 = (*it2)->first;

        if(grp1 == grp2) {

            std::vector<std::pair<int,int>* >::iterator it1_start_grp = it1;
            std::vector<int>::iterator it1d_start_grp;
            if(DISTINCT)
                it1d_start_grp = it1d;
            int pos2 = DISTINCT ? *it2d : (*it2)->second;
            int pos1 = DISTINCT ? *it1d : (*it1)->second;

            if(pos1<pos2) {
                while(pos2-max_gap-1 > pos1) {
                    if(it1+1 != end1 && (*(it1+1))->first == grp1) {
                        ++it1;
                        ++it1d;
                        pos1 = DISTINCT ? *it1d : (*it1)->second;
                    }
                    else break;
                }
                //devi controllare perchè è uscito.
                if(it1 != end1 &&
                        (*it1)->first == grp1 &&
                        pos1 >= pos2-max_gap-1 &&
                        pos1 <= pos2-min_gap-1) {
                    //se è uscito perchè ha trovato la prima pos che rispetta il gap...
                    bool insert = true;
                    bool BEM_context_dep= context_dep_BEM ? false : true;
                    if(context_dep_dist || context_dep_BEM) {
                        //se non ci sono distanze contestuali basta che ci sia un singolo it1 che va bene rispetto al gap
                        //else ci serve controllare che it1 sia uguale ad uno dei prefix di it2 ed aggiungere id1 alla
                        //lista dei prefissi
                        do{
                            bool valid_prefix= false;
                            if(context_dep_dist) {
                                for(int i = 0; i<prefix_pos_lists[id2-1][it2_c].size(); ++i)
                                    if(pos1 == prefix_pos_lists[id2-1][it2_c][i]) {
                                        prefix_l.push_back(pos1);
                                        valid_prefix = true;
                                    }
                                insert = prefix_l.size()>0;
                            }
                            if(context_dep_BEM && (!context_dep_dist || (context_dep_dist && valid_prefix))) {
                                Constraints* temp= new Constraints();
                                int it1_c = it1 - pos_lists[id1-1].begin();
                                BEM_context_dep = BEM_context_dep || init_bemCD(id1, it1_c, id2,
                                                     (*it2)->second, grp2, temp, bem_cd);
                                (*c_v).push_back(temp);
                            }
                            it1++;
                            it1d++;
                            if(it1 != end1)
                                pos1 = DISTINCT ? *it1d : (*it1)->second;

                        } while(it1 != end1 && (*it1)->first == grp1 && pos2-min_gap-1 <= pos1);
                    }
                    if(insert) {
                        bv.set(grp2-min_trackid,true);
                        (*ris_l).push_back(new std::pair<int,int>(grp2,(*it2)->second));
                        if(DISTINCT)
                            new_pos_d_l->push_back(*it2d);
                        if(context_dep_dist) {
                            new_prefix_pos_list->push_back(prefix_l);
                            prefix_l.clear();
                        }
                        if(*BEM_context_free && BEM_context_dep) {
                            bv_constr->set(grp2-min_trackid,true);
                            check_bem= true;
                        }
                    }
                }
            }
            it1 = it1_start_grp;
            it1d = it1d_start_grp;
            it2++;
            it2d++;
            it2_c++;
        }
    }

/*
    std::cout<<"POS_LIST result: " + Converter(id2).toString() + ":"<<std::endl;
    std::cout<<pos_list_string(*ris_l);
    std::cout<<std::endl;
*/

    (*BEM_context_free) = check_bem;

    if( bv.count(true)>= (pm.sup*grpId_count))
        return true;
    return false;
}

bool STSMiner::init_bemCD(int id1, int index_id1, int id2, int pos2, int grpId, Constraints* output,BitString* bem_cd) {
    //ci dice se almeno una delle bs inferite è tutta settata
    bool at_least_one = false;
    //1.PRENDI LE BS[] ASSOCIATE AD ID1, IN TRACK_i CON POS1
    std::vector<BitString> bs1= constr_vect[id1-1][index_id1]->bem;

    int id2_singleton= (int)Converter(results[id2-1][results[id2-1].size()-1]).toLong();
    //2.PRENDI LA BS DI ID2
    int j= findIndex(id2_singleton,pos2,grpId);
    BitString bs2= constr_vect_singleton[id2_singleton-1][j]->bem[0];
    //3.CICLA SU BS1[]
    for(int i = 0; i<bs1.size(); ++i) {
        BitString temp = inferBemCD(bs1[i],bs2);
        output->bem.push_back(temp);
        bool mc_ok = true;
        if(active_count) {
            //per ogni bit mid di temp...
            std::vector<std::pair<int,int> >new_mc_v ;
            for(int h=1; h<bem_vect_length-1; ++h){
                int mc2 = constr_vect_singleton[id2_singleton-1][j]->mid_counters[0][h-1].first;
                int mc1 = constr_vect[id1-1][index_id1]->mid_counters[i][h-1].first
                        + constr_vect[id1-1][index_id1]->mid_counters[i][h-1].second;
                new_mc_v.push_back(std::pair<int,int>(mc1,mc2));
                mc_ok = mc_ok && ((mc1+mc2)<=min_max_mid_count[h-1].getMax()) && (mc1+mc2>=min_max_mid_count[h-1].getMin());
            }
            output->mid_counters.push_back(new_mc_v);
        }
        at_least_one= at_least_one || (temp.count(true)==bemCD_constraints && mc_ok);
        check_go_on(output,bem_cd);
    }
    return at_least_one;
}

BitString STSMiner::inferBemCD(BitString bs1,BitString bs2) {
    BitString output(bem_vect_length);
    output.set(0,bs1.test(0));
    output.set(bem_vect_length-1,bs2.test(bem_vect_length-1));
    for(int k=1;k<bem_vect_length-1;++k)
        output.set(k,(bs1.test(k) || bs2.test(k)));

    return output;
}

void STSMiner::saveSequences(int k){

    if(k==1 && min_seq_length<2 && pm.seq_dist_vect.size()==0 && max_MIN_COUNT==1)
        for(int i = 0; i < bvptr.size() ; ++i)
            if(bvptr[i]->count(true)>=pm.sup*grpId_count)
                saveSingleton(i+1);

    if(k>1 && min_seq_length<=k)
     for(int i = 0; i < bvptr.size() ; ++i)
         if(bvptr[i]->count(true) >= pm.sup*grpId_count) {
             double sup = bvptr[i]->count(true)/(double)grpId_count;
             saveSequence(i+1,results[i],sup);
         }
}

void STSMiner::delete_all(int k) {
    int size_before = constr_vect.size()>0 ? constr_vect.size() : 0;
    int size_before_results = results.size();
    int size_before_bvptr = bvptr.size();
    int size_before_pos = pos_lists.size();
    int size_before_prefix_pos = context_dep_dist ? prefix_pos_lists.size() : 0;
    int size_before_bem_vect = 0;
    int size_before_mid_count = 0;
    if(context_free_BEM) {
        size_before_bem_vect = bem_vect.size();
        if(active_count)
            size_before_mid_count = mid_count.size();
    }
    int size_before_pos_d = DISTINCT ? pos_d_lists.size() : 0;

    resetStructs(k,
                 size_before_results,
                 size_before_bvptr,
                 size_before_pos,
                 size_before_prefix_pos,
                 size_before_bem_vect,
                 size_before_mid_count,
                 size_before_pos_d,
                 size_before);
    //DELETE SINGLETON STRUCTS!
    if(context_dep_BEM) {
        for(int i = 0; i < pos_lists_singleton.size(); i++)
            deleter(pos_lists_singleton[i].begin(),pos_lists_singleton[i].end()); //delete objects
        pos_lists_singleton.erase(pos_lists_singleton.begin(),pos_lists_singleton.begin()+size_before_pos);

        for(int i = 0; i < constr_vect_singleton.size(); i++)
            deleter(constr_vect_singleton[i].begin(),constr_vect_singleton[i].end()); //delete objects
        constr_vect_singleton.erase (constr_vect_singleton.begin(),constr_vect_singleton.begin()+size_before);
    }
}

void STSMiner::dropResultsTables(int k) {

    dropTable(pm.tab_result +"_Seq");
    dropTable(pm.tab_result +"_Seq_support");
    dropTable(pm.tab_result +"_Seq1");
    dropTable(pm.tab_result +"_matcher");

    dropTable(pm.tab_result +"_Seq"+ Converter(k).toString());
    dropTable(pm.tab_result +"_Seq"+ Converter(k-1).toString());
}


void STSMiner::mineRules () {

    int k = 3;

    try {
        MRLogPop();
        std::cout<<std::endl;
        MRLogPush("Starting STSMiner mining algorithm...");
        dropTable(pm.tab_result+"_matcher");

        std::string strK;
        bool EmptyResult = false;

        if(log)
            MRLog() << "Extracting 1-Sequences set..." << std::endl;

        if(!extract1Sequences()) {

            int singleton_num = pruning1Seq();
            if(singleton_num>0) {
                createBitVectors();
                if(singleton_num>1 && max_seq_length>1) {
                    if(log)
                        MRLog() << "Extracting 2-Sequences set..."<<std::endl;
                    EmptyResult = extract2Sequences();
                    if(!EmptyResult && max_seq_length==2) {
                        saveSequences(2);
                        delete_all(2);
                    }

                    while(!EmptyResult && k <= max_seq_length) {
                        if(k%10 == 0)
                            MRLog()<<"Please wait..."<<std::endl;
                        strK = Converter(k).toString();
                        if(log)
                            MRLog() << "Extracting " + strK + "-Sequences set..."<<std::endl;
                        EmptyResult = extractKSequences(k);

                        if(EmptyResult)
                            MRLog()<< "Cannot generate sequences of length " + strK << std::endl;
                        else if(k==max_seq_length) {
                            saveSequences(k);
                            delete_all(k);
                        }
                        ++k;
                    }
                }
                else {
                    saveSequences(1);
                    delete_all(1);
                }
            }//nothing generated...
            else
                delete_all(1);
        }//Source table is empty
        else
            k=2;
        std::string empty_t_name=pm.tab_result +"_Seq"+ Converter(k-1).toString();
        dropTable(empty_t_name);
        dropTable(pm.tab_result+"_Source");
        MRLogPop();
        MRLogPush("Saving frequent patterns...");
        connection.finalize(true);
        finalize_matcher(seq_count>1);
        MRLogPop();
        show_Results(log);
        std::cout<<std::endl;
        MRLog()<<"Extracted "+Converter(seq_count-1).toString()+" sequences."<<std::endl;
    } catch (const mrdb::SQLException& e) {
        std::cout<<StringUtils::toBoldRed("MRDB Exception:");
        std::cout<<e.what() <<std::endl;
        dropResultsTables(k);
        delete_all(-1);
        throw e;
    } catch (const minerule::MineruleException& e) {
        std::cout<<StringUtils::toBoldRed("Minerule Exception:");
        std::cout<<e.what()<<std::endl;
        dropResultsTables(k);
        delete_all(-1);
        throw e;
    } catch (const std::exception& e) {
        std::cout<<StringUtils::toBoldRed("Exception:");
        std::cout<<e.what()<<std::endl;
        dropResultsTables(k);
        delete_all(-1);
        throw e;
    }	catch(...) {
        std::cout<<StringUtils::toBoldRed("An unknown exception has been thrown... \n ");
        dropResultsTables(k);
        delete_all(-1);
        throw std::exception();
    }
}

}//end namespace

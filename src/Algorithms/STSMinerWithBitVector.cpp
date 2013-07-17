/*************************************/
/*	 STSequences miner    by Gloria  */
/*************************************/

/* La seguente classe crea un oggetto STSMiner il quale data la MINE SEQUENCE in input elabora
in automatico delle query SQL per estrarre le sequenze frequenti dal db delegandogli così il grosso del lavoro*/

/*
L'algoritmo accetta un attributo per l'ordinamento ed una attributo per gli elementi e restituisce
sequenze di coppie <el_attr,ord_attr> che rispettano il vincolo del supporto e del gap inteso come gap
tra i valori dell'attributo di ordinamento tra due elementi(coppie) successivi della sequenza.

QUERY MODELLO:

MINE SEQUENCE dest_table AS SELECT
el_attr, SUPPORT
WITH GAP min..max
FROM table
GROUP BY group_attr
ORDER BY ord_attr
EXTRACTING SEQUENCES WITH SUPPORT: x
*/

/*da correggere: la tipologia fissa degli elementi in Source non va molto bene... lines 25-27
leggi il tipo e scegli il corrispettivo sql*/

#include "minerule/Algorithms/STSMinerWithBitVector.h"

namespace minerule {

/*Questo metodo estrae dalla tabella gli elementi singoli*/
bool STSMinerWithBitVector::extract1Sequences() {

    std::string dropQuery = " DROP TABLE IF EXISTS Seq1;DROP TABLE IF EXISTS Seq2;";
                dropQuery += "DROP SEQUENCE IF EXISTS id2_sq; DROP TABLE IF EXISTS Source; DROP SEQUENCE IF EXISTS idsrc_sq;DROP SEQUENCE IF EXISTS id1_sq;";
    STSMinerWithBitVector::execQr(dropQuery);

    createSeqQ(1);
    std::string createTableQuery = "CREATE TABLE Source ( "
            + AttrList.at(0) + " integer, "
            + AttrList.at(1) + " numeric, "
            + AttrList.at(2) + " character varying(10));";
    execQr(createTableQuery);

    std::string sqlQuery = "INSERT INTO Source SELECT ";

    for(int i=0; i<AttrList.size()-1;i++)
        sqlQuery= sqlQuery + AttrList.at(i) + " , ";

     sqlQuery= sqlQuery  + AttrList.at(AttrList.size()-1);
     sqlQuery= sqlQuery  + " FROM "+ pm.tab_source + " "
                         + " ORDER BY "+ordAttrList
                         + " , "+groupAttrList+";";
    execQr(sqlQuery);

    return emptyTable("Source");
}

/*Filtriamo le 1-sequenze tenendo solo quelle con supporto sufficiente*/
bool STSMinerWithBitVector::pruning1Seq(){

    /*LE SEGUENTI RIGHE GENERANO UNA TAB A PARTE PER LE SEQUENZE LUNGHE 1 CON SUPPORTO SUFFICENTE*/
    std::string sqlQuery = "DROP TABLE IF EXISTS Seq1; CREATE TABLE Seq1";
    sqlQuery += "("+ ordAttrList + " numeric, "+ AttrList.at(2) + " character varying(10),";
    sqlQuery += "ID integer NOT NULL DEFAULT nextval('id1_sq'::regclass) PRIMARY KEY);";
    execQr(sqlQuery);

    sqlQuery = "INSERT INTO Seq1 SELECT "+ordAttrList+","+ AttrList.at(2);
    sqlQuery += " FROM Source GROUP BY "+ordAttrList+ " , "+AttrList.at(2);
    sqlQuery += " HAVING COUNT( DISTINCT "+groupAttrList+")>="+Converter(pm.sup*seqid_count).toString();
    execQr(sqlQuery);

    return emptyTable("Seq1");
}

void STSMinerWithBitVector::createBitVectors(){

    std::string q;
    std::vector<BitString*> new_bv;//vettore di bitvector per seq1
    //per ogni singoletto creiamo un vettore di bv_length bit in cui ogni entry rappresenta un trackid
    for(int i=0; i<bvptr_length; ++i){
        BitString* tmp= new BitString(bv_length);
        new_bv.push_back(tmp);
    }
    bvptr.push_back(new_bv);

    std::vector<BitString*> new_vect;//vettore di bistring per seq1
    for(int i=0; i<bvptr_length; ++i){//tanti quante le tuple in seq1
        BitString* tmp= new BitString(2);
        new_vect.push_back(tmp);
    }
    begin_end_vect.push_back(new_vect);

    q="SELECT * FROM Seq1";
    odbc::ResultSet* rs= execQr(q);

    while(rs->next()){
        //leggiamo riga per riga i valori dei singoletti e ne cerchiamo il count
        int d_time= rs->getInt(1);
        std::string subarea_id=rs->getString(2);
        int id=rs->getInt(3);
        q = "SELECT "+groupAttrList+" FROM Source WHERE "+ordAttrList+"="+Converter(d_time).toString()+" AND "+AttrList.at(2)+"='"+Converter(subarea_id).toString()+"';";
        odbc::ResultSet* ris = options.getODBCConnection()->prepareStatement(ODBCXX_STRING_CONST(q.c_str()))->executeQuery();
        while(ris->next()){
            int track_id= ris->getInt(1);
            BitString* v=bvptr.at(0).at(id-1);
            *v= v->set(track_id-1,true);
        }

        if(begin_e=="" && begin_t==""){
        //se non è specificato un vincolo di begin ci vanno bene tutte le seq
            BitString* v=begin_end_vect.at(0).at(id-1);
            *v= v->set(0,true);
        }
        else {
            if(begin_e!="" && checkConstraint(subarea_id,begin_e,begin_e_operator)){
                BitString* v=begin_end_vect.at(0).at(id-1);
                *v= v->set(0,true);
            }
            if(begin_t!="" && checkConstraint(Converter(d_time).toString(),begin_t,begin_t_operator)){
                BitString* v=begin_end_vect.at(0).at(id-1);
                *v= v->set(0,true);
            }
        }
        if(end_e=="" && end_t==""){
        //se non è specificato un vincolo di end ci vanno bene tutte le seq
            BitString* v=begin_end_vect.at(0).at(id-1);
            *v= v->set(1,true);
        }
        else {
            if(end_e!="" && checkConstraint(subarea_id,end_e,end_e_operator)){
                BitString* v=begin_end_vect.at(0).at(id-1);
                *v= v->set(1,true);
            }
            if(end_t!="" && checkConstraint(Converter(d_time).toString(),end_t,end_t_operator)){
                BitString* v=begin_end_vect.at(0).at(id-1);
                *v= v->set(1,true);
            }
        }
    }
}

/*Questo metodo estrae le sequenze di lunghezza 2*/
bool STSMinerWithBitVector::extract2Sequences() {

    std::string createTableQuery="CREATE TABLE Seq2 (ID_1 integer,ID_2 integer)";
    execQr(createTableQuery);

    std::string strmin_g = Converter(min_g).toString();
    std::string strmax_g = Converter(max_g).toString();

    std::string sqlQuery = "INSERT INTO Seq2 SELECT s_1.ID AS ID_1, s_2.ID AS ID_2 ";
                sqlQuery += "FROM Seq1 s_1, Seq1 s_2, Source s1, Source s2 ";
                sqlQuery += "WHERE s_1."+ ordAttrList +"=s1."+ ordAttrList +" AND s_2."+ ordAttrList +"=s2."+ ordAttrList +" ";
                sqlQuery += "AND s_1."+ AttrList.at(2) + "=s1."+ AttrList.at(2) + " AND s_2."+ AttrList.at(2) + "=s2."+ AttrList.at(2) + " ";
                sqlQuery += "AND s1."+groupAttrList+"=s2."+groupAttrList+" ";
                sqlQuery += "AND s1."+ ordAttrList +"<=s2."+ ordAttrList +"-"+strmin_g+" ";
                sqlQuery += "AND s1."+ ordAttrList +">=s2."+ ordAttrList +"-"+strmax_g+" ";

                /*********************/
                //modifica 15_07
                sqlQuery += "AND (";
                sqlQuery += "SELECT COUNT(*) ";
                sqlQuery += "FROM Source src3 ";
                sqlQuery += "WHERE src3."+groupAttrList+"=s1."+groupAttrList+" ";
                sqlQuery += "AND src3."+ ordAttrList + ">s_1."+ ordAttrList + " ";
                sqlQuery += "AND src3."+ ordAttrList + "<s_2."+ ordAttrList + " ) BETWEEN ";
                sqlQuery += Converter(max_space_gap).toString()+" AND "+Converter(min_space_gap).toString();

                /********************/
                //modifica 16_07
               // sqlQuery += "AND ("+funzione di adiacenza tra celle+") BETWEEN ";
               // sqlQuery += Converter(max_space_gap).toString()+" AND "+Converter(min_space_gap).toString();

                /********************/
                //versione originale
               /*sqlQuery += "AND NOT EXISTS(SELECT ";
                sqlQuery += "s_3.ID FROM Seq1 s_3,Source s3 ";
                sqlQuery += "WHERE s_3.d_time=s3.d_time AND s_3.subarea_id=s3.subarea_id AND s3.trackid=s1.trackID ";
                sqlQuery += "AND s3.d_time>s1.d_time ";
                sqlQuery += "AND s3.d_time<s2.d_time) ";*/
                sqlQuery += "GROUP BY ID_1, ID_2";//<---per eliminare i duplicati

    execQr(sqlQuery);
    return emptyTable("Seq2");
}

/*Questo metodo estrae le sequenze di lunghezza k  */
bool STSMinerWithBitVector::extractKSequences(int k) {

        std::string strK = Converter(k).toString();
        std::string strk_1 = Converter(k-1).toString();

        std::string dropQuery = " DROP TABLE IF EXISTS Seq"+strK+"; DROP SEQUENCE IF EXISTS id"+strK+"_sq;";
        execQr(dropQuery);

        std::string createTableQuery = "DROP TABLE IF EXISTS Seq"+strK+"; CREATE TABLE Seq"+strK+" (ID_1 integer,ID_2 integer)";
        execQr(createTableQuery);

        std::string sqlQuery = "INSERT INTO Seq"+strK+" SELECT s1.ID AS ID_1, s2.ID AS ID_2 ";
                    sqlQuery += "FROM Seq"+strk_1+" s1, Seq"+strk_1+" s2 ";
                    sqlQuery += "WHERE s1.ID_2=s2.ID_1 ";
                    sqlQuery += "GROUP BY s1.ID, s2.ID";
        execQr(sqlQuery);
        return emptyTable("Seq"+strK);
}

bool STSMinerWithBitVector::pruningSeqK(int k){
    std::string strK = Converter(k).toString();
    std::vector<BitString*> new_bvptr;
    std::vector<BitString*> new_begin_end_vect;
    std::string query= "SELECT * FROM Seq"+strK;

    odbc::ResultSet* rs= execQr(query);
    while(rs->next()){
        int id1=rs->getInt(1);
        int id2=rs->getInt(2);

        BitString* tmp= new BitString(*bvptr.at(k-2).at(id1-1));
        BitString* tmp2= new BitString(*bvptr.at(k-2).at(id2-1));

        //i nuovi valori di begin/end
        BitString* t = new BitString(*(begin_end_vect.at(k-2).at(id1-1)));
        BitString* t2 = new BitString(*(begin_end_vect.at(k-2).at(id2-1)));
        BitString* ris= new BitString(2);
        ris->set(0,t->test(0));
        ris->set(1,t2->test(1));

        //bitvector temporaneo che contiene l'and dei due bv relativi
        //agli elementi che si uniscono per formare la nuova seq
        BitString* tmp0= &(tmp->operator&=(*tmp2));

        if(tmp0->count(true)>=(pm.sup*seqid_count))
           { new_bvptr.push_back(tmp0);new_begin_end_vect.push_back(ris);}
        else execQr("DELETE FROM Seq"+strK+" WHERE ID_1="+Converter(id1).toString()+" AND ID_2="+Converter(id2).toString());
    }

    bvptr.push_back(new_bvptr);
    begin_end_vect.push_back(new_begin_end_vect);

    if(!emptyTable("Seq"+strK)){
        createSeqQ(k);
        query = "ALTER TABLE Seq"+strK+" ADD ID integer NOT NULL DEFAULT nextval('id"+strK+"_sq'::regclass) PRIMARY KEY ";
        execQr(query);//L'AGGIUNTA DELL'ID SAREBBE MEGLIO FARLA DOPO IL PRUNING COSÌ HA VALORI CONTINUI
        return false;
    }
    else return true;

}

void STSMinerWithBitVector::sourceFilter(){
    //se sono state specificate clausole begin e/o end filtriamo source eliminando tutte le track
    //che non contengono almeno un'occorrenza di questi elementi
    if(begin_e!= "" || begin_t!=""|| end_t!=""|| end_e!=""){
        std::string sq="DELETE FROM Source WHERE trackid NOT IN( SELECT s1.trackid FROM Source s1, Source s2 ";
        sq+="WHERE s1."+ groupAttrList +"=s2."+ groupAttrList +" AND ";
        bool addAND=false;//se viene impostato a true quando aggiungiamo il begin_e dobbiamo precederlo con un AND
        if(begin_t!= ""){
            sq+=" s1."+ordAttrList +begin_t_operator+begin_t;
            addAND=true;
        }
        if(begin_e!= ""){
            if(addAND)sq+=" AND ";
            sq+=" s1."+ AttrList.at(2) +begin_e_operator+"'"+begin_e+"'";
            addAND=true;
        }
        if(end_t!= ""){
            if(addAND)sq+=" AND ";
            sq+=" s2."+ordAttrList +end_t_operator+end_t;
            addAND=true;
        }
        if(end_e!= ""){
            if(addAND)sq+=" AND ";
            sq+=" s2."+AttrList.at(2)+end_t_operator+"'"+end_e+"'";
            addAND=true;
        }
        sq+=");";
        execQr(sq);
    }
}


/*Invoca l'estrattore di sequenze */
void STSMinerWithBitVector::mineRules (){
    dropAllTables();
    int k=0;
    std::string strK;
    bool EmptyResult = false;
    do{
        ++k;
        if(k==1){
            MRLog() << "Starting 1-Sequences extraction..."<<std::endl;
            EmptyResult =extract1Sequences();
            MRLog() << "Extraction done" << std::endl;
            //il numero di distinti trackid usati in source ci serve
            //come divisore per il calcolo del supporto
            seqid_count=getTrackidCount();
            sourceFilter();
            MRLog() << "Deleting 1-sequences with low support..."<<std::endl;
            EmptyResult =pruning1Seq();
            MRLog() << "Pruning completed"<<std::endl;
            //creazione bitvector per ogni entry della source table
            bvptr_length = readSeqVal(1);//ci serve un bitvector per ogni elemento delle sequenze
            min_trackid = getTrackidMinValue();
            bv_length =  getTrackidMaxValue()-min_trackid+1;//il range di valori dei trackid è la lunghezza dei bitvector
            MRDebug()<< "Creating bitvectors..."<<std::endl;
            createBitVectors();
            MRDebug()<< "Done."<<std::endl;
        }
        else{
            strK=Converter(k).toString();
            MRLog() << "Starting "+strK+"-Sequences extraction..."<<std::endl;
            if(k==2)EmptyResult = extract2Sequences();
            else EmptyResult =extractKSequences(k);
            if(EmptyResult)MRLog()<< "Cannot generate sequences of length "+strK<< std::endl;
            else MRLog() << "Extraction done" << std::endl;
            if(!EmptyResult){
                MRLog() << "Starting "+strK+"-Sequences pruning..."<<std::endl;
                EmptyResult =pruningSeqK(k);
                MRLog() << "Pruning done" << std::endl;
                if(EmptyResult)MRLog()<< "Table Seq"+strK+" now is empty..."<< std::endl;
            }
        }
    }while(!EmptyResult && k<600);
    //cancelliamo la tabella vuota
    execQr("DROP TABLE IF EXISTS Seq"+Converter(k).toString());
    showResults(k);
    /*********************************/
    //SALVATAGGIO SEQUENZE & BITVECTORS IN DEST_TABLE
    //vorrei aggiungere i bitvectors come un campo stringa in ogni tabella
    //in lettura bisognerà leggere la stringa char by char e ricreare i bv
    /*********************************/
}

}//end namespace


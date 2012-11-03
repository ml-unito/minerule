// care.cpp

#include "ConstrItemSetsExtraction.h"

#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include "Algorithms/PrepareDataUtils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MROPTIONFILE "mroptions"

namespace minerule {

  // Preprocessing functions

  size_t ConstrItemSetsExtraction::buildAttrStr(const ParsedMinerule::ListType& attr,
                                  size_t startIndex,
                                  std::string& attrStr,
                                  std::vector<int>& des) const {
    ParsedMinerule::ListType::const_iterator it = attr.begin();
    for( ; it!=attr.end(); it++ ) {
      if(it!=attr.begin()) {
        attrStr+=",";
      }

      attrStr+=*it;
      des.push_back(++startIndex);
    }

    return startIndex;
  }

  string ConstrItemSetsExtraction::buildQry( const std::string& groupAttrStr,
                         const std::string& attrStr,
                         const std::string& constraints) const {

    return string("SELECT "+groupAttrStr+","+attrStr+" "
                  "FROM "+minerule.getParsedMinerule().tab_source+" "+
                  (constraints.size()>0 ?
                   "WHERE "+constraints+" " :
                   "")
                  +"ORDER BY "+groupAttrStr+","+attrStr);
  }

  void ConstrItemSetsExtraction::prepareData() {
    MineruleOptions& mrOptions = MineruleOptions::getSharedOptions();

    options.setSupport( minerule.getParsedMinerule().sup );
    options.setConfidence( minerule.getParsedMinerule().conf );
    options.setBodyCardinalities( minerule.getParsedMinerule().bodyCardinalities);
    options.getBodyCardinalities().applyConstraints(mrOptions.getParsers().getBodyCardinalities());

    PrepareDataUtils pdu(minerule, *this);
    options.setTotGroups(pdu.evaluateTotGroups());

    MRLog() << "Building db queries" << endl;
    string bodyConstraints;
    string headConstraints;
    HeadBodyPredicatesSeparator::separate((minerule.getParsedMinerule().mc!=NULL?
                                           minerule.getParsedMinerule().mc->l_and:
                                           NULL),
                                          bodyConstraints,
                                          headConstraints);
    size_t index;
    string groupAttr;
    string bodyAttr;
    string headAttr;
    index=buildAttrStr(minerule.getParsedMinerule().ga,
                       0,
                       groupAttr,
                       rowDes.groupBodyElems );

    buildAttrStr(minerule.getParsedMinerule().ba,
                 index,
                 bodyAttr,
                 rowDes.bodyElems);

    string bodyQry =
      buildQry( groupAttr,
                bodyAttr,
                bodyConstraints);

    MRLog() << "Executing queries" << endl;

    coreConn.useConnection(MineruleOptions::getSharedOptions().getOdbc_db().getConnection());
    coreConn.setOutTableName(minerule.getParsedMinerule().tab_result);
    coreConn.setBodyCardinalities(minerule.getParsedMinerule().bodyCardinalities);
    coreConn.create_db_rule(0);
    coreConn.init();

    MRDebug() << "CARE: body queries:" << bodyQry.c_str() << endl;
    statement = coreConn.getConnection()->prepareStatement(bodyQry.c_str());
  }

  void ConstrItemSetsExtraction::execute() {
    MRLogPush("Starting ConstrItemSetsExtraction mining algorithm...");

    MRLog() << "Preparing data sources..." << endl;
    prepareData();

    odbc::ResultSet* result;

    float support = options.getSupport();
    int maxBody = options.getBodyCardinalities().getMax();

    result = statement->executeQuery();

    ItemType gid1;
    bool found = Transaction::findGid(gid1,result,rowDes,true);
//    string resultfile = tmpnam(NULL);
//    BodyMap bodyMap(resultfile.c_str(),1);
    BodyMap bodyMap(coreConn,1);

    int howManyGroups = 0;
    int totalGroups = options.getTotGroups();
    int howManyRows = 0;
    //int howManyPart = 1;
  //  rsmd = result->getMetaData();
  //  int nFields = rsmd->getColumnCount();
    while (!result->isAfterLast()) {
	HeadBodySourceRow hbsr(result, rowDes);
	ItemType gid = hbsr.getGroupBody();
	Transaction t1(rowDes), t2(rowDes);
	t1.loadBody(gid,result,0);
	howManyRows += bodyMap.add(t1,howManyGroups);
	howManyGroups++;
    }
    MRLog() << "Total rows: " << howManyRows << endl;
    MRLog() << "Total groups: " << totalGroups << endl;
    MRLogPop();

    MRLogPush("Starting itemset extraction...");

    bodyMap.updateCount();
    MRLog() << "Total bodies before pruning: " << bodyMap.size() << endl;
  //  MRLogPop();
//    MRLogPush("Starting pruning ...");
    bodyMap.pruneMap(support*totalGroups,true);
//    MRLogPop();
    bodyMap.setTotalGroups(totalGroups);
//  MRLogPush("Starting rule extraction ...");
    vector<BodyMap::iterator> v;
    multimap<int, BodyMap::iterator> temp;
    for (BodyMap::iterator i = bodyMap.begin(); i!=bodyMap.end(); i++)
       	if (!i->second.done) temp.insert(pair<int, BodyMap::iterator>(i->second.count(),i));
    for (multimap<int, BodyMap::iterator>::iterator i = temp.begin(); i!=temp.end(); i++)
        v.insert(v.end(),i->second);
    MRLog() << "Total bodies after pruning: " << v.size() << endl;

    NewRule r;
    NewRuleSet rs;
//    bodyMap.openOutputFiles();
    int nrules = bodyMap.generateStartItemSets(rs,r,v,maxBody,support*totalGroups,-1);
//    bodyMap.closeOutputFiles();
    MRLog() << "After extracting itemsets: " << nrules << endl;
    MRLogPop();

    coreConn.finalize();
/*
    string loadstr1 = "LOAD DATA INFILE '";
    loadstr1 += resultfile;
    loadstr1 += ".r' INTO TABLE ";
    loadstr1 += minerule.getParsedMinerule().tab_result;
    string loadstr2 = "LOAD DATA INFILE '";
    loadstr2 += resultfile;
    loadstr2 += ".hb' INTO TABLE ";
    loadstr2 += minerule.getParsedMinerule().tab_result;
    loadstr2 += "_elems";
*/
    delete statement;
/*
    statement = coreConn.getConnection()->prepareStatement(loadstr1);
    stmt1 = coreConn.getConnection()->prepareStatement(loadstr2);

    MRLogPush("Loading result into Database...");
    loadstr1 = resultfile + ".r";
    chmod(loadstr1.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
    statement->executeUpdate();
    //unlink(loadstr1.c_str());
    loadstr1 = resultfile + ".hb";
    chmod(loadstr1.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
    stmt1->executeUpdate();
    //unlink(loadstr1.c_str());
    MRLogPop();

    delete statement;
    delete stmt1;
*/
  }

} //end namespace
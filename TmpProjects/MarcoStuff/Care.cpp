// care.cpp

#include "Care.h"

#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include "Algorithms/PrepareDataUtils.h"

#define MROPTIONFILE "mroptions"

namespace minerule {

  // Preprocessing functions

  size_t CARE::buildAttrStr(const ParsedMinerule::AttrVector& attr,
                                  size_t startIndex,
                                  std::string& attrStr,
                                  std::vector<int>& des) const {
    ParsedMinerule::AttrVector::const_iterator it = attr.begin();
    for( ; it!=attr.end(); it++ ) {
      if(it!=attr.begin()) {
        attrStr+=",";
      }

      attrStr+=*it;
      des.push_back(++startIndex);
    }

    return startIndex;
  }

  string CARE::buildQry( const std::string& groupAttrStr,
                         const std::string& attrStr,
                         const std::string& constraints) const {

    return string("SELECT "+groupAttrStr+","+attrStr+" "
                  "FROM "+minerule.getParsedMinerule().tab_source+" "+
                  (constraints.size()>0 ?
                   "WHERE "+constraints+" " :
                   "")
                  +"ORDER BY "+groupAttrStr+","+attrStr);
  }

  void CARE::prepareData() {
    MineruleOptions& mrOptions = MineruleOptions::getSharedOptions();

    options.setSupport( minerule.getParsedMinerule().sup );
    options.setConfidence( minerule.getParsedMinerule().conf );
    options.setBodyCardinalities( minerule.getParsedMinerule().bodyCardinalities);
    options.setHeadCardinalities( minerule.getParsedMinerule().headCardinalities);
    options.getBodyCardinalities().applyConstraints(mrOptions.getParsers().getBodyCardinalities());
    options.getHeadCardinalities().applyConstraints(mrOptions.getParsers().getHeadCardinalities());

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
    buildAttrStr(minerule.getParsedMinerule().ha,
                 index,
                 headAttr,
                 rowDes.headElems);

    string bodyQry =
      buildQry( groupAttr,
                bodyAttr,
                bodyConstraints);

    string headQry =
      buildQry( groupAttr,
                headAttr,
                headConstraints);

    MRLog() << "Executing queries" << endl;

    connection.useODBCConnection(MineruleOptions::getSharedOptions().getOdbc_db().getODBCConnection());
    connection.setOutTableName(minerule.getParsedMinerule().tab_result);
    connection.setBodyCardinalities(minerule.getParsedMinerule().bodyCardinalities);
    connection.setHeadCardinalities(minerule.getParsedMinerule().headCardinalities);
    connection.createResultTables();

    MRDebug() << "CARE: body queries:" << bodyQry.c_str() << endl;
    MRDebug() << "CARE: head queries:" << headQry.c_str() << endl;
    statement = connection.getODBCConnection()->prepareStatement(bodyQry.c_str());
    stmt1 = connection.getODBCConnection()->prepareStatement(headQry.c_str());
  }

  void CARE::execute() {
    MRLogPush("Starting CARE mining algorithm...");

    MRLog() << "Preparing data sources..." << endl;
    prepareData();

    odbc::ResultSet* result, *result1;

    float support = options.getSupport();
    int maxBody = options.getBodyCardinalities().getMax();
    int maxHead = options.getHeadCardinalities().getMax();

    result = statement->executeQuery();
    result1 = stmt1->executeQuery();

    ItemType gid1;
    bool found = Transaction::findGid(gid1,result1,rowDes,true);
    found = found && Transaction::findGid(gid1,result,rowDes,true);
    char * resultfile = tmpnam(NULL);
    BodyMap bodyMap(resultfile,1);
    BodyMap headMap(resultfile,1);

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
	bool found2 = t2.findGid(gid,result1,rowDes);
	if (found2) {
		t2.loadHead(gid,result1,0);
	}
		howManyRows += bodyMap.add(t1,totalGroups);
		howManyRows += headMap.add(t2,totalGroups);
		howManyGroups++;
    }
    MRLog() << "Total rows: " << howManyRows << endl;
    MRLog() << "Total groups: " << totalGroups << endl;
    MRLogPop();

    MRLogPush("Starting rule extraction...");

    bodyMap.updateCount();
    headMap.updateCount();
    MRLog() << "Total bodies before pruning: " << bodyMap.size() << endl;
  //  MRLogPop();
//    MRLogPush("Starting pruning ...");
    bodyMap.pruneMap(support*totalGroups,true);
    headMap.pruneMap(support*totalGroups,true);
//    MRLogPop();
    bodyMap.setTotalGroups(totalGroups);
//  MRLogPush("Starting rule extraction ...");
    vector<BodyMap::iterator> v;
    vector<BodyMap::iterator> v1;
    multimap<int, BodyMap::iterator> temp;
    for (BodyMap::iterator i = bodyMap.begin(); i!=bodyMap.end(); i++)
       	if (!i->second.done) temp.insert(pair<int, BodyMap::iterator>(i->second.count(),i));
    for (multimap<int, BodyMap::iterator>::iterator i = temp.begin(); i!=temp.end(); i++)
        v.insert(v.end(),i->second);
    for (BodyMap::iterator i = headMap.begin(); i!=headMap.end(); i++)
        v1.insert(v1.end(),i);
    MRLog() << "Total bodies after pruning: " << v.size() << endl;

    NewRule r;
    NewRuleSet rs1;
    bodyMap.openOutputFiles();
    int nrules = bodyMap.buildRules(rs1,r,v,v1,maxBody,maxHead,support*totalGroups,-1);
    bodyMap.closeOutputFiles();
    MRLog() << "After extracting rules: " << nrules << endl;
    MRLogPop();

    delete statement;
    delete stmt1;
  }

} //end namespace

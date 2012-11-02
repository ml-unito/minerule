#include "Algorithms/PartitionBase.h"
#include "Algorithms/AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h"
#include "Algorithms/PrepareDataUtils.h"


void partitionWithoutClusters(const minerule::AlgorithmsOptions& algoOptions);

namespace minerule {

  void PartitionBase::prepareData() {
    PrepareDataUtils pdu(minerule, *this);
    MRLogPush("Preparing data sources...");

    odbc::Connection* connection;
    MineruleOptions& mrOptions = 
      MineruleOptions::getSharedOptions();

    options.setMiningAlgorithmsOptions(mrOptions.getMiningAlgorithms());
    
    minerule::HeadBodySourceRowDescription rowDes;
    connection = mrOptions.getOdbc_db().getConnection();
    
    options.setSupport( minerule.getParsedMinerule().sup );
    options.setConfidence( minerule.getParsedMinerule().conf );

    options.setBodyCardinalities( minerule.getParsedMinerule().bodyCardinalities);
    options.setHeadCardinalities( minerule.getParsedMinerule().headCardinalities);

    options.getBodyCardinalities().applyConstraints(mrOptions.getParsers().getBodyCardinalities());
    options.getHeadCardinalities().applyConstraints(mrOptions.getParsers().getHeadCardinalities());

    MRLogPush("Preparing data sources..."); 
    string queryText;
    pdu.buildSourceTableQuery( queryText, rowDes );
    MRDebug() << "PartitionBase: query: " << queryText << endl;

    odbc::PreparedStatement* statement= 
      connection->prepareStatement(queryText);

    options.setConnection(connection);
    options.setStatement(statement);
    options.setSourceRowDescription(rowDes);
    options.setOutTableName(minerule.getParsedMinerule().tab_result);
    options.setTotGroups(pdu.evaluateTotGroups());
  
    MRLogPop();
  }


  void PartitionBase::execute() {
    MRLog() << "This is Partition (without clusters) mining algorithm..." << endl;
    prepareData();

    partitionWithoutClusters(options);
  };

}


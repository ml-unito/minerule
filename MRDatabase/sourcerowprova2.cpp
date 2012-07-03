#include<iostream>
#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>

#include<map>
#include<ext/hash_set>

#include "sourcerow.h"
#include "sourcerowdescriptor.h"


using namespace minerule;
using namespace std;
using namespace __gnu_cxx;


int
main(int argc, char** argv) {
  odbc::Connection* connection;
  try {
    //    if( argc<2 || String(argv[1])==String("mysql") ) {
      cout << "Using MySQL" << endl;
      connection=
	odbc::DriverManager::getConnection("prova", "root", "mysql");
      //    } else {
      //      cout << "Using PostgreSQL" << endl;
      //      connection=
      //	odbc::DriverManager::getConnection("MineRule-psg", "esposito", "");
      //    }

    // First step: read from a result set and insert its values in another table
    // Everything using only the sourcerow primitives.

      odbc::Statement* statement = connection->createStatement();
      odbc::ResultSet* rs = statement->executeQuery("select * from sales_facts");


    // Description of HeadBodySourceRow the following lines means
    // that each element is described by a single attribute and
    // the gbid is given by source column 1
    // the cbid is given by source column 2
    // the bid is given by source column 3
    // the body elem is given by source columns 4 and 5
    HeadBodySourceRowDescription rowDes;
    rowDes.groupBodyElems.push_back(1);
    rowDes.clusterBodyElems.push_back(2);
    /*    rowDes.bodyElems.push_back(3);
    rowDes.bodyElems.push_back(4);
    rowDes.clusterHeadElems.push_back(5);
    rowDes.headElems.push_back(6); */

    map< SourceRowAttributeCollection, int, SourceRowAttributeCollection > 
      mymap;
    
    
    SourceRowAttributeCollection body;

    SourceRowDescriptor srd(rs, rowDes);
    cout << "---- Descriptions ----" << endl;
    cout << "group body descr:" << srd.getGroupBody().getSQLDataDefinition() << endl;
    cout << "cluster body descr:" << srd.getClusterBody().getSQLDataDefinition() << endl;
    cout << "body descr:" << srd.getBody().getSQLDataDefinition() << endl;
    cout << "cluster head descr:" << srd.getClusterHead().getSQLDataDefinition() << endl;
    cout << "head descr:" << srd.getHead().getSQLDataDefinition() << endl;
    cout << "==== Column Names ====" << endl;
    cout << "group body names:" << srd.getGroupBody().getSQLColumnNames() << endl;
    cout << "cluster body names:" << srd.getClusterBody().getSQLColumnNames() << endl;
    cout << "body names:" << srd.getBody().getSQLColumnNames() << endl;
    cout << "cluster head names:" << srd.getClusterHead().getSQLColumnNames() << endl;
    cout << "head names:" << srd.getHead().getSQLColumnNames() << endl;
    cout << "======================" << endl;

    odbc::Statement* outTableStatement = connection->createStatement();
    
    outTableStatement->execute(
			       "create table outprova (" +
			       srd.getGroupBody().getSQLDataDefinition() +","+
			       srd.getClusterBody().getSQLDataDefinition()+//","+
			       //   srd.getBody().getSQLDataDefinition()+","+
			       //srd.getClusterHead().getSQLDataDefinition()+","+
			       //srd.getHead().getSQLDataDefinition() +
			       ")");

    int i=0;
    while( rs->next() ) {
      if(argc>1) {
	//cout << mymap.size() << " " << i << "\r";
	//HeadBodySourceRow* item= new HeadBodySourceRow(rs, rowDes);	
	HeadBodySourceRow item(rs, rowDes);

	body = item.getBody();

	//cout << *item << endl;
	cout << "!!" << body << endl;

       	mymap[body]=i;
	outTableStatement->execute(
				   "insert into outprova values (" +
				   item.getGroupBody().getSQLData() +","+
				   item.getClusterBody().getSQLData()+","+
				   //	   item.getBody().getSQLData()+","+
				   //item.getClusterHead().getSQLData()+","+
				   //item.getHead().getSQLData() +
				   ")"); 
      }
      
      i++;
      //      cout << item << endl;
    }

    delete rs;
    delete statement;

    // First step ended 

    // Second Step, we read the data back the data just inserted using a new soucerow
    // object

    HeadBodySourceRowDescription outDes;
    
    unsigned int lastOccupied; 

    lastOccupied=
      outDes.setGroupBodyElems(1,srd.getGroupBody().getColumnsCount());
    lastOccupied=
      outDes.setClusterBodyElems(lastOccupied+1, srd.getClusterBody().getColumnsCount());
    lastOccupied=
      outDes.setBodyElems(lastOccupied+1, srd.getBody().getColumnsCount());
    lastOccupied=
      outDes.setClusterHeadElems(lastOccupied+1, srd.getClusterHead().getColumnsCount());
    lastOccupied=
      outDes.setHeadElems(lastOccupied+1, srd.getHead().getColumnsCount());

   
    odbc::ResultSet* outrs = 
      outTableStatement->executeQuery(
				     "select "+
				     srd.getGroupBody().getSQLColumnNames() +","+
				     srd.getClusterBody().getSQLColumnNames()+ " "+/*","+
				     srd.getBody().getSQLColumnNames()+","+
				     srd.getClusterHead().getSQLColumnNames()+","+
				     srd.getHead().getSQLColumnNames() + " " +*/
				     "from outprova"
				     );

    while(outrs->next()) {
      HeadBodySourceRow row(outrs,outDes);

      cout << row << endl;
    }
    
    delete outTableStatement; 
    delete connection;

  } catch (odbc::SQLException e){
    cerr << e.getMessage() << endl;
    throw;
  } catch (std::exception e) {
    cerr << e.what() << endl;
    throw;
  }
  return 0;
}

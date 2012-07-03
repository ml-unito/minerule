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
    rowDes.bodyElems.push_back(3);
    rowDes.bodyElems.push_back(4);
    rowDes.clusterHeadElems.push_back(5);
    rowDes.headElems.push_back(6);

    vector<SourceRowAttributeCollection> srvec;
    vector<SourceRowAttributeCollection> srvec2;


    while( rs->next() ) {
      if(argc>1) {
	//cout << mymap.size() << " " << i << "\r";
	//HeadBodySourceRow* item= new HeadBodySourceRow(rs, rowDes);	
	HeadBodySourceRow item(rs, rowDes);
	
	//	srvec.push_back(item.getBody());
	//cout << *item << endl;
      }
    }

    exit(1);

    srvec2 = srvec;

    srvec.clear();
    cout << "=======SRVEC==(should be empty)=====" << endl;
    vector<SourceRowAttributeCollection>::const_iterator it = srvec.begin();
    for(; it!=srvec.end(); it++ ) {
      cout << *it << endl;
    }

    cout << "=======SRVEC2=======" << endl;
    for(it=srvec2.begin(); it!=srvec2.end(); it++ ) {
      cout << *it << endl;
    }

    delete rs;
    delete statement;
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

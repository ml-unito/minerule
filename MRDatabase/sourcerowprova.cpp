#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>

#include "sourcerow.h"

// libcwd includes
using namespace minerule;


int
main(int argc, char** argv) {
  //Debug( dc::malloc.on() );
  //  cout << "Memory allocated:" << libcw::debug::mem_blocks() << endl;

  odbc::Connection* connection;
  try {
    //    if( argc<2 || String(argv[1])==String("mysql") ) {
      cout << "Using MySQL" << endl;
      connection=
	odbc::DriverManager::getConnection("test", "root", "mysql");
      //    } else {
      //      cout << "Using PostgreSQL" << endl;
      //      connection=
      //	odbc::DriverManager::getConnection("MineRule-psg", "esposito", "");
      //    }

    odbc::Statement* statement = connection->createStatement();
    odbc::ResultSet* rs = statement->executeQuery("select * from InputTableHan");

    assert(rs!=NULL);

    // Description of HeadBodySourceRow the following lines means
    // that each element is described by a single attribute and
    // the gbid is given by source column 1
    // the cbid is given by source column 2
    // the bid is given by source column 3
    // etc. etc.
        // Allocated memory...
    //    cout << "Memory allocated(srd-1):" << libcw::debug::mem_blocks() << endl;
    HeadBodySourceRowDescription srd;
    /* srd.groupBodyElems.push_back(1);
       srd.clusterBodyElems.push_back(2);*/
    srd.bodyElems.push_back(1);
    srd.bodyElems.push_back(2);
    /*    srd.bodyElems.push_back(3);
    srd.bodyElems.push_back(4);
    srd.clusterHeadElems.push_back(5);
    srd.headElems.push_back(6);*/

    HeadBodySourceRowDescription srd2;
    srd2.groupBodyElems.push_back(1);
    srd2.clusterBodyElems.push_back(2);
    /*    srd2.bodyElems.push_back(3);
    srd2.bodyElems.push_back(4);
    srd2.clusterHeadElems.push_back(5);
    srd2.headElems.push_back(6);*/
    // Allocated memory...
    //    cout << "Memory allocated(srd1):" << libcw::debug::mem_blocks() << endl;

    cout << "prima del ciclo";

    while( rs->next() ) {
      HeadBodySourceRow item(rs, srd);
      const SourceRowElement& sre = item.getBody();
      string s;
      SourceRowElement::serializeElementToString(sre, s);
      cout << "Deserializing:" << s << endl;
      SourceRowElement* elem = 
	SourceRowElement::deserializeElementFromString(s);

      cout << *elem << endl;

      /*      if(argc>1) {
	cout << endl;
	HeadBodySourceRow item(rs,srd);	
	cout << item;

	HeadBodySourceRow item2(item);
	cout << item2;
	}*/
    }

    cout << "dopo il ciclo";

    //    cin >> n;
    delete rs;
    delete statement;
    delete connection;
  } catch (MineruleException& e) {
    cerr << e.what() << endl;
  } catch (std::exception& e){
    cerr << e.what() << endl;
  }

  // Allocated memory...
  //  cout << "Memory allocated:" << libcw::debug::mem_blocks() << endl;
  return 0;
}

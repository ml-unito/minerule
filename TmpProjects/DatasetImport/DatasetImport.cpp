#include <iostream>
#include <string>
#include <sstream>

#include "minerule/Utils/MineruleOptions.hpp"
#define MROPTIONFILE "mroptions.txt"
#define BUFSIZE 1<<18


using namespace std;
using namespace minerule;

class DBInserter {
  odbc::PreparedStatement* state;
  odbc::Connection* conn;
public:
  DBInserter( odbc::Connection* c, string tbname ) throw(MineruleException, odbc::SQLException) :
    conn(c)  {
    createTable( tbname );
    state = conn->prepareStatement("INSERT INTO "+tbname+" VALUES (?,?)");
  }

  ~DBInserter() {
    delete state;
  }

  void insertItem( size_t ngroup, size_t item ) throw(MineruleException, odbc::SQLException) {
    state->setInt(1, ngroup);
    state->setInt(2, item );

    state->execute();
  }


  void createTable( const string& tbname ) throw(MineruleException, odbc::SQLException) {
    odbc::Statement* state = conn->createStatement();
    state->execute("CREATE TABLE " +tbname+" (gid INT(10), item INT(10), INDEX (gid,item))" );

    delete state;
  }
};



int
main(int argc, char** argv) {
  if(!(argc>2)) { 
    cout << "Usage: " << argv[0] << " <inputfilename> <tabname>" << endl;
    exit(1);
  }

  try {

    MineruleOptions& mr = MineruleOptions::getSharedOptions();
    mr.readFromFile(MROPTIONFILE);
  
    ifstream infile( argv[1] );
    assert( infile );
    size_t ngroup=0;

    DBInserter inserter( mr.getODBC().getODBCConnection(), argv[2] );

    while( infile ) {
      ngroup++;
      char buf[BUFSIZE];
      infile.getline( buf, BUFSIZE );
      if(!infile)
	continue;
      // mettere controllo su failbit
    
      stringstream streambuf( buf );
      while( streambuf ) {
	size_t item;
	streambuf >> item;
	if(!streambuf)
	  continue;

	inserter.insertItem( ngroup, item );
      }

      if(ngroup%1000==0) {
	std::cout << "count:" << ngroup << "\r";
	std::cout.flush();
      }
      
    }

    cout << endl;
    cout << "Done!" << endl;
  } catch( odbc::SQLException& e ) {
    cout << e.what() << endl;
  } catch( std::exception& e ) {
    cout << e.what() << endl;
  }
}

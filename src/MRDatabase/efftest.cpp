#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>

#include "MRDatabase/sourcerow.h"
#include "Utils/MineruleOptions.h"
#include "MRDatabase/itemtype.h"

using namespace minerule;

class mrec_base {
public:
  virtual void set(int ,int ) =0;
};

class mrec : public mrec_base {
protected:
  int n1;
  int n2;

public:
  virtual void set(int , int );
};

void mrec::set(int m1,int m2) {
  n1=m1; 
  n2=m2;
}



void readdb(odbc::Statement* state) {
  odbc::ResultSet* rs = state->executeQuery("SELECT * FROM expT25_I10_D10_N1");

#ifdef BOBO
  HeadBodySourceRowDescription srd;
  srd.groupBodyElems.push_back(1);
  srd.bodyElems.push_back(2);
#endif

  //  map<SourceRowAttributeCollection,SourceRowAttributeCollection> tmp;
  while( rs->next() ) {
#ifdef BOBO
     HeadBodySourceRow sr(rs,srd);
     ItemType it = sr.getBody();
     
     //     tmp[sr.getBody()]=sr.getBody();
     //     tmp[sr.getHead()]=sr.getBody(); 
#else
     mrec_base* m = new  mrec();
     m->set(rs->getLong(1),rs->getLong(2));
     delete m;
#endif
   }
   
   delete rs;

   MRLog() << "Before clearing the vector" << endl;
   //   tmp.clear();
   //   tmp.erase(tmp.begin(), tmp.end());

   MRLog() << "Before exiting procedure"<< endl;
}

int
main() {
 odbc::Connection* connection=
   MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

 odbc::Statement* state = 
   connection->createStatement();

 MRLog() << "Starting to read..." << endl;
 readdb(state);
 MRLog() << "Done!" << endl;

 MRLog() << "Starting to read..." << endl;
 readdb(state);
 MRLog() << "Done!" << endl;

 MRLog() << "Starting to read..." << endl;
 readdb(state);
 MRLog() << "Done!" << endl;


}

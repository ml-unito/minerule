#include "mrstatement.h"


int main() {
  odbc::PreparedStatement* state = new MRStatement("prova");
  odbc::ResultSet* query = state->executeQuery();

  while( query->next() ) {
    cout << "qui" << endl;
  }
}

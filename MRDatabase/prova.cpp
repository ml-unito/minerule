#include <iostream>
#include <sstream>

using namespace std;



int
main() {
  string str="prova ciao come stai";
  istringstream s(str);

  while(s) {
    string news;
    s >> news;
    cout << news << endl;
  }
  return 0;
}

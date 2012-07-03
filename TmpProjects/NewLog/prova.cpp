#include <iostream>
#include <sys/time.h>

using namespace std;

int main() {
  struct timeval tv_start;
  struct timeval tv;
  
  
  gettimeofday(&tv,NULL);
  cout << double(tv.tv_sec-tv_start.tv_sec) << endl;

}

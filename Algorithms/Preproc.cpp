#include "CCSMSequence.h"
#include <fstream>
#include <string>

using namespace std;


int main(int argc, char** argv){

ifstream in("dino1.txt");
ofstream out(argv[1]);
string row;
CCSMSequence input;
int limit = atoi(argv[3]);

int i=0;
while (i!=limit){
	getline(in,row);
	input.read(row);
	CCSMSequence output(input,0,atoi(argv[2]));
	out<<output.toStdString()<<endl;
	input.svuota();
	i++;
}
in.close();
out.close();
}


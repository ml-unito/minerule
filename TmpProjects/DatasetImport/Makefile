CXX=g++
CFLAGS=-I../.. -L../../lib -Wall
LIBS=-lmr -lodbc++


datasetimport: DatasetImport.o
	$(CXX) $(CFLAGS) -o datasetimport DatasetImport.o $(LIBS)

DatasetImport.o: DatasetImport.cpp
	$(CXX) $(CFLAGS) -c DatasetImport.cpp

clean: 
	rm -f *.o datasetimport

include ../Makefile.vars

LOCALLIB=libalgorithms.a
LIBS=-lodbc++ -L. -lalgorithms -L../lib -lmrdatabase -lutils -loptimizer

OBJECTS=partitionWithClustersLauncher.o sqlCoreConn.o  AlgorithmsOptions.o partitionBaseLauncher.o Algorithms.o PartitionBase.o PartitionWithClusters.o FPGrowth.o fpGrowthLauncher.o fpGrowthOrigLauncher.o IDIncrementalAlgorithm.o ConstrTree.o IncrAlgoClasses.o DestrTree.o IncrementalAlgorithm.o BFSWithGidsNoCross.o PrepareDataUtils.o ResultCombinator.o MiningAlgorithmBase.o FSMiner.o FSTree.o FSTreeSequence.o FSTreeNode.o CCSMSequence.o CCSMiner.o BFSWithGidsAndCross.o  Bodymap.o Care.o ConstrItemSetsExtraction.o


SOURCES=$(OBJECTS:.o=.cpp)
INCLUDES= -I. -I.. -I$(LIBGISTHOME)/include -I$(LIBGAHOME) -DLIBGIST 
MARCOLIBS=-L$(MARCOPATH)/libgist/lib -lbtree -lgist -lrtree -lnptree -lgist

all: $(LOCALLIB) ../lib/$(LOCALLIB)

miner: 
#	g++ $(INCLUDES) -Wall -O0 -o FSMiner FSMiner.cpp  FSTree.cpp  FSTreeSequence.cpp  FSTreeNode.cpp 
	g++ $(INCLUDES) -Wall -O0 -o CCSM CCSMiner.cpp CCSMSequence.cpp

$(LOCALLIB): $(OBJECTS) 
	$(AR) rcs $(LOCALLIB) $(OBJECTS)

.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) core.exe partitionWithoutCluster.exe

../lib/$(LOCALLIB): $(LOCALLIB)
	cp $(LOCALLIB) ../lib


%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
        $(RM) -f $@.$$$$

include $(SOURCES:.cpp=.d)


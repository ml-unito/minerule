MRHOME = /usr/local/home/esposito/minerule/v1.0.7/TmpProjects/MarcoStuffWithMRDatabase_2_with_conf
include ../../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
#LOCALLIB=NULLLIB

#libs needet to build specific targets in this directory
LIBS=-L../../lib -L$(LIBGISTHOME)/lib -lmr -lodbc++  -lbtree -lgist -lrtree -lnptree -lgist
#objects to be included in the library
#OBJECTS=marco_partitionBaseLauncher.o marco_partitionWithClusterLauncher.o 
SOURCES=$(OBJECTS:.o=.cpp)
INCLUDES=-w -I. -I../.. -I../../Algorithms -I../../MRDatabase -I$(LIBGISTHOME)/include -DLIBGIST -g

#additional targets (if any)
OTHERTARGETS=marcocore.exe

all: $(LOCALLIB)  ../lib/$(LOCALLIB) $(OTHERTARGETS) 

# the following line shows how a target in OTHERTARGET should be made
marcocore.exe: $(OBJECTS) $(LOCALLIB) marcocore.o
	 $(CXX) $(CXXOPT) -o marcocore.exe marcocore.o $(OBJECTS) $(LIBS)

extractRules: $(LOCALLIB) extractRules.o
	 $(CXX) $(CXXOPT) -o extractRules.exe extractRules.o $(OBJECTS) $(LIBS)


prova: $(LOCALLIB) prova.o
	 $(CXX) $(CXXOPT) -o prova prova.o $(LIBS)

.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) $(OTHERTARGETS)

# this makes $(LOCALLIB)
$(LOCALLIB): $(OBJECTS) 
	$(AR) rcs $(LOCALLIB) $(OBJECTS)

# this makes ../lib/...
../lib/$(LOCALLIB): $(LOCALLIB)
	cp $(LOCALLIB) ../lib

# automatic building of dependencies
%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
        $(RM) -f $@.$$$$

include $(SOURCES:.cpp=.d)


include ../../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
#LOCALLIB=NULLLIB

#libs needet to build specific targets in this directory
LIBS=-L../../lib -L$(LIBGISTHOME)/lib -lmr -lodbc++  -lbtree -lgist -lrtree -lnptree -lgist
#objects to be included in the library
OBJECTS=bodymap.o bitstring.o constraints.o Care.o ConstrItemSetsExtraction.o
SOURCES=$(OBJECTS:.o=.cpp)
INCLUDES+=-w -I. -I../.. -I../../Algorithms -I../../MRDatabase -I$(LIBGISTHOME)/include -DLIBGIST -g

#additional targets (if any)
OTHERTARGETS=

all: $(LOCALLIB)  ../lib/$(LOCALLIB) $(OTHERTARGETS) 

# the following line shows how a target in OTHERTARGET should be made

extractItemset: $(OBJECTS) $(LOCALLIB) extractItemset.o
	 $(CXX) $(CXXOPT) -o extractItemset.exe extractItemset.o $(OBJECTS) $(LIBS)

extractRules: $(OBJECTS) $(LOCALLIB) extractRules.o
	 $(CXX) $(CXXOPT) -o extractRules.exe extractRules.o $(OBJECTS) $(LIBS)

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


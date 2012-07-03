MRHOME = /home/minerule/sources/marcoV1.0.2
include ../../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
#LOCALLIB=NULLLIB

#libs needet to build specific targets in this directory
LIBS=-L../../lib -L$(LIBGISTHOME)/lib -lmr -lodbc++  -lbtree -lgist -lrtree -lnptree -lgist
#objects to be included in the library
OBJECTS=mkindex.o qryindex.o provaindex.o
SOURCES=$(OBJECTS:.o=.cpp)
INCLUDES=-w -I. -I../.. -I../../Algorithms -I../../MRDatabase -I$(LIBGISTHOME)/include -DLIBGIST

#additional targets (if any)
OTHERTARGETS=mkindex qryindex provaindex

all: $(LOCALLIB)  ../lib/$(LOCALLIB) $(OTHERTARGETS) 

# the following line shows how a target in OTHERTARGET should be made
mkindex: $(OBJECTS) $(LOCALLIB) 
	 $(CXX) $(CXXOPT) -o mkindex  mkindex.o $(LIBS)


qryindex: $(OBJECTS) $(LOCALLIB) 
	 $(CXX) $(CXXOPT) -o qryindex  qryindex.o $(LIBS)

provaindex: $(OBJECTS) $(LOCALLIB)
	$(CXX) $(CXXOPT) -o provaindex provaindex.o $(LIBS)

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


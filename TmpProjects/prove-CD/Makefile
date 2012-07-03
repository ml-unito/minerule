include ../../Makefile.vars
#the following var must be set to the library made in the current
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB)
LOCALLIB=NULLLIB

#libs needed to build specific targets in this directory
LIBS=-lmr -lodbc++ -lbtree -lgist -lrtree -lnptree -lgist
#objects to be included in the library
OBJECTS=costr.o classi.o algo.o
SOURCES=$(OBJECTS:.o=.cpp)
CXXOPT+=-I../../ -L../../lib -L$(LIBGISTHOME)/lib -I$(LIBGISTHOME)/include -DLIBGIST

#additional targets (if any)
OTHERTARGETS=costr

all:  $(OTHERTARGETS)

# the following line shows how a target in OTHERTARGET should be made

costr: $(OBJECTS)
	$(CXX) $(CXXOPT) -o costr $(OBJECTS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) $(OTHERTARGETS) $(OBJECTS)



# automatic building of dependencies
%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
        $(RM) -f $@.$$$$

include $(SOURCES:.cpp=.d)

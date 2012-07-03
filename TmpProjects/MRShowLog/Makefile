include ../../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
LOCALLIB=NULLLIB

#libs needed to build specific targets in this directory
LIBS=-lmr -lodbc++
#objects to be included in the library
OBJECTS=mrshowlog.o
SOURCES=$(OBJECTS:.o=.cpp)
CXXOPT+=-I../../ -L../../lib

#additional targets (if any)
OTHERTARGETS=mrshowlog

all:  $(OTHERTARGETS)

# the following line shows how a target in OTHERTARGET should be made
mrshowlog: $(OBJECTS) 
	$(CXX) $(CXXOPT) -o mrshowlog mrshowlog.o $(LIBS)


.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) $(OTHERTARGETS)


# automatic building of dependencies
%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
        $(RM) -f $@.$$$$

include $(SOURCES:.cpp=.d)
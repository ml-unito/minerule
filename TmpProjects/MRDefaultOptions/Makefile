include ../../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
LOCALLIB=NULLLIB

#libs needet to build specific targets in this directory
LIBS=-lutils -lodbc++
#objects to be included in the library
OBJECTS=MRDefaultOptions.o
SOURCES=$(OBJECTS:.o=.cpp)
CXXOPT+=-I../../ -L../../lib

#additional targets (if any)
OTHERTARGETS=mrdefaults

all: $(OTHERTARGETS) 


mrdefaults: $(OBJECTS) ../../lib/libutils.a
	$(CXX) $(CXXOPT) -o mrdefaults $(OBJECTS) $(LIBS)


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


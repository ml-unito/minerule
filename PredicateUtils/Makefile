include ../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
LOCALLIB=libpredicateutils.a

#libs needet to build specific targets in this directory
LIBS=
#objects to be included in the library
OBJECTS=ExpressionNFCoder.o PredicateUtils.o Predicate.o SimplePredAnalyzer.o HeadBodyPredicatesSeparator.o Interval.o
SOURCES=$(OBJECTS:.o=.cpp)

#additional targets (if any)
OTHERTARGETS=

all: $(LOCALLIB)  ../lib/$(LOCALLIB) $(OTHERTARGETS) 

# the following line shows how a target in OTHERTARGET should be made
#target1: $(OBJECTS) $(LOCALLIB) target1.o
#	$(CXX) $(CXXOPT) -o target1 target1.o $(LIBS)


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


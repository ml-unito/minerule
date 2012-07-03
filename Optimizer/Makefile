include ../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
LOCALLIB=liboptimizer.a

#libs needet to build specific targets in this directory
LIBS=
#objects to be included in the library
OBJECTS=OptimizedMinerule.o OptimizerCatalogue.o QueryNormalizer.o QueryResult.o GAQueryCombinator.o 
SOURCES=$(OBJECTS:.o=.cpp)

#additional targets (if any)
OTHERTARGETS=prova

all: $(LOCALLIB)  ../lib/$(LOCALLIB)

# the following line shows how a target in OTHERTARGET should be made
prova: $(OBJECTS) $(LOCALLIB) prova.o
	$(CXX) $(CXXOPT) -o prova prova.o -L../lib -loptimizer -lparsers -lutils -lodbc++


.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) $(OTHERTARGET)

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


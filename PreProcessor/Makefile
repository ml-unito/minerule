include ../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
LOCALLIB=libpreprocessor.a

#libs needet to build specific targets in this directory
LIBS=-L. -lpreprocessor -lodbc++
#objects to be included in the library
OBJECTS=translatedtablestandardsql.o translationmanager.o translationmanagerstandardsql.o
SOURCES=$(OBJECTS:.o=.cpp)

#additional targets (if any)
OTHERTARGETS=tmanagerprova

all: $(LOCALLIB) ../lib/$(LOCALLIB)  $(OTHERTARGETS)


tmanagerprova: $(OBJECTS) $(LOCALLIB) tmanagerprova.o
	$(CXX) $(CXXOPT) -o tmanagerprova tmanagerprova.o $(LIBS)


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


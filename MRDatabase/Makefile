include ../Makefile.vars
LOCALLIB=libmrdatabase.a

LIBS=-L../lib  -lmrdatabase -lutils -lodbc++ $(DEBUGLIBS)
SOURCES=sourcerow.cpp sourcerowelement.cpp sourcerowattribute.cpp sourcerowattributecollection.cpp sourcerowdescriptor.cpp sourcerowcommon.cpp MIndex.cpp
OBJECTS=$(SOURCES:.cpp=.o)

#default target
all: $(LOCALLIB) ../lib/$(LOCALLIB)


#other targets
$(LOCALLIB): $(OBJECTS)
	$(AR) rcs $(LOCALLIB) $(OBJECTS)

sourcerowprova: sourcerowprova.cpp ../lib/$(LOCALLIB)
	$(CXX) $(CXXOPT) -o sourcerowprova $(INCL)  sourcerowprova.cpp  $(LIBS)


sourcerowprova2: sourcerowprova2.cpp ../lib/$(LOCALLIB)
	$(CXX) $(CXXOPT) -o sourcerowprova2 sourcerowprova2.cpp $(INCL) $(LIBS)

sourcerowprova3: sourcerowprova3.cpp ../lib/$(LOCALLIB)
	$(CXX) $(CXXOPT) -o sourcerowprova3 $(INCL)  sourcerowprova3.cpp $(LIBS)
efftest: efftest.cpp ../lib/$(LOCALLIB)
	$(CXX) $(CXXOPT) -o efftest efftest.cpp -L../lib -I.. $(LIBS)

bbefftest: efftest.cpp ../lib/$(LOCALLIB)
	$(CXX) $(CXXOPT) -DBOBO -o bbefftest efftest.cpp -L../lib -I.. $(LIBS)


#implicit rules
.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) *.o
	$(RM) sourcerowprova	
	$(RM) sourcerowprova2
	$(RM) sourcerowprova3

../lib/$(LOCALLIB): $(LOCALLIB)
	cp $(LOCALLIB) ../lib


%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
        $(RM) $@.$$$$

include $(SOURCES:.cpp=.d)


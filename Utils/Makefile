include ../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
LOCALLIB=libutils.a

#libs needet to build specific targets in this directory
LIBS=-L../lib -lutils -lodbc++
#objects to be included in the library
OBJECTS=MineruleOptions.o OptionParser_parser.o  OptionParserLib.o SQLUtils.o MRLogger.o MineruleErrors.o AlgorithmTypes.o Constraints.o Bitstring.o
SOURCES=MineruleOptions.cpp OptionParserLib.cpp SQLUtils.cpp MRLogger.cpp MineruleErrors.cpp AlgorithmTypes.cpp Constraints.cpp Bitstring.cpp 

#additional targets (if any)
OTHERTARGETS=

all: OptionParser_lexer.cpp $(LOCALLIB)  ../lib/$(LOCALLIB) $(OTHERTARGETS) 

#prova:
#	bison -d -pOP -oOptionParser_parser.cpp OptionParser.ypp
#	flex -oOptionParser_lexer.cpp -POP OptionParser.lpp 
#	g++ -c -I. OptionParser_lexer.cpp
#	g++ -c -I. OptionParser_parser.cpp
#	g++ -c -I. OptionParserLib.cpp
#	g++ -o p OptionParser_parser.o OptionParser_lexer.o

# the following line shows how a target in OTHERTARGET should be made
#test: $(OBJECTS) ../lib/$(LOCALLIB) test.o
#	$(CXX) $(CXXOPT) -o test test.o $(LIBS)

OptionParser_parser.o: OptionParser.ypp
	bison -d -pOP -oOptionParser_parser.cpp OptionParser.ypp
	$(CXX) $(CXXOPT) $(INCL) -c OptionParser_parser.cpp

OptionParser_lexer.cpp: OptionParser.lpp
	flex -oOptionParser_lexer.cpp -POP OptionParser.lpp
#	$(CXX) $(CXXOPT) $(INCL) -c OptionParser_lexer.cpp

.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -DYY_NO_UNPUT -c $<


clean: defclean
	$(RM) $(OTHERTARGETS)
	$(RM) OptionParser_lexer.cpp
	$(RM) OptionParser_parser.cpp

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


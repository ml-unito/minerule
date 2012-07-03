
include ../Makefile.vars
LOCALLIB= libparsers.a

YACC=bison -d -p mr
FLEX=flex -Pmr -i

OBJECTS = MineruleParser.tab.o ParsedMinerule.o ParsedPredicate.o ParserLibrary.o PredicateBase.o lex.mr.o SupportMeasure.o
SOURCES = MineruleParser.tab.cpp ParsedMinerule.cpp ParserLibrary.cpp ParsedPredicate.cpp PredicateBase.cpp lex.mr.c SupportMeasure.cpp

all: $(LOCALLIB) ../lib/$(LOCALLIB) 

../lib/$(LOCALLIB): $(LOCALLIB)
	cp $(LOCALLIB) ../lib

$(LOCALLIB): $(OBJECTS) 
	$(AR) rcs $(LOCALLIB) $(OBJECTS)

#all: prova

#prova: $(OBJECTS) prova.cpp ParserLibrary.cpp
#	sh -c "cd ../lib; make"
#	$(CXX) $(CFLAGS) -I. -I.. -L../lib -o prova prova.cpp ParserLibrary.cpp -lfl -lmr -lodbc++

MineruleParser.tab.cpp: MineruleParser.ypp ../Utils/MinMaxPair.h ParsedMinerule.h ../Utils/Converter.h ParsedPredicate.h
	$(YACC) MineruleParser.ypp

lex.mr.c: MineruleParser.lpp ../Utils/MinMaxPair.h ParsedMinerule.h ../Utils/Converter.h  ParsedPredicate.h
	$(FLEX) MineruleParser.lpp

lex.mr.o: lex.mr.c
	$(CXX) $(CXXOPT) $(INCL) -c lex.mr.c


ParsedMinerule.o:ParsedMinerule.h ParsedMinerule.cpp MineruleParser.ypp MineruleParser.lpp
	$(CXX) $(CXXOPT) $(INCL) -c ParsedMinerule.cpp


.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) *.o
	$(RM) lex.mr.c
	$(RM) MineruleParser.tab.cpp MineruleParser.tab.hpp


#%.d: %.cpp
#	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
#        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#        $(RM) -f $@.$$$$

#%.d: %.c
#	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
#        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#        $(RM) -f $@.$$$$

#include $(SOURCES:.cpp=.d)


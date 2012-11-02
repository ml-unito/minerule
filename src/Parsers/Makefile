SOURCES=$(shell ls *.cpp)
OBJECTS=$(SOURCES:%.cpp=$(OBJDIR)/%.o)
CXXFLAGS+=-I$(LIBGISTHOME)

all:$(OBJECTS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

YACC=bison -d -p mr
FLEX=flex -Pmr -i

MineruleParser.tab.cpp: MineruleParser.ypp ../Utils/MinMaxPair.h ParsedMinerule.h ../Utils/Converter.h ParsedPredicate.h
	$(YACC) MineruleParser.ypp

lex.mr.c: MineruleParser.lpp ../Utils/MinMaxPair.h ParsedMinerule.h ../Utils/Converter.h  ParsedPredicate.h
	$(FLEX) MineruleParser.lpp

lex.mr.o: lex.mr.c
	$(CXX) $(CXXOPT) $(INCL) -c lex.mr.c


# ParsedMinerule.o:ParsedMinerule.h ParsedMinerule.cpp MineruleParser.ypp MineruleParser.lpp
# 	$(CXX) $(CXXOPT) $(INCL) -c ParsedMinerule.cpp
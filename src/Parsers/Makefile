SOURCES=$(shell ls *.cpp)
OBJECTS=$(SOURCES:%.cpp=$(OBJDIR)/%.o)
CXXFLAGS+=-I$(LIBGISTHOME)

all: MineruleParser.tab.cpp lex.mr.c  $(OBJDIR)/MineruleParser.tab.o $(OBJDIR)/lex.mr.o $(OBJECTS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@


YACC=bison -d -p mr
FLEX=flex -Pmr -i

MineruleParser.tab.cpp: MineruleParser.ypp $(INCLUDEDIR)/Utils/MinMaxPair.h $(INCLUDEDIR)/Parsers/ParsedMinerule.h $(INCLUDEDIR)/Utils/Converter.h $(INCLUDEDIR)/Parsers/ParsedPredicate.h
	$(YACC) MineruleParser.ypp

lex.mr.c: MineruleParser.lpp $(INCLUDEDIR)/Utils/MinMaxPair.h $(INCLUDEDIR)/Parsers/ParsedMinerule.h $(INCLUDEDIR)/Utils/Converter.h  $(INCLUDEDIR)/Parsers/ParsedPredicate.h
	$(FLEX) MineruleParser.lpp

# lex.mr.o: lex.mr.c
# 	$(CXX) $(CXXOPT) $(INCL) -c lex.mr.c


# ParsedMinerule.o:ParsedMinerule.h ParsedMinerule.cpp MineruleParser.ypp MineruleParser.lpp
# 	$(CXX) $(CXXOPT) $(INCL) -c ParsedMinerule.cpp
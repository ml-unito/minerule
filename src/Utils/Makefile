SOURCES=$(shell ls *.cpp)
OBJECTS=$(SOURCES:%.cpp=$(OBJDIR)/%.o)
CXXFLAGS+=-I$(LIBGISTHOME)

all: $(OBJDIR)/OptionParser_parser.o OptionParser_lexer.cpp $(OBJECTS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/OptionParser_parser.o: OptionParser_lexer.cpp OptionParser.ypp
	bison -d -pOP -oOptionParser_parser.cpp OptionParser.ypp
	$(CXX) $(CXXFLAGS) -c OptionParser_parser.cpp -o $(OBJDIR)/OptionParser_parser.o

OptionParser_lexer.cpp: OptionParser.lpp
	flex -oOptionParser_lexer.cpp -POP OptionParser.lpp


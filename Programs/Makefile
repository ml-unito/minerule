#libs needed to build specific targets in this directory
# LIBS= -L../lib -L$(LIBGAHOME)/ga -lmr -lga -lodbc++
LIBS= -L$(LIBDIR) -L$(LIBGADIR) -lmr -lodbc++ -lga

all: $(BINDIR)/minerule $(BINDIR)/mrcatalogue $(BINDIR)/mrprintrules $(BINDIR)/mrdefaults

$(BINDIR)/minerule: minerule.cpp  ../lib/libmr.a 
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/minerule minerule.cpp $(LIBS)

$(BINDIR)/mrcatalogue: mrcatalogue.cpp  ../lib/libmr.a 
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/mrcatalogue mrcatalogue.cpp $(LIBS)

$(BINDIR)/mrprintrules: PrintRules.cpp RuleFormatter.cpp RuleFormatter.h Rules.h
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/mrprintrules PrintRules.cpp RuleFormatter.cpp $(LIBS)
	
$(BINDIR)/mrdefaults: MRDefaultOptions.cpp  ../lib/libmr.a 
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/mrdefaults MRDefaultOptions.cpp $(LIBS)

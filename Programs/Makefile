#libs needed to build specific targets in this directory
LIBS= -L../lib -L$(LIBGAHOME)/ga -lmr -lga -lodbc++

all: $(BINDIR)/minerule $(BINDIR)/mrcatalogue

$(BINDIR)/minerule: minerule.cpp  ../lib/libmr.a 
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/minerule minerule.cpp $(LIBS)


$(BINDIR)/mrcatalogue: mrcatalogue.cpp  ../lib/libmr.a 
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/mrcatalogue mrcatalogue.cpp $(LIBS)



#libs needed to build specific targets in this directory
LIBS= -L../lib -L$(LIBGAHOME)/ga -lmr -lga -lodbc++

all: $(BINDIR)/minerule

# the following line shows how a target in OTHERTARGET should be made
$(BINDIR)/minerule: minerule.cpp  ../lib/libmr.a 
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/minerule minerule.cpp $(LIBS)



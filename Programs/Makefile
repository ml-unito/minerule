#libs needed to build specific targets in this directory
LIBS= -L../lib -L$(LIBGAHOME)/ga -lmr -lga -lodbc++

all: minerule

# the following line shows how a target in OTHERTARGET should be made
minerule: minerule.cpp  ../lib/libmr.a 
	$(CXX) $(CXXFLAGS) -o minerule minerule.cpp $(LIBS)



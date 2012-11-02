include ../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
LOCALLIB=NULLLIB

#libs needed to build specific targets in this directory
LIBS= -L../lib -L$(LIBGAHOME)/ga -lmr -lga -lodbc++
#LIBS=-L$(LIBODBCPLUSPLUS)  -L../lib -L$(LIBGAHOME)/ga -lmr -lga -lodbc++
#objects to be included in the library
OBJECTS=kernel.o
SOURCES=$(OBJECTS:.o=.cpp)

#additional targets (if any)
OTHERTARGETS=kernel.exe

all: $(OTHERTARGETS)

# the following line shows how a target in OTHERTARGET should be made
kernel.exe: $(OBJECTS)  ../lib/libmr.a 
	$(CXX) $(CXXOPT) -o kernel.exe kernel.o $(LIBS)
	#libtool $(CXX) $(CXXOPT) -o kernel.exe kernel.o $(LIBS)


.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) $(OTHERTARGETS)

# automatic building of dependencies
%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
        $(RM) -f $@.$$$$

include $(SOURCES:.cpp=.d)


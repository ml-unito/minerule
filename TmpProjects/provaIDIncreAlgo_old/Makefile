MRHOME = /home/minerule/sources/marcoV1.0.2
include ../../Makefile.vars
#the following var must be set to the library made in the current 
#dir or commented out (IMPORTANT: do not set it to something strange
#since the default clean target perform a rm -f $(LOCALLIB) 
#LOCALLIB=NULLLIB

#libs needet to build specific targets in this directory
LIBS=-L../../lib -L$(LIBGISTHOME)/lib -lmr -lodbc++
#objects to be included in the library
OBJECTS=idincralgo.o
SOURCES=$(OBJECTS:.o=.cpp)
INCLUDES=-w -I. -I../.. -I../.. -I../..  -DLIBGIST

#additional targets (if any)
OTHERTARGETS=idincralgo

all: $(OTHERTARGETS) 

# the following line shows how a target in OTHERTARGET should be made
idincralgo: $(OBJECTS) $(LOCALLIB) ../../lib/libmr.a
	 $(CXX) $(CXXOPT) -o idincralgo  idincralgo.o $(LIBS)


.cpp.o:
	$(CXX) $(CXXOPT) $(INCL) -c $<


clean: defclean
	$(RM) $(OTHERTARGETS)

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


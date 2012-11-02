# 
#  DIRECTORIES
# 
MRHOME=$(PWD)
INCLUDEDIR=$(PWD)/include/minerule
OBJDIR=$(PWD)/obj
SRCDIR=$(PWD)/src
EXTLIBSDIR=$(PWD)/ExtLibs
#
# ENVIRONMENT CONFIGURATION
#
CXX=g++
AR=ar
RM=rm -f
CXXFLAGS = -g3 -O0 $(INCLUDES) -Wall $(MR_VERSION_FLAG)
#CXXOPT = -O6 $(INCLUDES) -Wall $(MR_VERSION_FLAG)

#
# VERSION DEFINITION
#
MR_VERSION=2.0a
MR_VERSION_FLAG=-DMR_VERSION=\"$(MR_VERSION)\"

#
# COMPILATION FLAGS DEFINITION
#

LIBGISTHOME=$(PWD)/ExtLibs/libgist
LIBGAHOME=$(PWD)/ExtLibs/galib245
INCLUDES=-I. -I.. -I$(INCLUDEDIR) -I$(LIBGISTHOME)/include -I$(LIBGAHOME)
# N.B.: Adding -DMRUSERWARNING to CXXOPT a number of project
# related warning will show up

export INCLUDEDIR CXX AR RM CXXFLAGS MR_VERSION MR_VERSION_FLAG LIBGISTHOME OBJDIR MRHOME EXTLIBSDIR

all: build_ext_libs build_lib build_programs

clean:
	$(RM) $(OBJDIR)/*.o
	make -C $(EXTLIBSDIR) clean

$(OBJDIR):
	mkdir $(OBJDIR)

$(INCLUDEDIR):
	mkdir $(INCLUDEDIR)

build_ext_libs: $(EXTLIBSDIR)
	make -C $(EXTLIBSDIR)

build_lib: $(OBJDIR) $(INCLUDEDIR) $(SRCDIR)
	make -C $(SRCDIR)

build_programs: 
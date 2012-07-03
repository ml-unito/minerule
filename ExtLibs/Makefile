include ../Makefile.vars
export MRHOME
export LIBGISTHOME
export LIBGAHOME

$(warning ******************************* WARINING **************************************)
$(warning For readability reasons I'm disabling warning in all subdirs of this directory!)
$(warning *******************************************************************************)

all: GIST GA

GIST:
	make -C libgist/src CPPFLAGS='${CPPFLAGS} -w' all

GA:
	make -C galib245 all

clean:
	make -C libgist/src  clean
	make -C galib245

TARGETS =  PrintRules.t MRDefaultOptions.t MRCatalogue.t MRShowLog.t

CLEANTARGETS =  PrintRules.c MRDefaultOptions.c MRCatalogue.c MRShowLog.c

all: $(TARGETS)
	echo "All Made!"

clean: allclean
	echo "Clean Done!"


allclean: $(CLEANTARGETS)
	echo "Clean Done!"

%.t:
	sh -c "cd $*; make"


%.c:
	sh -c "cd $*; make clean"



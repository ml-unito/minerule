TARGETS =  ExtLibs.t Utils.t MRDatabase.t PredicateUtils.t Parser3_bis.t Algorithms.t DeveloperUtils.t  Optimizer.t    Utils.t lib.t TmpProjects.t Kernel.t

CLEANTARGETS =  ExtLibs.clean Utils.clean MRDatabase.clean PreProcessor.clean PredicateUtils.clean Parser3_bis.clean Algorithms.clean DeveloperUtils.clean  Optimizer.clean    Utils.clean lib.clean  TmpProjects.clean Kernel.clean

.PHONY:TAGS

all: $(TARGETS) TAGS
	echo "All Made!"

clean: allclean postclean
	echo "Clean Done!"


postclean:
	find . \( -name "*.d" -o -name "*.exe" -o -name "*.o" -o -name "lib*.a" -o -name "*~" -o -name "TAGS" -o -name "*.bak" \) -exec $(RM) {} \;

depclean:		
	find . -name "*.d" -exec $(RM) {} \;

allclean: $(CLEANTARGETS)
	echo "Clean Done!"
TAGS:
	rm -f TAGS
	find . \( -name "*.cpp" -o -name "*.h" -o -name "*.ypp" -o -name "*.lpp" \) -print | xargs etags --append

%.t:
	sh -c "cd $*; make"


%.clean:
	sh -c "cd $*; make clean"



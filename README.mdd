How to compile:
  From the top most directory type the following commands:
	./configure
	make
  the main executable (kernel.exe) will be found in ./Kernel

Here follows the description of the minerule directory structure.

There are the 5 branches at top level:
  Algorithms: contains the sources of the implemented mining algorithms
  MRDatabase: contains the classes used to connect to and/or to abstract the
	    Database
  DeveloperUtils: contains some utilities that may be used in the developing 
		  or debugging phase
  Optimizer: contains the classes used by the system in order to optimize 
	     the execution of Minerule queries
  TmpProject: directory to be used to store temporary new directories that 
	      may or may not become important for the project (i.e., it should 
	      be used in order to avoid to pollute the directory structure 
	      with unuseful branches)
  Parsers: contains lex, yacc and utility files used to parse the minerule 
	   queries
  PreProcessor: contains classes needed to pre-process user data before 
		calling the mine rule operator
  PredicateUtils: classes abstracting the concept of boolean predicate and
		  algorithms operating on them.
  ExtLibs: Libraries developed by third parties and needed by the system 
	   (e.g., libgist for the mining index)
  Kernel: contains the classes that coordinates the whole system
  Docs: pieces of documentation (notably: the README about the Optimizer 
	catalog can be used to create the cataloger itself if it is
	given to the mysql client).
  
  bin: place where each binary will be put
  lib: place where each library will be put

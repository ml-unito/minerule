# Minerule installation instruction

The present guide to installing the minerule has been tested using linux (mint) and postgresql.

The installation procedure on Mac OS X is very similar, with the advantage that most dependencies can be
installed using [homebrew](http://brew.sh) (to install galib, you will need to tap into homebrew/science).


##Prerequisites

We will assume that you already have a postgres database installed and configured on your system. If this is not the case, please follow one of the many postgress installation guides present on the internet (e.g., https://wiki.postgresql.org/wiki/Detailed_installation_guides).

Install developer tools and curl:

		sudo apt-get install curl g++ git cmake bison flex


##Install galib (from sources)

1. download the latest sources (v. 2.4.7):

		curl -o galib247.tgz http://lancet.mit.edu/ga/dist/galib247.tgz

2. decompress the archive and cd into it:

		tar xzvf galib247.tgz
		cd galib247

3. compile install the library

		make lib

4. unfortunately the current version of the library has a small compilation problem.
   To fix it you will have to edit the files:

		galib/ga/GA1DArrayGenome.C
		galib/ga/GA2DArrayGenome.C
		galib/ga/GA3DArrayGenome.C

   changing every occurrence of:

		initializer(…);
		mutator(…);
		comparator(…);
		crossover(…);

   into:

		this->initializer(…);
		this->mutator(…);
		this->comparator(…);
		this->crossover(…);

5. install galib
 	
		make install

## Install the minerule (from sources)

1. Grab the latest sources and cd into the minerule directory

		git clone git@github.com:boborbt/minerule.git
		cd minerule
		
2. create and configure the build directory

		mkdir build
		cd build
		cmake ../src

3. build the system

		make

4. install it

5. 		make install

## Configure options for the minerule system

You can create a default minerule configuration file by issuing the command:

		mr defaults > options.txt

Using your favorite editor, edit the option file with the information needed to access your datasource. In particular, in the mrdb option group set:

1.  name to 'name of the database you want to use'

2.  username to 'the name of a db user with enough privileges to work with the given database'

3.  password to the password of the db user

4. dbms to the name of the database server you want to use (presently only postgres is supported)

At the end of the process the head of the file should look like:

```
		mrdb::{
			+name=DataSourceName
			+username=UserName
			+password=...
			...
			+dbms=postgres
		}

		...
```

## Install the minerule catalogue

If everything is ok, you should now be able to install the minerule catalogue by using the following command:

	./mr catalogue -I

You should see some log message. In the first part of the message you should see a number of red 'MISSING' messages (they are normal, they inform you that the catalogue was not already installed). Then a number of green 'OK' messages.

If this is the case, congratulation, the minerule system is ready for your first query.

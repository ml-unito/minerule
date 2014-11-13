# Minerule installation instruction

The present guide to installing the minerule has been tested using linux (mint) and postgresql.
The installation procedure is very similar on os x, with the advantage that most dependencies can be
installed using [homebrew](http://brew.sh) (you need to tap into homebrew/science, then install galib and libodbc++ formulas).


##Prerequisites

We will assume that you already have a postgres database installed and configured on your system. If this is not the case, please follow one of the many postgress installation guides present on the internet (e.g., https://wiki.postgresql.org/wiki/Detailed_installation_guides).

Install g++ and curl:
		
		sudo apt-get install curl g++ git cmake bison flex


##Install odbc and related libraries

1. Install obdc-postgresql drivers:

		sudo apt-get install odbc-postgresql

2. Install unixodbc configuration manager:

		sudo apt-get install unixodbc-bin
	
3. Install unixodbc headers:

		sudo apt-get install unixodbc-dev

##Install libodbc++ library (from sources):

1. download the latest sources (v.0.2.5) from sourceforge.net

		curl -L -o libodbcxx.tar.gz http://downloads.sourceforge.net/project/libodbcxx/libodbc++/0.2.5/libodbc++-0.2.5.tar.gz

2. decompress the archive and cd into it:

		tar xzvf libodbcxx.tar.gz <br>
		cd libodbc++-0.2.5

3. Unfortunately the library contains a small error that prevents it to be compiled under linux. To solve the problem you need to use your favorite text editor (e.g., gedit, vim, emacs, ...) and add the following line at the very top of file src/datastream.h:
	
		#include <cstdio>

4. run configure and make:

		./configure
		./make
	
5. run the installation script: 

		sudo make install
	
6. step out of the source directory

		cd ..
	
7. (optional) remove the sources from your system:
	
		rm -rf libodbc++*
		
##Install galib (from sources)

1. download the latest sources (v. 2.4.7):

		curl -o galib247.tgz http://lancet.mit.edu/ga/dist/galib247.tgz
	
2. decompress the archive and cd into it:

		tar xzvf galib247.tgz
		cd galib247
	
3. compile and install the library

		make lib<br>
		sudo make install

## Install the minerule (from sources)

<!-- FIXME il minerule non potrà  essere scaricato da kdd in questo modo!! -->

1. Grab the latest sources and cd into the minerule directory 

		git clone ssh://mluser@kdd.di.unito.it/usr/local/GIT/minerule<br>
		cd minerule
		
2. Decompress the gist library in the ExtLibs subdirectory

		tar -C ExtLibs -xzvf libgist.tar.gz
	
3. create and configure the build directory

		mkdir build
		cd build
		cmake ../src
	
3. build the system

		make
		
4. install it

5. 		make install


## Configure your odbc datasources
The minerule system will access to your database through odbc. Then, before you can use it, it is necessary to configure the datasources needed to let the system access your data. In the following we will assume that you need to configure a odbc datasource to access a schema named 'testdb' with a user named 'user1' using password 'pwd'.

1. Launch the odbc utilty for creating datasources:

		ODBCCreateDataSourceQ4
		
2. A window dialog will appear asking you the type of the datasources. Choose 'User' and click 'Next'.

3. Select the first row of the table shown in the new dialog and click 'Next'.

4. The new dialog will show you a table with some field to be compiled, leave everything untouched except for:

	a. Name: set it to 'test' (this is the name of the datasource)
	
	b. Database: set it to 'testdb'
	
	c. Username: set it to 'user1'
	
	d. Password: set it to 'pwd'

5. Click 'Finish' to save the configuration.

## Configure options for the minerule system

From the build directory (or wherever you have access to the minerule binaries if you moved them), launch the 'defaults' sub-command to create a new option file:

		mr defaults > options.txt

Using your favorite editor, edit the option file with the information needed to access to the newly created odbc datasource. In particular, in the obdc option group set:

1.	name to 'test'

2.  username to 'user1'

3.  password to 'pwd'

4. dbms to 'postgres'

At the end of the process the head of the file should look like:

		odbc::{
			+name=test
			+username=user1			
			+password=pwd
			...
			+dbms=postgres
		}
		
		...

## Install the minerule catalogue

If everything is ok, you should now be able to install the minerule catalogue by using the following command:

	./mr catalogue -I
	
You should see some log message. In the first part of the message you should see a number of red 'MISSING' messages (they are normal, inform you that the catalogue was not already installed). Then a number of green 'OK' messages.

If this is the case, congratulation, the minerule system is ready for your first query.

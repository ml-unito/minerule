# Minerule installation instruction

The present guide to installing the minerule has been tested using linux (mint) and postgresql.

The installation procedure on Mac OS X is very similar, with the advantage that most dependencies can be
installed using [homebrew](http://brew.sh) (you need to tap into homebrew/science, then install galib and libodbc++ formulas).

On OS X, in addition to the above mentione formulas, you will need to install the unixodbc manager (``brew install unixodbc``) and the postgres odbc driver (``brew install psqlodbc``). Also, the state of guis odbc managers is pretty bad on this platform. You may want to manually configure the relevant files by editing ``/usr/local/Cellar/unixodbc/2.3.2_1/etc/odbcinst.ini`` so to look similar to (we are assuming you did install unixodbc through homebrew):

```ini
[postgres]
Description     = PostgreSQL driver
Driver          = /usr/local/lib/psqlodbcw.so
Setup           =
FileUsage       = 1
```

and  ~/.odbc.ini to look similar to:

```ini
[DataSourceName]
Description         = DataSource description
Driver              = postgres
Trace               = Yes
TraceFile           = sql.log
Database            = DataBaseName
Servername          = localhost
UserName            = UserName
Password            = Password
Port                = 5432
Protocol            = 6.4
ReadOnly            = No
RowVersioning       = No
ShowSystemTables    = No
ShowOidColumn       = No
FakeOidIndex        = No
ConnSettings        =
```
where ``DataSourceName``, ``DataBaseName``, ``UserName``, and ``Password`` needs to be changed to match your needs.

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


## Configure your odbc datasources
The minerule system will access to your database through odbc. Then, before you can use it, it is necessary to configure the datasources needed to let the system access your data. In the following we will assume that you need to configure a odbc datasource to access a schema named 'testdb' with a user named 'user1' using password 'pwd'.

1. Launch the odbc utilty for creating datasources:

		ODBCCreateDataSourceQ4

2. A window dialog will appear asking you the type of the datasources. Choose 'User' and click 'Next'.

3. Select the first row of the table shown in the new dialog and click 'Next'.

4. The new dialog will show you a table with some field to be compiled, leave everything untouched except for:

	a. Name: set it to the name you want to give to this datasource (in the following we will assume it to be DataSourceName)

	b. Database: set it to the name of the database as seen by postgres (in the following we will assume it to be 'DataBaseName')

	c. Username: set it to a user name recognized by postgres and with enough privileges to work on DataBaseName (in the following we will assume it to be "UserName")

	d. Password: set it to the password of UserName

5. Click 'Finish' to save the configuration.

## Configure options for the minerule system

From the build directory (or wherever you have access to the minerule binaries if you moved them), launch the 'defaults' sub-command to create a new option file:

		mr defaults > options.txt

Using your favorite editor, edit the option file with the information needed to access to the newly created odbc datasource. In particular, in the obdc option group set:

1.	name to 'DataSourceName'

2.  username to 'UserName'

3.  password to the password of UserName

4. dbms to 'postgres'

At the end of the process the head of the file should look like:

```
		odbc::{
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

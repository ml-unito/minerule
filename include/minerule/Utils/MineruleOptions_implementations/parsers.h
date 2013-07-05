//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
class Parsers : public OptionBase {
   FILE* logfile;
   MinMaxPair bodyCardinalities;
   MinMaxPair headCardinalities;

   void clearStream() {
	if(logfile!=NULL && logfile!=stdout && logfile!=stderr) {
	  fclose(logfile);
	}
	logfile=NULL;
   }
 public:
   Parsers() : logfile(NULL), 
	bodyCardinalities(MinMaxPair(1,1000)),
	headCardinalities(MinMaxPair(1,1000)) {};

   virtual ~Parsers() {
	clearStream();
   }
   
   virtual std::string className() const {
	return "parsers";
   }

   virtual void setOption(const std::string& name, const std::string& value)
	throw(MineruleException);

   void setLogFILE(const std::string& fname) throw(MineruleException);
   void setLogOnStdout();
   void setLogOnStderr();

   const FILE* getLogFILE() const {
	assert( logfile!=NULL );
	return logfile;
   }

   void setMinBodyElems(int m) {
	bodyCardinalities.setMin(m);
   }

   void setMaxBodyElems(int M) {
	bodyCardinalities.setMax(M);
   }

   void setMinHeadElems(int m) {
	headCardinalities.setMin(m);
   }

   void setMaxHeadElems(int M) {
	headCardinalities.setMax(M);
   }

   const MinMaxPair& getBodyCardinalities() const {
	return bodyCardinalities;
   }

   const MinMaxPair& getHeadCardinalities() const {
	return headCardinalities;
   }
 };

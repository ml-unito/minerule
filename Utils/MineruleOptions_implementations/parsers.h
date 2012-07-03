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

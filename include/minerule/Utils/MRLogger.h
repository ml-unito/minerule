#include <vector>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <sstream>
#include <map>


namespace minerule {

	class MRLogger {
	private:
		class LogInfo {
		public:
			std::string indent;
			clock_t cpu;
			struct timeval time;

			LogInfo(const LogInfo& li) : indent(li.indent), cpu(li.cpu), time(li.time) { }

			LogInfo(const std::string& ind) :
			indent(ind), cpu(clock()) {
				gettimeofday(&time,NULL);
			};

			LogInfo& operator=(const LogInfo& li) {
				indent=li.indent;
				cpu=li.cpu;
				time=li.time;
				return *this;
			}
		}; // LogInfo
		
		class MeasurementInfo {
		public:
			LogInfo start;
			double totCpu;
			double totTime;
			bool startIsValid;
			
			MeasurementInfo() : start(""), totCpu(0.0), totTime(0.0), startIsValid(false) {};
		};


		static const std::string START_SEPARATOR;
		static const std::string CONT_SEPARATOR;
		static const std::string END_SEPARATOR;
		static std::ofstream nullLog;

		typedef std::vector< LogInfo > LogStack;
		typedef std::map< std::string, MeasurementInfo > Measurements;
		
		LogStack logStack;
		Measurements measurements;
		
		std::string indentString;
		std::string indentInset;
		std::ostream* os;
		size_t logLevel;    // the desired logLevel
		size_t curLogLevel; // the current logLevel

		void indent() {
			*os<<indentString;
		}
		
		double getTimeSecs(const LogInfo& last, const LogInfo& first) const {
			return double(last.time.tv_sec-first.time.tv_sec)
				+double((last.time.tv_usec-first.time.tv_usec)/1e6);
		}

		double getCpuSecs(const LogInfo& last, const LogInfo& first) const {
			return double(last.cpu-first.cpu)/double(CLOCKS_PER_SEC);
		}

		std::string evalTimeMemInfo(const LogInfo& li) const;

		void logMeasurement(const std::string& description, const MeasurementInfo& data);
	public:
		MRLogger(std::ostream& ostr);
		MRLogger();
		~MRLogger(void);

		void setStream(std::ostream& ostr);
		std::ostream* getStream() const {
			return os;
		}

		void updateIndentString();
		void push(const std::string& descr);
		void pop();

		double getCurrentCpuSecs() const;
		double getCurrentTimeSecs() const;
		double getCurrentCpuDelta() const;
		double getCurrentTimeDelta() const;
    

		std::ostream& log() {
			if(curLogLevel>logLevel)
				return nullLog;
			indent();
			*os<<" ";
			return *os;
		}

		size_t getLogLevel() {
			return logLevel;
		}

		void setLogLevel(size_t newLevel) {
			logLevel=newLevel;
		}
		
		size_t getIndentLen() { return indentString.size(); }
		
		void startMeasuring(std::string description) {
			MeasurementInfo& m = measurements[description];
			assert(!m.startIsValid);
			
			m.start = LogInfo(""); // saving start time
			m.startIsValid = true;
		}
		
		void stopMeasuring(std::string description) {
			MeasurementInfo& m = measurements[description];
			LogInfo stop("");
			
			assert(m.startIsValid);
			
			m.totCpu += getCpuSecs(stop, m.start);
			m.totTime += getTimeSecs(stop, m.start);
			m.startIsValid = false;			
		}
		
		void logMeasurements();
		
	}; // MRLogger


} // namespace

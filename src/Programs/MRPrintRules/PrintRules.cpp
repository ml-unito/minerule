#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>

#include <iostream>
#include <assert.h>

#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Optimizer/OptimizerCatalogue.h"
#include "minerule/Database/SourceRowElement.h"
#include "minerule/Utils/FileUtils.h"
#include "minerule/Result/RuleFormatter.h"

#include "Options.h"


using namespace minerule;

namespace mrprint {

	void
	printRules(std::string queryname,	RuleFormatter& formatter, double conf) throw (MineruleException, odbc::SQLException, std::exception) {
		QueryResult::Iterator qit;
		OptimizerCatalogue::getMRQueryResultIterator( queryname, qit, -1, conf );
    
		while( qit.next() ) {
			Rule r;
			qit.getRule(r);
        
			formatter.printRule(r);
		}
    
		formatter.postExec();
	}
	
	
	std::string getQueryName(const Options& options) {
		if( !options.queryName().empty() )
			return options.queryName();
		
		return minerule::OptimizerCatalogue::getMRQueryName(options.queryNumber());
	}
	
	
	RuleFormatter* newFormatter(const Options& options) {
		RuleFormatter* rf = NULL;
		std::string sortOrder = options.sortOrder();
		
		if(sortOrder=="no") {
				rf=new SimpleRuleFormatter();
		} else if(sortOrder=="scbh") {
				rf=new SortedRuleFormatter<QueryResult::SortSuppConfBodyHead>();
		} else if(sortOrder=="bhsc") {
				rf=new SortedRuleFormatter<QueryResult::SortBodyHeadSuppConf>();
		} else if(sortOrder=="hbsc") {
				rf=new SortedRuleFormatter<QueryResult::SortHeadBodySuppConf>();
		} else if(sortOrder=="csbh") {
				rf=new SortedRuleFormatter<QueryResult::SortConfSuppBodyHead>();
		} else if(sortOrder=="cbhs") {
				rf=new SortedRuleFormatter<QueryResult::SortConfBodyHeadSupp>();
		} else if(sortOrder=="cbsh") {
				rf=new SortedRuleFormatter<QueryResult::SortConfBodySuppHead>();
		} else {
				printError("Unsupported sort order:" + sortOrder);
				exit(MRPRINT_OPTION_PARSING_ERROR);
		}
    
		if(rf==NULL) {
			rf = new SimpleRuleFormatter();
		}
        
		if( options.noLogArtifacts() )
			rf->setSuppressLog(true);
		
		return rf;
	}

}

int main(int argc, char** argv) {    
	try {
		MineruleOptions& mr = MineruleOptions::getSharedOptions();
		
		RuleFormatter* rf=NULL;
		double conf = -1;

		mrprint::Options options(argc, argv);		
		options.parse();

		mr.readFromFile(options.mroptFileName());

		rf = newFormatter(options);
		if( options.noLowConfidenceFilter() ) {
			conf = 0.0;
		}
		
		
		std::string queryName = getQueryName(options);

		if(!rf->suppressLog()) MRLogPush("Printing result set...");
		
		mrprint::printRules( queryName, *rf, conf);
		
		if(!rf->suppressLog()) MRLogPop();
		
		
		delete rf;
	} catch ( minerule::MineruleException& e ) {
		MRErr() << "MineruleError:" << e.what() << std::endl;
		throw;
	}  catch( odbc::SQLException& e) {
		MRErr() << "SQLError:" << e.what() << std::endl;
		throw;
	}  catch (std::exception& e) {
		MRErr() << "Couldn't execute your request, the reason is:" 
			<< e.what() << std::endl;
		throw;
	}  catch (...) {
		MRErr() << "Uncought exception!" << std::endl;
		throw;
	}
    
	return 0;
}

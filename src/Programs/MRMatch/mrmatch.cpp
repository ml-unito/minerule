#include <iostream>
#include <getopt.h>

#include "minerule/Result/RulesMatcher.h"
#include "minerule/Result/RuleFormatter.h"

#include "Matcher.h"
#include "mrmatch.h"
#include "Options.h"

using namespace minerule;
 
namespace mrmatch {
	
	SourceTableRequirements sourceTableRequirements(const ParsedMinerule& minerule) {
		if(minerule.hasCrossConditions() || !OptimizerCatalogue::hasIDConstraints(minerule)) 
			return SourceTableRequirements( SourceTableRequirements::CrossProduct | SourceTableRequirements::SortedGids );
		else
			return SourceTableRequirements();		
	}
		
	void execute(const Options& options) {	
		MRLogPush("Building source table...");
		// rebuild source table
		CatalogueInfo info;
		OptimizerCatalogue::getMRQueryInfo(options.queryName(), info, false);
		ParsedMinerule minerule(info.qryText);
		if( options.tableName() != "" ) {
			minerule.tab_source = options.tableName();
		}
		
		SourceTableRequirements requirements = sourceTableRequirements(minerule);		
		SourceTable st( minerule, requirements);
		MRLogPop();

		MRLogPusher("Reading rules...");
		// load past minerule result
		QueryResult::Iterator it;
		OptimizerCatalogue::getMRQueryResultIterator(options.queryName(), it, minerule.sup, minerule.conf);
		
		std::auto_ptr<Matcher> matcher(Matcher::newMatcher(options.matchKind()));

		while(it.next()) {
			Rule& rule = matcher->addRule();
			it.getRule(rule);
		}
		MRLogPop();
				
		MRLogPush("Matching...");
		matcher->match(st);
		MRLogPop();
		
		MRLogPush("Printing results:");
		matcher->printMatches( );
		
		MRLogPop();
	}
	
}


int main (int argc, char* const argv[]) {
	try {
		mrmatch::Options options;
	
		options.parse(argc, argv);
		if(!options.initMineruleOptions()) {
			std::cout << "Minerule Options not specified." << std::endl
				<< "They can be specified in several ways, you can:" << std::endl
				<< " provide the path to an option file using flag " << StringUtils::toBold("-f") << "." << std::endl
				<< " launch the program from a directory containing an option file named 'options.txt'" << std::endl	
				<< " provide the options on the command line using " << StringUtils::toBold("-O") << " flags." << std::endl;

			exit(mrmatch::MRMATCH_OPTION_PARSING_ERROR);
		}
		
		execute(options);
	
	} catch( std::exception& e ) {
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}
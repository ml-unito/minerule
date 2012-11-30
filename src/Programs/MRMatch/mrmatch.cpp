#include <iostream>
#include <getopt.h>

#include "Result/RulesMatcher.h"
#include "mrmatch.h"
#include "Options.h"

using namespace minerule;

namespace mrmatch {

	void printUsage(int argc, char* const argv[]) {
		char* progName = argv[0];
		
		std::cout << StringUtils::toBold("Usage:") << std::endl
			<< "  " << StringUtils::toBold(progName) << " [-n <num query>] [-O <optionlist>] [-v] [-h] [<query name>] " << std::endl << std::endl;
	
		std::cout 
			<< StringUtils::toBold("   -n") << " -- specifies a query number (instead of a query name)" << std::endl
			<< StringUtils::toBold("   -O") << " -- specifies a minerule option on the command line (overrides those read from file)" << std::endl
			<< StringUtils::toBold("   -f") << " -- specify a file name containing the Options to be used." << std::endl
			<< "         default is 'optins.txt'"<< std::endl;
			// << StringUtils::toBold("   -v") << " -- version informations" << std::endl
			// << StringUtils::toBold("   -h") << " -- this message" << std::endl
  
		exit(SUCCESS);
	}

	Options parseOptions(int argc,  char* const argv[]) {
		char ch;
		Options options;
		
		while( (ch=getopt(argc, argv, "hn:")) != -1 ) {
			switch(ch) {
				case 'n':
				options.setQueryNumber(Converter(optarg).toLong());
				break;
				case '?':
				default:
				std::cout << StringUtils::toBold("Option not recognized:") << "-" << ch << endl;
				printUsage(argc, argv);
				exit(MRMATCH_OPTION_PARSING_ERROR);
			}
		}
	
		if( optind < argc-1 ) {
			cout << StringUtils::toBold("Error:") << "some of the options were not recognized" << std::endl;
			
			printUsage(argc, argv);
			exit(MRMATCH_OPTION_PARSING_ERROR);
		}
		
		if( optind == argc-1 )
			options.setQueryName(argv[argc-1]);
	
		return options;
	}
	
	SourceTableRequirements sourceTableRequirements(const ParsedMinerule& minerule) {
		if(minerule.hasCrossConditions() || !minerule.hasIDConstraints()) 
			return SourceTableRequirements( SourceTableRequirements::CrossProduct | SourceTableRequirements::SortedGids );
		else
			return SourceTableRequirements();		
	}
	
	void matchWithCrossProduct(RuleGidsVector& rules, SourceTable& st) {
		assert( false ); // not yet implemented
	}
	
	void matchWithoutCrossProduct(RuleGidsVector& rules, SourceTable& st) {
		SourceTable::Iterator bodyIt = st.newIterator(SourceTable::BodyIterator);
		SourceTable::Iterator headIt = st.newIterator(SourceTable::HeadIterator);
				
		while(!bodyIt.isAfterLast()) {
			ItemType gid = bodyIt->getGroup();
			
			ItemTransaction<RulesMatcher::SetType> bodies;
			ItemTransaction<RulesMatcher::SetType> heads;
			
			bodies.loadBody(gid, bodyIt); 			// this advances the body iterator
			
			if( TransactionBase<RulesMatcher::SetType>::findGid(gid, headIt) ) // positioning the head iterator 
				break;								// no more heads to load

			heads.loadHead(gid, headIt);			// loading the heads
			
			// populating results
			for(RuleGidsVector::iterator ruleIt = rules.begin(); ruleIt!=rules.end(); ++ruleIt) {
				if( RulesMatcher::match( ruleIt->first, bodies, heads ) ) {
					ruleIt->second.push_back( gid );
				}
			}
			
		}
		
	}
	
	void printMatches( const RuleGidsVector& matches ) {
		for(RuleGidsVector::const_iterator it=matches.begin(); it!=matches.end(); ++it) {
			
		}
	}
	
	void execute(const Options& options) {		
		// rebuild source table
		CatalogueInfo info;
		OptimizerCatalogue::getMRQueryInfo(options.queryName(), info, false);
		ParsedMinerule minerule(info.qryText);
		
		SourceTableRequirements requirements = sourceTableRequirements(minerule);		
		SourceTable st( minerule, requirements);

		// load past minerule result
		QueryResult::Iterator it;
		OptimizerCatalogue::getMRQueryResultIterator(options.queryName(), it, minerule.sup, minerule.conf);
		RuleGidsVector rules;

		while(it.next()) {
			rules.push_back( std::pair<Rule, std::vector<ItemType> >() );
			it.getRule(rules.back().first);
		}
		
		// perform the match
		if( requirements.crossProduct() ) {
			matchWithCrossProduct(rules, st);			
		} else {
			matchWithoutCrossProduct(rules,st);
		}
		
		// show results
		printMatches( rules );
	}
	
}


int main (int argc, char* const argv[]) {
	try {
		mrmatch::Options options;
	
		options = mrmatch::parseOptions(argc, argv);
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
		cerr << e.what() << std::endl;
	}
	
	return 0;
}
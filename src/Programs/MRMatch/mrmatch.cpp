#include <iostream>
#include <getopt.h>

#include "Result/RulesMatcher.h"
#include "Result/RuleFormatter.h"
#include "mrmatch.h"
#include "Options.h"

using namespace minerule;

namespace mrmatch {

	void printUsage(int argc, char* const argv[]) {
		char* progName = argv[0];
		
		std::cout << StringUtils::toBold("Usage:") << std::endl
			<< "  " << StringUtils::toBold(progName) << " [-n <num query>] [-O <optionlist>] [-v] [-h] [<query name>] " << std::endl << std::endl;
	
		std::cout
			<< StringUtils::toBold("   -h") << " - prints this message." << std::endl
			<< StringUtils::toBold("   -c") << " - suppress colors."
			<< StringUtils::toBold("   -n") << " - specifies a query number (instead of a query name)." << std::endl
			<< StringUtils::toBold("   -O") << " - specifies a minerule option on the command line (overrides those read from file)." << std::endl
			<< StringUtils::toBold("   -f") << " - specify a file name containing the Options to be used." << std::endl
			<< "         default is 'optins.txt'"<< std::endl;
			// << StringUtils::toBold("   -v") << " -- version informations" << std::endl
			// << StringUtils::toBold("   -h") << " -- this message" << std::endl
  
		exit(SUCCESS);
	}

	Options parseOptions(int argc,  char* const argv[]) {
		char ch;
		Options options;
		
		while( (ch=getopt(argc, argv, "hcn:")) != -1 ) {
			switch(ch) {
				case 'h':
					printUsage(argc, argv);
					break;
				case 'c':
					StringUtils::setColorsEnabled(false);
					break;
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
						
			if( !TransactionBase<RulesMatcher::SetType>::findGid(gid, headIt) ) {// positioning the head iterator  
				break;								// no more heads to load
			}

			heads.loadHead(gid, headIt);			// loading the heads
			
			// populating results
			SimpleRuleFormatter sf;
			
			for(RuleGidsVector::iterator ruleIt = rules.begin(); ruleIt!=rules.end(); ++ruleIt) {				
				if( RulesMatcher::match( ruleIt->first, bodies, heads ) ) {
					ruleIt->second.push_back( gid );
				}
			}
			
		} // while
		
	} // matchWithoutCrossProduct
	
	std::string formatGids( const std::vector<ItemType>& gids ) {
		std::stringstream str;
		for(std::vector<ItemType>::const_iterator it = gids.begin(); it!=gids.end(); ++it) {
			str << *it << " ";
		}
		
		return str.str();
	}
	
	void printMatches( const RuleGidsVector& matches ) {
		SimpleRuleFormatter sf;
		sf.setFieldWidths( SimpleRuleFormatter::FieldWidths(10,10,9,9) );
		for(RuleGidsVector::const_iterator it=matches.begin(); it!=matches.end(); ++it) {
			MRLog() 	<< StringUtils::toBold("Rule: ") << sf.formatRule(it->first) << " "
						<< StringUtils::toBold("Gids: ") << formatGids(it->second) << endl;
		}
	}
	
	void execute(const Options& options) {	
		MRLogPush("Building source table...");
		// rebuild source table
		CatalogueInfo info;
		OptimizerCatalogue::getMRQueryInfo(options.queryName(), info, false);
		ParsedMinerule minerule(info.qryText);
		
		SourceTableRequirements requirements = sourceTableRequirements(minerule);		
		SourceTable st( minerule, requirements);
		MRLogPop();

		MRLogPusher("Reading rules...");
		// load past minerule result
		QueryResult::Iterator it;
		OptimizerCatalogue::getMRQueryResultIterator(options.queryName(), it, minerule.sup, minerule.conf);
		RuleGidsVector rules;

		while(it.next()) {
			rules.push_back( std::pair<Rule, std::vector<ItemType> >() );
			it.getRule(rules.back().first);
		}
		MRLogPop();
		
		
		MRLogPush("Matching...");
		// perform the match
		if( requirements.crossProduct() ) {
			matchWithCrossProduct(rules, st);			
		} else {
			matchWithoutCrossProduct(rules,st);
		}
		MRLogPop();
		
		MRLogPush("Printing results:");
		// show results
		printMatches( rules );
		
		MRLogPop();
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
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
#include <string.h>
#include "minerule/mrdb/ResultSet.hpp"
#include "minerule/mrdb/Connection.hpp"
#include "minerule/mrdb/Statement.hpp"
#include "minerule/mrdb/PreparedStatement.hpp"

#include <iostream>
#include <assert.h>

#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/Optimizer/OptimizerCatalogue.hpp"
#include "minerule/Database/SourceRowElement.hpp"
#include "minerule/Utils/FileUtils.hpp"
#include "minerule/Result/RuleFormatter.hpp"

#include "Options.hpp"


using namespace minerule;

namespace mrprint {

	void
	printRules(std::string queryname,	RuleFormatter& formatter, double conf) {
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
	}  catch( mrdb::SQLException& e) {
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

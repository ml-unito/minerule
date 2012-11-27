#include <iostream>


int main (int argc, char const *argv[]) {
	
	
	RulesMatcher matcher(rules);
	while(!it->isAfterLast()) {
		if( mather.match(*it, matchingRules) ) {
			cout << "Rules:" << matchingRules << "match group:" << it->getGroup() << endl;
		}
	}
	
	ResultsetMatcher matcher(resultSet);
	for( it = resultSet.begin(); it!=resultSet.end(); ++it) {
		if( matcher.match(*it) )
	}
	
	return 0;
}
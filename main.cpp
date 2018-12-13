#include <iostream>
#include <fstream>
#include <sstream>
#include "database.h"

using namespace std;

int main(int argc, char* argv[]) {
	//OPEN FILE/CHECK ERRORS
	ifstream in(argv[1]);

	Lexer myLexer;
	Parser myParser;
	Database myDatabase;

	myLexer.scan(in);
	myParser.parse(myLexer.getTokens());
	myDatabase.doSchemes(myParser.getSchemes());
	myDatabase.doFacts(myParser.getFacts());

	vector<Rule> rules = myParser.getRules();

	cout << myDatabase.printOptimizedRuleResults(rules);
	myDatabase.doQueries(myParser.getQueries());
	cout << myDatabase.printQueryResults();

	in.close();
	return 0;
}

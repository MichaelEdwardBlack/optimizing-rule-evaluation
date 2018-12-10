#include <iostream>
#include <fstream>
#include <sstream>
#include "graph.h"

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
	Graph myGraph(rules);
	cout << myGraph.printGraph();
	cout << myGraph.printReverseGraph();
	/*
	myDatabase.doRules(myParser.getRules());
	cout << myDatabase.printRuleResults();
	myDatabase.doQueries(myParser.getQueries());
	cout << myDatabase.printQueryResults();
	*/
	in.close();
	return 0;
}

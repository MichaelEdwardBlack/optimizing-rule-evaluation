#include "lexer.h"
#include "rule.h"
#include "expression.h"
#include <vector>
#include <set>

using namespace std;

class Parser {
public:
	Parser();
	void error();
	void advance(vector<Token>& tokens);
	void matchTerminal(Token::TOKEN_NUM token_type, vector<Token>& tokens);
	string operators(vector<Token>& tokens);
	string expression(vector<Token>& tokens);
	void parameter(vector<Token>& tokens);
	void parameterList(vector<Token>& tokens);
	void stringList(vector<Token>& tokens);
	void idList(vector<Token>& tokens);
	void predicateFact(vector<Token>& tokens);
	void headPredicate(vector<Token>& tokens);
	void predicateNoDomain(vector<Token>& tokens);
	void predicate(vector<Token>& tokens);
	void predicateListNoDomain(vector<Token>& tokens);
	void predicateList(vector<Token>& tokens);
	void scheme(vector<Token>& tokens);
	void schemeList(vector<Token>& tokens);
	void fact(vector<Token>& tokens);
	void factList(vector<Token>& tokens);
	void rule(vector<Token>& tokens);
	void ruleList(vector<Token>& tokens);
	void query(vector<Token>& tokens);
	void queryList(vector<Token>& tokens);
	void parse(vector<Token> tokens);
	vector<Predicate> getSchemes();
	vector<Predicate> getFacts();
	vector<Rule> getRules();
	vector<Predicate> getQueries();
	string toString();
private:
	int currentToken;
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	vector<Parameter> currentParam;
	vector<Predicate> currentPred;
	set<string> domain;
};

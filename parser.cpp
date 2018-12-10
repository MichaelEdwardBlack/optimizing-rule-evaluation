#include "parser.h"

Parser::Parser() {
	currentToken = 0;
}
void Parser::error() {
	throw currentToken;
}

void Parser::advance(vector<Token>& tokens) {
	int sizeToken = tokens.size();
	if (currentToken < sizeToken) {
		currentToken++;
	}
	else {
		matchTerminal(Token::ENDFILE, tokens);
	}
}

void Parser::matchTerminal(Token::TOKEN_NUM token_type, vector<Token>& tokens) {
	if (tokens[currentToken].getType() == Token::TOKEN_TYPE[token_type]) {
		advance(tokens);
	}
	else error();
}

string Parser::operators(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "ADD") {
		matchTerminal(Token::ADD, tokens);
		return "+";
	}
	else {
		matchTerminal(Token::MULTIPLY, tokens);
		return "*";
	}
}
string Parser::expression(vector<Token>& tokens) {
	Expression tempExpression;
	string param1;
	string theOp;
	string param2;
	matchTerminal(Token::LEFT_PAREN, tokens);
	if (tokens[currentToken].getType() == "LEFT_PAREN") {
		param1 = expression(tokens);
	}
	else {
		param1 = tokens[currentToken].getValue();
		//should be able to take a string or id
		if (tokens[currentToken].getType() == "ID") {
			matchTerminal(Token::ID, tokens);
		}
		else if (tokens[currentToken].getType() == "STRING") {
			matchTerminal(Token::STRING, tokens);
		}
	}
	theOp = operators(tokens);
	if (tokens[currentToken].getType() == "LEFT_PAREN") {
		param2 = expression(tokens);
	}
	else {
		//should be able to take a string or id
		param2 = tokens[currentToken].getValue();
		if(tokens[currentToken].getType() == "ID") {
			matchTerminal(Token::ID, tokens);
		}
		else if (tokens[currentToken].getType() == "STRING") {
			matchTerminal(Token::STRING, tokens);
		}
	}
	tempExpression = Expression(param1, theOp, param2);
	matchTerminal(Token::RIGHT_PAREN, tokens);
	return tempExpression.toString();
}
void Parser::parameter(vector<Token>& tokens) {
	string param = tokens[currentToken].getValue();
	if (tokens[currentToken].getType() == "STRING") {
		currentParam.push_back(Parameter(true, tokens[currentToken].getType(), tokens[currentToken].getValue()));
		matchTerminal(Token::STRING, tokens);
	}
	else if (tokens[currentToken].getType() == "ID") {
		currentParam.push_back(Parameter(false, tokens[currentToken].getType(), tokens[currentToken].getValue()));
		matchTerminal(Token::ID, tokens);
	}
	else if (tokens[currentToken].getType() == "LEFT_PAREN") {
		currentParam.push_back(Parameter(false, "expression", expression(tokens)));
	}
	else error();
}
void Parser::parameterList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "COMMA") {
		matchTerminal(Token::COMMA, tokens);
		parameter(tokens);
		parameterList(tokens);
	}
}
void Parser::stringList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "COMMA") {
		matchTerminal(Token::COMMA, tokens);
		if (tokens[currentToken].getType() == "STRING") {
			currentParam.push_back(Parameter(true, tokens[currentToken].getType(), tokens[currentToken].getValue()));
			domain.emplace(tokens[currentToken].getValue());
			matchTerminal(Token::STRING, tokens);
			stringList(tokens);
		}
	}
}
void Parser::idList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "COMMA") {
		matchTerminal(Token::COMMA, tokens);
		if (tokens[currentToken].getType() == "ID") {
			currentParam.push_back(Parameter(false, tokens[currentToken].getType(), tokens[currentToken].getValue()));
			matchTerminal(Token::ID, tokens);
			idList(tokens);
		}
	}
}
void Parser::headPredicate(vector<Token>& tokens) {
	currentParam.clear();
	if (tokens[currentToken].getType() == "ID") {
		parameter(tokens);
		string currentName = currentParam.front().toString();
		currentParam.clear();
		matchTerminal(Token::LEFT_PAREN, tokens);
		if (tokens[currentToken].getType() == "ID") {
			parameter(tokens);
		}
		else error();
		idList(tokens);
		matchTerminal(Token::RIGHT_PAREN, tokens);
		currentPred.push_back(Predicate(currentName, currentParam));
	}
}
void Parser::predicate(vector<Token>& tokens) {
	currentParam.clear();
	parameter(tokens);
	string currentName = currentParam.front().toString();
	currentParam.clear();
	matchTerminal(Token::LEFT_PAREN, tokens);
	parameter(tokens);
	parameterList(tokens);
	matchTerminal(Token::RIGHT_PAREN, tokens);
	currentPred.push_back(Predicate(currentName, currentParam));
}
void Parser::predicateList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "COMMA") {
		matchTerminal(Token::COMMA, tokens);
		predicate(tokens);
		predicateList(tokens);
	}
}
void Parser::scheme(vector<Token>& tokens) {
	currentPred.clear();
	headPredicate(tokens);
	if (currentPred.empty()) {
		error();
	}
	else schemes.push_back(currentPred[0]);
}
void Parser::schemeList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "ID") {
		scheme(tokens);
		schemeList(tokens);
	}
}
void Parser::fact(vector<Token>& tokens) {
	currentPred.clear();
	string factName = tokens[currentToken].getValue();
	matchTerminal(Token::ID, tokens);
	matchTerminal(Token::LEFT_PAREN, tokens);
	if (tokens[currentToken].getType() == "STRING") {
		currentParam.clear();
		domain.emplace(tokens[currentToken].getValue());
		parameter(tokens);
	}
	else error();
	stringList(tokens);
	matchTerminal(Token::RIGHT_PAREN, tokens);
	currentPred.push_back(Predicate(factName, currentParam));
	facts.push_back(currentPred[0]);
	matchTerminal(Token::PERIOD, tokens);
}
void Parser::factList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "ID") {
		fact(tokens);
		factList(tokens);
	}
}
void Parser::rule(vector<Token>& tokens) {
	currentPred.clear();
	headPredicate(tokens);
	Predicate currentName = currentPred[0];
	currentPred.clear();
	matchTerminal(Token::COLON_DASH, tokens);
	predicate(tokens);
	predicateList(tokens);
	matchTerminal(Token::PERIOD, tokens);
	rules.push_back(Rule(currentName, currentPred));
}
void Parser::ruleList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "ID") {
		rule(tokens);
		ruleList(tokens);
	}
}
void Parser::query(vector<Token>& tokens) {
	currentPred.clear();
	predicate(tokens);
	if (currentPred.empty()) {
		error();
	}
	queries.push_back(currentPred[0]);
	matchTerminal(Token::Q_MARK, tokens);
}
void Parser::queryList(vector<Token>& tokens) {
	if (tokens[currentToken].getType() == "ID") {
		query(tokens);
		queryList(tokens);
	}
}
void Parser::parse(vector<Token> tokens) {
	matchTerminal(Token::SCHEMES, tokens);
	matchTerminal(Token::COLON, tokens);
	scheme(tokens);
	schemeList(tokens);
	matchTerminal(Token::FACTS, tokens);
	matchTerminal(Token::COLON, tokens);
	factList(tokens);
	matchTerminal(Token::RULES, tokens);
	matchTerminal(Token::COLON, tokens);
	ruleList(tokens);
	matchTerminal(Token::QUERIES, tokens);
	matchTerminal(Token::COLON, tokens);
	query(tokens);
	queryList(tokens);
	matchTerminal(Token::ENDFILE, tokens);
}
vector<Predicate> Parser::getSchemes() { return schemes; }
vector<Predicate> Parser::getFacts() { return facts; }
vector<Rule> Parser::getRules() { return rules; }
vector<Predicate> Parser::getQueries() { return queries; }
string Parser::toString() {
	int sizeSchemes = schemes.size();
	int sizeFacts = facts.size();
	int sizeRules = rules.size();
	int sizeQueries = queries.size();
	int sizeDomain = domain.size();
	stringstream ss;

	ss << "Success!" << endl;
	ss << "Schemes(" << sizeSchemes << "):" << endl;
	for (int i = 0; i < sizeSchemes; i++) {
		ss << "  " << schemes.at(i).toString() << endl;
	}

	ss << "Facts(" << sizeFacts << "):" << endl;
	for (int i = 0; i < sizeFacts; i++) {
		ss << "  " << facts.at(i).toString() << "." << endl;
	}

	ss << "Rules(" << sizeRules << "):" << endl;
	for (int i = 0; i < sizeRules; i++) {
		ss << "  " << rules.at(i).toString() << "." << endl;
	}

	ss << "Queries(" << sizeQueries << "):" << endl;
	for (int i = 0; i < sizeQueries; i++) {
		ss << "  " << queries.at(i).toString() << "?" << endl;
	}

	ss << "Domain(" << sizeDomain << "):" << endl;
	for (std::set<string>::iterator it = domain.begin(); it != domain.end(); ++it) {
		ss << "  " << *it << endl;
	}

	return ss.str();
}

#include "token.h"

const string Token::TOKEN_TYPE[18] = { "COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN", "RIGHT_PAREN", "COLON", "COLON_DASH", 
"MULTIPLY","ADD", "SCHEMES", "FACTS", "RULES", "QUERIES", "ID", "STRING", "COMMENT", "UNDEFINED", "EOF" };

const string Token::TOKEN_NAME[18] = { ",", ".", "?", "(", ")", ":", ":-", "*", "+", "Schemes", "Facts", "Rules", 
"Queries", "ID", "STRING", "COMMENT", "UNDEFINED", "" };

Token::Token() {
	this->line = 1;
}

Token::Token(string name, string value, int current_line) {
	this->type = name;
	this->value = value;
	this->line = current_line;
}

string Token::getType() {
	return type;
}

string Token::getValue() {
	return value;
}

string Token::toString() {
	stringstream ss;
	ss << "(" << type << "," << "\"" << value << "\"," << line << ")";
	return ss.str();
}

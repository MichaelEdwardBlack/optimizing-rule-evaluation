#include "lexer.h"

void Lexer::addToken(string TOKEN_TYPE, string value, int line) {
	Token newToken(TOKEN_TYPE, value, line);
	listTokens.push_back(newToken);
	numTokens++;
}

void Lexer::colonCase(ifstream& inputFile) {
	if (inputFile.peek() == '-') {
		input = inputFile.get();
		addToken(Token::TOKEN_TYPE[Token::COLON_DASH], Token::TOKEN_NAME[Token::COLON_DASH], numLine);
	}
	else {
		addToken(Token::TOKEN_TYPE[Token::COLON], Token::TOKEN_NAME[Token::COLON], numLine);
	}
}

void Lexer::stringCase(ifstream& inputFile) {
	int startLine = numLine; // in case this is a multi-line string we only want the line number where the string starts
	stringstream newString;
	newString << input;
	while (inputFile.peek() != EOF) {
		input = inputFile.get();
		if (input == '\'') { // check for end of comment
			if (inputFile.peek() != '\'') { // check if end of comment symbol was just an escape key
				newString << input;
				addToken(Token::TOKEN_TYPE[Token::STRING], newString.str(), startLine);
				return;
			}
			else {
				newString << input;
				input = inputFile.get();
				newString << input;
			}
		}
		else {
			if (input == '\n') {
				numLine++;
			}
			newString << input;
		}
	}
	error(newString.str(), startLine); // string never ended; throw the never ending string
}

void Lexer::commentCase(ifstream& inputFile) {
	stringstream comment;
	comment << input;
	if (inputFile.peek() == '|') { // check if multi-line comment
		multiCommentCase(inputFile); // go to the function to handle multi-line comments
	}
	else {
		while (inputFile.peek() != '\n' && !inputFile.eof()) {
			input = inputFile.get();
			comment << input;
		}
		//addToken(Token::TOKEN_TYPE[Token::COMMENT], comment.str(), numLine);
	}
}

void Lexer::multiCommentCase(ifstream& inputFile) {
	int startLine = numLine; // in case this is a multi-line comment we only want the line number where the comment starts
	stringstream comment;
	comment << input;
	while (inputFile.peek() != EOF) {
		input = inputFile.get();
		if (input == '|' && inputFile.peek() == '#') {
			comment << input;
			input = inputFile.get();
			comment << input;
			//addToken(Token::TOKEN_TYPE[Token::COMMENT], comment.str(), startLine);
			return;
		}
		comment << input;
		if (input == '\n') { numLine++; }
		if (inputFile.peek() == EOF) {
			error(comment.str(), startLine);
			return;
		}
	}
}

void Lexer::defaultCase(ifstream& inputFile) {
	if (isalpha(input)) {
		stringstream newString;
		newString << input;
		while (isalnum(inputFile.peek())) {
			input = inputFile.get();
			newString << input;
		}
		// Check if the string is a query, rule etc. or just a ID
		checkKeyWord(newString.str());

	}
	else if (isspace(input)) {}
	else {
		stringstream err;
		err << input;
		error(err.str(), numLine);
	}
}

void Lexer::checkKeyWord(string inputString) {
	if (inputString == "Schemes") {
		addToken(Token::TOKEN_TYPE[Token::SCHEMES], Token::TOKEN_NAME[Token::SCHEMES], numLine);
	}
	else if (inputString == "Facts") {
		addToken(Token::TOKEN_TYPE[Token::FACTS], Token::TOKEN_NAME[Token::FACTS], numLine);
	}
	else if (inputString == "Rules") {
		addToken(Token::TOKEN_TYPE[Token::RULES], Token::TOKEN_NAME[Token::RULES], numLine);
	}
	else if (inputString == "Queries") {
		addToken(Token::TOKEN_TYPE[Token::QUERIES], Token::TOKEN_NAME[Token::QUERIES], numLine);
	}
	else {
		addToken(Token::TOKEN_TYPE[Token::ID], inputString, numLine);
	}
}

void Lexer::scan(ifstream& inputFile) {
	while (inputFile.peek() != EOF) {
		input = inputFile.get();
		switch (input) {
		case ',':
			addToken(Token::TOKEN_TYPE[Token::COMMA], Token::TOKEN_NAME[Token::COMMA], numLine);
			break;
		case '.':
			addToken(Token::TOKEN_TYPE[Token::PERIOD], Token::TOKEN_NAME[Token::PERIOD], numLine);
			break;
		case '?':
			addToken(Token::TOKEN_TYPE[Token::Q_MARK], Token::TOKEN_NAME[Token::Q_MARK], numLine);
			break;
		case '(':
			addToken(Token::TOKEN_TYPE[Token::LEFT_PAREN], Token::TOKEN_NAME[Token::LEFT_PAREN], numLine);
			break;
		case ')':
			addToken(Token::TOKEN_TYPE[Token::RIGHT_PAREN], Token::TOKEN_NAME[Token::RIGHT_PAREN], numLine);
			break;
		case '*':
			addToken(Token::TOKEN_TYPE[Token::MULTIPLY], Token::TOKEN_NAME[Token::MULTIPLY], numLine);
			break;
		case '+':
			addToken(Token::TOKEN_TYPE[Token::ADD], Token::TOKEN_NAME[Token::ADD], numLine);
			break;
		case '\n':
			numLine++;
			break;
		case ':':
			colonCase(inputFile);
			break;
		case '\'':
			stringCase(inputFile);
			break;
		case '#':
			commentCase(inputFile);
			break;
		default:
			defaultCase(inputFile);
			break;
		}
	}
	addToken(Token::TOKEN_TYPE[Token::ENDFILE], Token::TOKEN_NAME[Token::ENDFILE], numLine);
}

void Lexer::error(string sError, int line) {
	addToken(Token::TOKEN_TYPE[Token::UNDEFINED], sError, line);
}

int Lexer::getNumTokens() {
	return numTokens;
}

vector<Token> Lexer::getTokens() {
	return listTokens;
}

string Lexer::printTokens() {
	stringstream ss;
	for (int i = 0; i < numTokens; i++) {
		ss << listTokens.at(i).toString() << endl;
	}
	return ss.str();
}

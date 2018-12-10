#include <fstream>
#include <sstream>
#include <vector>
#include "token.h"

class Lexer {
public:
	Lexer() { numLine = 1; numTokens = 0; }
	void addToken(std::string name, std::string value, int line);
	void colonCase(ifstream& inputFile);
	void stringCase(ifstream& inputFile);
	void multiCommentCase(ifstream& inputFile);
	void commentCase(ifstream& inputFile);
	void checkKeyWord(string inputString);
	void defaultCase(ifstream& inputFile);
	void scan(ifstream& inputFile);
	void error(string sError, int line);
	string printTokens();
	int getNumTokens();
	vector<Token> getTokens();
private:
	int numLine;
	int numTokens;
	char input;
	vector<Token> listTokens;
};

#include <string>
#include <sstream>

using namespace std;

class Token {
public:
	enum TOKEN_NUM {
		COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY,
		ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, ENDFILE
	};
	static const string TOKEN_TYPE[18];
	static const string TOKEN_NAME[18];
	Token();
	Token(string name, string val, int current_line);
	string getType();
	string getValue();
	string toString();
private:
	string type;
	string value;
	int line;
};

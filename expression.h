#include <string>
#include <sstream>

using namespace std;

class Expression {
public:
	Expression();
	Expression(string param, string op, string param2);
	string toString();
private:
	string parameter;
	string theOperator;
	string parameter2;
};
#include <string>

using namespace std;

class Parameter {
public:
	Parameter();
	Parameter(bool lit, string n, string val);
	string getValue();
	bool isLiteral();
	string toString();
private:
	bool literal;
	string name;
	string value;
};

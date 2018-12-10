#include "parameter.h"
#include <sstream>
#include <vector>

using namespace std;

class Predicate {
public:
	Predicate();
	Predicate(string n, vector<Parameter> params);
	string toString();
	string getName();
	vector<string> getParameters();
	vector<Parameter> getParameterVector();
private:
	string name;
	vector<Parameter> parameters;
};

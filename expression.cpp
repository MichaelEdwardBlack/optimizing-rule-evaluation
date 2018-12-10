#include "expression.h"

Expression::Expression() {
	parameter = "";
	theOperator = "";
	parameter2 = "";
}

Expression::Expression(string param, string op, string param2) {
	parameter = param;
	theOperator = op;
	parameter2 = param2;
}

string Expression::toString() {
	stringstream ss;
	ss << '(' << parameter << theOperator << parameter2 << ')';

	return ss.str();
}

#include "parameter.h"

Parameter::Parameter() {
	literal = false;
}

Parameter::Parameter(bool lit, string n, string val) {
	literal = lit;
	name = n;
	value = val;
}

string Parameter::getValue() {
	return value;
}

bool Parameter::isLiteral() {
	return literal;
}

string Parameter::toString() {
	return value;
}

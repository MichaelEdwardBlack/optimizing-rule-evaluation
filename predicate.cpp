#include "predicate.h"

Predicate::Predicate() {}

Predicate::Predicate(string n, vector<Parameter> params) {
	name = n;
	parameters = params;
}

string Predicate::toString() {
	int size = parameters.size();
	stringstream ss;
	ss << name << "(";
	for (int i = 0; i < size; i++) {
		ss << parameters.at(i).toString();
		if (i < (size - 1)) {
			ss << ",";
		}
	}
	ss << ")";
	return ss.str();
}

string Predicate::getName() {
	return name;
}

vector<string> Predicate::getParameters() {
	int size = parameters.size();
	vector<string> params;
	for (int i = 0; i < size; i++) {
		params.push_back(parameters.at(i).toString());
	}
	return params;
}

vector<Parameter> Predicate::getParameterVector() {
	return parameters;
}

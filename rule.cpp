#include "rule.h"

Rule::Rule() {}

Rule::Rule(Predicate n, vector<Predicate> d) {
	head = n;
	data = d;
}

Predicate Rule::getHead() {
	return head;
}

vector<Predicate> Rule::getData() {
	return this->data;
}

string Rule::toString() {
	int count = 0;
	int sizeData = data.size();
	stringstream ss;

	ss << head.toString() << " :- ";
	for (int i = 0; i < sizeData; i++) {
		ss << data.at(i).toString();
		count++;
		if (count < sizeData) {
			ss << ",";
		}
	}
	return ss.str();
}

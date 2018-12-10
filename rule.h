#include "predicate.h"
#include <vector>

class Rule {
	friend class Graph;
public:
	Rule();
	Rule(Predicate n, vector<Predicate> d);
	Predicate getHead();
	vector<Predicate> getData();
	string toString();
private:
	Predicate head;
	vector<Predicate> data;
};

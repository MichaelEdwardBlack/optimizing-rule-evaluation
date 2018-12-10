#include <set>
#include "schema.h"
#include "tuple.h"

using namespace std;

class Relation {
public:
	Relation();
  void addColumns(Schema s);
  void addRow(Tuple t);
	void addRows(set<Tuple> tupleSet);
  Relation selectColumnValue(int columnPosition, string value);
  Relation selectColumnColumn(int column1, int column2);
	Relation project(vector<int> indexes);
	Relation rename(int columnPosition, string columnName);
	Relation join(Relation t);
	Relation unionTable(Relation t);
	int match(vector<string> s, string columnName);
	void clear();
	Schema getColumns();
  set<Tuple> getRows();
	int getColumnIndexOf(string columnName);
	int getNumRows();
	void setName(string n);
	string getName();
	string printColumns();
  string printRows();
private:
	string name;
	Schema columns;
  set<Tuple> rows;
  int numRows;
};

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Schema {
public:
	Schema();
  void addAttribute(string a);
	void addAttributes(Schema columns);
	void changeAttribute(int i, string v);
  int getIndexOf(string s);
	int size();
  string at(int i);
	void clear();
  string toString();
private:
	vector<string> schema;
  int numAttributes;
};

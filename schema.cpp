#include "schema.h"

Schema::Schema() { numAttributes = 0; }

void Schema::addAttribute(string a) {
  schema.push_back(a);
  numAttributes++;
}

void Schema::addAttributes(Schema columns) {
  int numColumns = columns.size();
  this->numAttributes += numColumns;
  for (int i = 0; i < numColumns; i++) {
    this->schema.push_back(columns.at(i));
  }
}

void Schema::changeAttribute(int i, string v) {
  schema.at(i) = v;
}

int Schema::getIndexOf(string s) {
  for (int i = 0; i < numAttributes; i++) {
    if (schema.at(i) == s) {
      return i;
    }
  }
  return -1; // error
}

int Schema::size() {
  return numAttributes;
}

string Schema::at(int i) {
  return schema.at(i);
}

void Schema::clear() {
  schema.clear();
  numAttributes = 0;
}

string Schema::toString() {
  std::stringstream ss;
  int size = schema.size();
  for (int i = 0; i < size; i++) {
    ss << schema.at(i) << ",";
  }
  return ss.str();
}

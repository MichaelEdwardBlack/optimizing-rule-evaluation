#include "relation.h"

Relation::Relation() { numRows = 0; name = ""; }

void Relation::addColumns(Schema s) {
  columns = s;
}

void Relation::addRow(Tuple t) {
  rows.insert(t);
  numRows++;
}

void Relation::addRows(set<Tuple> tupleSet) {
  rows = tupleSet;
  numRows = tupleSet.size();
}

Relation Relation::selectColumnValue(int columnPosition, string value) {
  Relation newTable;
  newTable.setName(this->name);
  newTable.addColumns(this->columns);
  for (set<Tuple>::iterator it = rows.begin(); it != rows.end(); ++it) {
    if (it->at(columnPosition) == value) {
      newTable.addRow(*it);
    }
  }
  return newTable;
}

Relation Relation::selectColumnColumn(int column1, int column2) {
  Relation newTable;
  newTable.setName(this->name);
  newTable.addColumns(this->columns);
  for (set<Tuple>::iterator it = rows.begin(); it != rows.end(); ++it) {
      if (it->at(column1) == it->at(column2)) {
        newTable.addRow(*it);
      }
  }
  return newTable;
}

Relation Relation::project(vector<int> indexes) {
  Relation newTable;
  newTable.setName(this->name);
  Schema newColumns;
  Tuple newRow;
  int numColumns = indexes.size();
  // Make new headers for the columns
  for (int i = 0; i < numColumns; i++) {
    newColumns.addAttribute(this->columns.at(indexes.at(i)));
  }
  newTable.addColumns(newColumns);
  // Add rows to the new table
  for (set<Tuple>::iterator it = rows.begin(); it != rows.end(); ++it) {
    newRow.clear();
    for (int i = 0; i < numColumns; i++) {
      newRow.push_back(it->at(indexes.at(i)));
    }
    newTable.addRow(newRow);
  }

  return newTable;
}

Relation Relation::rename(int columnPosition, string columnName) {
  Relation tempTable;
  Schema tempColumns = this->columns;
  tempColumns.changeAttribute(columnPosition, columnName);
  tempTable.setName(this->name);
  tempTable.addColumns(tempColumns);
  tempTable.addRows(this->rows);
  return tempTable;
}

Relation Relation::join(Relation t) {
  Relation tempTable;
  Schema newHeader;
  Schema oldColumns = this->columns;
  int numOldColumns = oldColumns.size();
  Schema newColumns = t.getColumns();
  int numNewColumns = newColumns.size();
  vector<int> projectPositions;
  vector<int> oldSchemaMatches;
  vector<int> newSchemaMatches;

  for (int i = 0; i < numOldColumns; i++) {
    projectPositions.push_back(i);
    for (int j = 0; j < numNewColumns; j++) {
      if (oldColumns.at(i) == newColumns.at(j)) {
        oldSchemaMatches.push_back(i);
        newSchemaMatches.push_back(j);
      }
      else {
        projectPositions.push_back(numOldColumns + j);
      }
    }
  }
  oldColumns.addAttributes(newColumns);
  tempTable.addColumns(oldColumns);

  int numMatches = oldSchemaMatches.size();
  if (numMatches == 0) {
    set<Tuple> newRows = t.getRows();
    Tuple tempRow;
    Tuple tempRowAppend;

    for (set<Tuple>::iterator oldIt = this->rows.begin(); oldIt != this->rows.end(); ++oldIt) {
      for (set<Tuple>::iterator newIt = newRows.begin(); newIt != newRows.end(); ++newIt) {
        tempRow = *oldIt;
        tempRowAppend = *newIt;
        tempRow.insert(tempRow.end(), tempRowAppend.begin(), tempRowAppend.end());
        tempTable.addRow(tempRow);
      }
    }
    return tempTable;
  }

  else {
    set<Tuple> newRows = t.getRows();
    Tuple tempRow;
    Tuple tempRowAppend;
    bool fullMatch;
    for (set<Tuple>::iterator oldIt = this->rows.begin(); oldIt != this->rows.end(); ++oldIt) {
      for (set<Tuple>::iterator newIt = newRows.begin(); newIt != newRows.end(); ++newIt) {
        fullMatch = true;
        for (int i = 0; i < numMatches; i ++) {
          if (oldIt->at(oldSchemaMatches.at(i)) != newIt->at(newSchemaMatches.at(i))) {
            fullMatch = false;
            break;
          }
        }
        if (fullMatch) {
          tempRow = *oldIt;
          tempRowAppend = *newIt;
          tempRow.insert(tempRow.end(), tempRowAppend.begin(), tempRowAppend.end());
          tempTable.addRow(tempRow);
        }
      }
    }
    tempTable = tempTable.project(projectPositions);
    return tempTable;
  }
}

Relation Relation::unionTable(Relation t) {
  Relation tempTable;
  tempTable.addColumns(this->columns);
  tempTable.addRows(this->rows);
  set<Tuple> newRows = t.getRows();
  for (set<Tuple>::iterator it = newRows.begin(); it != newRows.end(); ++it) {
    tempTable.addRow(*it);
  }
  return tempTable;
}

int Relation::match(vector<string> s, string columnName) {
  int schemaSize = s.size();
  for (int i = 0; i < schemaSize; i++) {
    if (s.at(i) == columnName) {
      return i;
    }
  }
  return -1;
}

void Relation::clear() {
  columns.clear();
  rows.clear();
  numRows = 0;
  name = "";
}

Schema Relation::getColumns() {
  return columns;
}

set<Tuple> Relation::getRows() {
  return this->rows;
}

int Relation::getColumnIndexOf(string columnName) {
  return this->columns.getIndexOf(columnName);
}

int Relation::getNumRows() {
  return this->rows.size();
}

void Relation::setName(string n) {
  this->name = n;
}

string Relation::getName() {
  return this->name;
}

string Relation::printColumns() {
    std::stringstream ss;
    ss << columns.toString();
    return ss.str();
}

string Relation::printRows() {
  std::stringstream ss;
  //problem here!!!
  int numColumns = this->columns.size();

  for (set<Tuple>::iterator it = this->rows.begin(); it != this->rows.end(); ++it) {
      ss << "  ";
      for (int i = 0; i < numColumns; i++) {
        ss << this->columns.at(i) << "=" << it->at(i);
        if (i < numColumns - 1) {
          ss << ", ";
        }
      }
      ss << "\n";
  }

  return ss.str();
}

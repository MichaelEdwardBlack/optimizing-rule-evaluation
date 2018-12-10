#include "database.h"

Database::Database() { matchIndex = 0; numRuleEvaluations = 0; changes = false; }

void Database::doSchemes(vector<Predicate> schemes) {
  Relation tempTable;
  string tableName;
  Schema tempHeaders;
  int numSchemes = schemes.size();
  int numAttributes;
  vector<string> attributes;
  for (int i = 0; i < numSchemes; i++) {
    tempTable.clear();
    tempHeaders.clear();
    tableName = schemes.at(i).getName();
    tempTable.setName(tableName);
    attributes = schemes.at(i).getParameters();
    numAttributes = attributes.size();
    for (int j = 0; j < numAttributes; j++) {
      tempHeaders.addAttribute(attributes.at(j));
    }
    tempTable.addColumns(tempHeaders);
    this->tables.insert({tableName, tempTable});
  }
}

void Database::doFacts(vector<Predicate> facts) {
  string tableName;
  Tuple row;
  vector<string> tupleValues;
  int numTupleValues;
  int numFacts = facts.size();
  for (int i = 0; i < numFacts; i++) {
    row.clear();
    tupleValues.clear();
    tableName = facts.at(i).getName();
    tupleValues = facts.at(i).getParameters();
    numTupleValues = tupleValues.size();
    for (int j = 0; j < numTupleValues; j++) {
      row.push_back(tupleValues.at(j));
    }
    this->tables[tableName].addRow(row);
  }
}

void Database::doRules(vector<Rule> rules) {
  int numRowsBefore;
  int numRowsAfter;
  Relation tempTable;
  this->rules = rules;
  int numRules = rules.size();
  Predicate head;
  string headName;
  vector<string> headParameters;
  int numHeadParameters;
  vector<int> matchingColumns;
  vector<Predicate> predicates;
  int numPredicates;

  do {
    this->changes = false;
    this->numRuleEvaluations++;
    for (int i = 0; i < numRules; i++) {
      tempTable.clear();
      matchingColumns.clear();
      head = rules.at(i).getHead();
      headName = head.getName();
      headParameters = head.getParameters();
      numHeadParameters = headParameters.size();
      predicates = rules.at(i).getData();
      numPredicates = predicates.size();

      doQueries(predicates);
      tempTable = this->queryResults.at(0);
      for (int j = 1; j < numPredicates; j++) {
        tempTable = tempTable.join(this->queryResults.at(j));
      }
      for (int j = 0; j < numHeadParameters; j++) {
        matchingColumns.push_back(tempTable.getColumnIndexOf(headParameters.at(j)));
      }
      tempTable = tempTable.project(matchingColumns);
      tempTable.addColumns(this->tables[headName].getColumns());
      numRowsBefore = this->tables[headName].getNumRows();
      this->tables[headName] = this->tables[headName].unionTable(tempTable);
      numRowsAfter = this->tables[headName].getNumRows();
      if (numRowsAfter > numRowsBefore) {
        this->changes = true;
      }
    }
  } while (this->changes);
}

void Database::doQueries(vector<Predicate> queries) {
  this->queryResults.clear();
  this->queries.clear();
  this->queries = queries;
  Relation tempTable;
  string tableName;
  vector<Parameter> params;
  vector<int> projectPositions;
  //map of something to help with columncolumn select
  int numParameters;
  int numQueries = queries.size();

  for (int i = 0; i < numQueries; i++) {
    tempTable.clear();
    params.clear();
    projectPositions.clear();
    tableName = queries.at(i).getName();
    tempTable = tables[tableName];
    params = queries.at(i).getParameterVector();
    numParameters = params.size();
    this->parameterCheck.clear();
    this->matchIndex = 0;
    for (int j = 0; j < numParameters; j++) {
      if (params.at(j).isLiteral()) {
        tempTable = tempTable.selectColumnValue(j, params.at(j).getValue());
      }
      else if (match(params.at(j).getValue())) {
        tempTable = tempTable.selectColumnColumn(j, this->matchIndex);
      }
      else {
        projectPositions.push_back(j);
      }
      tempTable = tempTable.rename(j, params.at(j).getValue());
    }
    tempTable = tempTable.project(projectPositions);
    this->queryResults.push_back(tempTable);
  }
}

bool Database::match(string parameter) {
  int numParameters = this->parameterCheck.size();
  for (int i = 0; i < numParameters; i++) {
    if (parameterCheck.at(i) == parameter) {
      this->matchIndex = i;
      return true;
    }
  }
  parameterCheck.push_back(parameter);
  return false;
}

string Database::printRuleResults() {
  stringstream result;
  result << "Schemes populated after " << this->numRuleEvaluations
    << " passes through the Rules.\n\n";
  return result.str();
}

string Database::printQueryResults() {
  stringstream result;
  int numQueries = this->queryResults.size();
  for (int i = 0; i < numQueries; i++) {
    if (this->queryResults.at(i).getNumRows() == 0) {
      result << this->queries.at(i).toString() << "? No\n";
    }
    else {
      result << this->queries.at(i).toString() << "? Yes(" << this->queryResults.at(i).getNumRows() << ")\n";
      if (this->queryResults.at(i).getColumns().size() != 0) {
        result << this->queryResults.at(i).printRows();
      }
    }
  }
  return result.str();
}

string Database::printTables() {
  stringstream ss;
  for (map<string, Relation>::iterator it = this->tables.begin(); it != this->tables.end(); it++) {
    ss << it->first << "\n";
    ss << it->second.printColumns() << "\n";
    ss << it->second.printRows() << "\n";
  }
  return ss.str();
}

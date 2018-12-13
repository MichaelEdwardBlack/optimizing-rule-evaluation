#include "node.h"

Node::Node() {
  post_order_number = -1;
  visited = false;
}

void Node::wasVisited() {
  this->visited = true;
}

void Node::assignPostOrderNumber(int number) {
  this->post_order_number = number;
}

void Node::insertAdjacentNodes(int rule_index) {
  dependencies.insert(rule_index);
}

string Node::printAdjacentNodes() {
  stringstream ss;
  int count = 0;
  for(auto adjacent_node : dependencies) {
    count++;
    if (count > 1) {
      ss << ",";
    }
    ss << "R" << adjacent_node;
  }
  return ss.str();
}

set<int> Node::getAdjacentNodes() {
  return this->dependencies;
}

Node::~Node() {
  dependencies.clear();
}

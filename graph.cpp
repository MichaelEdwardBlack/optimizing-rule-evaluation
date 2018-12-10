#include "graph.h"

Graph::Graph(vector<Rule>& r) {
  numPostOrder = 1;
  this->rules = r;
  this->makeGraphs();
  this->dfsForest();
  this->findScc();
}

void Graph::addDependencies(int rule_index) {
  Rule dependency_rule = rules.at(rule_index);
  int rule_dependent_index = 0;

  for(auto depend_rule : rules) {
    for(auto predicate : depend_rule.data) {
      if(dependency_rule.head.getName() == predicate.getName()) {
        graph.at(rule_dependent_index).insertAdjacentNodes(rule_index);
        reverse_graph.at(rule_index).insertAdjacentNodes(rule_dependent_index);
      }
    }
    rule_dependent_index++;
  }
}

void Graph::makeGraphs() {
  int numRules = rules.size();
  for(int i = 0; i < numRules; ++i) {
    Node forward;
    Node reverse;
    graph.insert(pair<int, Node> (i, forward));
    reverse_graph.insert(pair<int, Node> (i, reverse));
  }
  int rule_index = 0;
  for(auto rule : rules) {
    addDependencies(rule_index);
    rule_index++;
  }
}

void Graph::dfs(int node_index) {
  Node& node = reverse_graph.at(node_index);
  node.wasVisited();
  set<int> adjacent_nodes = node.getAdjacentNodes();

  for(auto set_index : adjacent_nodes) {
    if(reverse_graph.at(set_index).visited == false)  {
      dfs(set_index);
    }
  }
  node.assignPostOrderNumber(numPostOrder);
  numPostOrder++;
}

void Graph::dfsForest() {
  for(auto node : reverse_graph) {
    if(node.second.visited == false) {
      dfs(node.first);
    }
  }
}

vector<int> Graph::getHighestToLowestPON() {
  vector<int> highestToLowest;

  for(int i = numPostOrder; i > 0; --i) {
    for(auto reverse : reverse_graph) {
      if(i == reverse.second.post_order_number) {
        highestToLowest.push_back(reverse.first);
      }
    }
  }
  return highestToLowest;
}

void Graph::sccInDfs(int index, set<int> &scc) {
  Node& node = graph.at(index);
  node.wasVisited();
  set<int> adjacent_nodes = node.getAdjacentNodes();

  for(auto set_index : adjacent_nodes) {
    if(graph.at(set_index).visited == false) {
      sccInDfs(set_index, scc);
    }
  }
  scc.insert(index);
}

void Graph::findScc() {
  vector<int> highToLowestRules = getHighestToLowestPON();
  int numHighToLowestRules = highToLowestRules.size();
  for(int i = 0; i < numHighToLowestRules; ++i) {
    set<int> scc_set;
    if(graph.at(highToLowestRules[i]).visited == false) {
      sccInDfs(highToLowestRules[i], scc_set);
    }

    scc.insert(pair<int, set<int>> (highToLowestRules[i],scc_set));
  }
}

string Graph::printGraph() {
  stringstream ss;
  ss << "Dependency Graph";
  for(auto foward : graph) {
    ss << "\n  R" << foward.first << ":";
    ss << foward.second.printAdjacentNodes();
  }
  ss << "\n\n";

  return ss.str();
}

string Graph::printReverseGraph() {
  stringstream ss;

  ss << "Reverse Graph";
  for(auto reverse : reverse_graph) {
    ss << "\n  R" << reverse.first << ":";
    ss << reverse.second.printAdjacentNodes();
  }
  ss << "\n\n";

  return ss.str();
}

//======================================================================

// Define the print scc function
string Graph::printScc()
{
  stringstream ss;

  ss << "Postorder Numbers";
  for(auto foward : reverse_graph)
  {
      ss << "\n  R" << foward.first << ": ";
      ss << foward.second.post_order_number;
  }
  ss << "\n\n";

  ss << "SCC Search Order";
  vector<int> search_order = getHighestToLowestPON();
  for(auto search : search_order)
  {
      ss << "\n  R" << search;
  }
  ss << "\n\n";

  return ss.str();
}

//======================================================================

// Define the graph destructor
Graph::~Graph(){}

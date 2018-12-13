#include <vector>
#include <set>
#include <map>
#include "node.h"
#include "parser.h"

class Graph {
  friend class Database;
public:
  Graph();
  Graph(vector<Rule>& r);
  void addDependencies(int);
  void makeGraphs();
  void dfs(int);
  void dfsForest();
  vector<int> getHighestToLowestPON();
  void sccInDfs(int, set<int>&);
  void findScc();
  string printGraph();
  string printReverseGraph();
  string printScc();
  ~Graph();
private:
  vector<Rule> rules;
  map<int, Node> graph;
  map<int, Node> reverse_graph;
  map<int, set<int>> scc;
  int numPostOrder;
};

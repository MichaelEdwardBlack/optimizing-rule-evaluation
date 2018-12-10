#include <string>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

class Node : public vector<string> {
  friend class Graph;
public:
  Node();
  void wasVisited();
  void assignPostOrderNumber(int);
  void insertAdjacentNodes(int);
  string printAdjacentNodes();
  set<int> getAdjacentNodes();
  ~Node();
private:
  int post_order_number;
  bool visited;
  set<int> dependencies;
};

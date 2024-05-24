#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "Graph.h"

using namespace std;

int main() {
  Graph graph("./tests/test_data_airports.csv","./tests/test_data_routes.csv");
  vector<int> v;
  v.push_back(1); v.push_back(2); v.push_back(3);
  graph = graph.subgraph(v);
  graph.traveling_salesman();
  return 0;
}

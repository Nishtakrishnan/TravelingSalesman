#include "Graph.h"

using namespace std;

vector<int> Graph::traveling_salesman() {

    complete_graph();
    vector<int> tsp, odd_nodes;

    Graph mst = Graph(*this);
    vector<pair<int, int>> mst_edges = mst.minimum_spanning_tree();
    mst.clear_edge();
    for (pair<int, int> edge : mst_edges) {
        mst.add_edge(edge.first, edge.second);
    }
    print_graph(mst);

    cout << endl << endl << endl;

    for (auto airport_iter : mst.data) {
        int ID = airport_iter.first;
        Airport &airport = airport_iter.second;
        if (airport.routes.size() % 2 == 1) {
            odd_nodes.push_back(ID);
        }
    }
    Graph mst_odd = mst.subgraph(odd_nodes);
    mst_odd.complete_graph();
    print_graph(mst_odd);
    vector<pair<int, int>> matching = mst_odd.weight_perfect_matching();

    Graph ec = Graph(mst);
    for (pair<int, int> match : matching) {
        int sourceID = match.first;
        int destinationID = match.second;
        if (ec.data[sourceID].routes.find(destinationID) == ec.data[sourceID].routes.end()) {
            ec.data[sourceID].routes[destinationID] = INFINITY_WEIGHT;
            ec.data[destinationID].routes[sourceID] = INFINITY_WEIGHT;
        }
    }
    vector<int> circuit = ec.eulerian_path();
    
    return tsp;
}
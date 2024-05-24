#include "Graph.h"
#include "Dsets.h"

using namespace std;

vector<pair<int, int>> Graph::minimum_spanning_tree() {
    vector<pair<int, int>> mst;
    vector<pair<int, pair<int, int>>> edges;
    DisjointSets nodes;
    for (auto airport_iter : data) {
        int sourceID = airport_iter.first;
        nodes.add_elements(sourceID);
        for (auto route_iter : airport_iter.second.routes) {
            int destinationID = route_iter.first;
            int distance = route_iter.second;
            edges.push_back({distance, {sourceID, destinationID}});
        }
    }
    sort(edges.begin(), edges.end());
    for (auto edge_iter : edges) {
        pair<int, int> edge = edge_iter.second;
        if (nodes.find(edge.first) == nodes.find(edge.second)) continue;
        mst.push_back(edge);
        nodes.set_union(edge.first, edge.second);
        if (mst.size() == nodes.size() - 1) break;
    }
    return mst;
}
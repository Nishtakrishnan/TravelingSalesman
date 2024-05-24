#include "../cs225/catch/catch.hpp"

#include <iostream>
#include "../Graph.h"

Graph graph("./tests/test_data_airports.csv","./tests/test_data_routes.csv");

TEST_CASE("Data Processing: Node Construction") {
    //  valid airport inputs
    for (int i = 1; i < 5; ++i) {
        REQUIRE(graph.data.find(i) != graph.data.end());
    }
    /*
        invalid airport inputs:
        5: string name "NULL"
        6: string name "\N"
        7: placeholder name \N
        8: placeholder latitude/longitude \N
        9: latitude out of range
        10: longitude out of range
        11: csv of incorrect length
    */
    for (int i = 5; i < 12; ++i) {
        REQUIRE(graph.data.find(i) == graph.data.end());
    }
}

TEST_CASE("Data Processing: Route Construction") {
    /*
        valid routes:
        (1, 2): every piece of information available
        (1, 3): missing IATA/ICAO but valid ID
        (1, 4): missing ID but IATA/ICAO valid
    */
    for (int i = 2; i < 5; ++i) {
        REQUIRE(graph.data[1].routes.find(i) != graph.data[1].routes.end());
    }
    /*  invalid route
        (1, 5): node with ID = 5 missing
    */
    REQUIRE(graph.data[1].routes.find(5) == graph.data[1].routes.end());
}

TEST_CASE("Calculate Weight") {
    /*
        distance from node 1 to node 4 is expected to be half of the earth's circumference
        because two nodes are both on equator and on the symmetry of the rotational axis
    */
    REQUIRE(graph.data[1].routes[4] == (int) (PI * EARTH_RADIUS));
}

TEST_CASE("Edge Operation") {
    //  guarantee existence of two-sided edge (1, 2)
    REQUIRE(graph.data[1].routes.find(2) != graph.data[1].routes.end());
    REQUIRE(graph.data[2].routes.find(1) != graph.data[2].routes.end());
    //  test remove edge from 1 to 2
    graph.remove_edge(1, 2);
    REQUIRE(graph.data[1].routes.find(2) == graph.data[1].routes.end());
    REQUIRE(graph.data[2].routes.find(1) != graph.data[2].routes.end());
    //  test no problem remove inexisting edge
    graph.remove_edge(1, 2);
    //  test remove edge from 2 to 1
    graph.remove_edge(2, 1);
    REQUIRE(graph.data[2].routes.find(1) == graph.data[2].routes.end());
    //  test add edge is valid and two sided
    graph.add_edge(1, 2);
    REQUIRE(graph.data[1].routes.find(2) != graph.data[1].routes.end());
    REQUIRE(graph.data[2].routes.find(1) != graph.data[2].routes.end());
}

TEST_CASE("Subgraph") {
    Graph copy = Graph(graph);
    copy.add_edge(2, 3);
    copy.add_edge(2, 4);
    copy.add_edge(3, 4);
    vector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3);
    Graph subgraph = copy.subgraph(v);
    //  guarantee node 4 is not in the graph
    REQUIRE(subgraph.data.find(4) == subgraph.data.end());
    //  guarantee all edges connected with node 4 is removed
    REQUIRE(subgraph.data[1].routes.find(4) == subgraph.data[1].routes.end());
    REQUIRE(subgraph.data[2].routes.find(4) == subgraph.data[2].routes.end());
    REQUIRE(subgraph.data[3].routes.find(4) == subgraph.data[3].routes.end());
}

TEST_CASE("Complete Graph") {
    Graph copy = Graph(graph);
    copy.complete_graph();
    for (int i = 2; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            REQUIRE(copy.data[i].routes[j] == INFINITY_WEIGHT);
            REQUIRE(copy.data[j].routes[i] == INFINITY_WEIGHT);
        }
    }
}

TEST_CASE("DFS on Graph") {
    //  create a copy of graph
    Graph copy = Graph(graph);
    REQUIRE(copy.depth_first_traversal());
    //  specific graph should be inconnected if one edge is removed
    copy.remove_edge(1, 2);
    copy.remove_edge(2, 1);
    REQUIRE(!copy.depth_first_traversal());
}

TEST_CASE("Minimum Spanning Tree on Graph") {
    //  create a complete graph for minimum spanning tree
    Graph copy = Graph(graph);
    copy.add_edge(2, 3);
    copy.add_edge(2, 4);
    copy.add_edge(3, 4);
    //  guarantee graph is connected
    REQUIRE(copy.depth_first_traversal());
    //  create a graph created with minimum spanning tree and is connected
    vector<pair<int, int>> mst = copy.minimum_spanning_tree();
    copy.clear_edge();
    for (pair<int, int> route : mst) {
        copy.add_edge(route.first, route.second);
    }
    REQUIRE(copy.depth_first_traversal());
    //  expect edges to be (1, 2), (2, 3), and (3, 4), because these nodes are all one halp of the equator with same distance on degree
    for (pair<int, int> route : mst) {
        REQUIRE(copy.data[route.first].routes[route.second] == (int) (PI * EARTH_RADIUS / 3));
    }
    //  expect the graph of minimum spanning tree to be inconnected when an edge is removed
    copy.remove_edge(mst[0].first, mst[0].second);
    copy.remove_edge(mst[0].second, mst[0].first);
    REQUIRE(!copy.depth_first_traversal());
}

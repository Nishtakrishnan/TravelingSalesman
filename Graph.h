#pragma once

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <iterator>
#include <unordered_map>
#include <vector>
#include <stack>
#include <cstring>
#include <string>

#ifdef NULL_INPUT
#undef NULL_INPUT
#endif
#define NULL_INPUT "\\N"

#ifdef NULL_INPUT_STRING
#undef NULL_INPUT_STRING
#endif
#define NULL_INPUT_STRING "NULL"

#ifdef NULL_AIRPORT_ID
#undef NULL_AIRPORT_ID
#endif
#define NULL_AIRPORT_ID -1

#ifdef INVALID_POSITION
#undef INVALID_POSITION
#endif
#define INVALID_POSITION 360.0

#ifdef INT_FUNC_FAIL
#undef INT_FUNC_FAIL
#endif
#define INT_FUNC_FAIL -1

#ifdef EARTH_RADIUS
#undef EARTH_RADIUS
#endif
#define EARTH_RADIUS 6378.137

#ifdef PI
#undef PI
#endif
#define PI 3.1415926535898

#ifdef INFINITY_WEIGHT
#undef INFINITY_WEIGHT
#endif
#define INFINITY_WEIGHT INT32_MAX

using namespace std;

class Graph
{
private:

    /**
     * @brief Structure used to store information of a single airport, no information of routes is included
     */
    struct AirportInfo
    {
        int id;
        string name;
        float latitude, longitude;
        AirportInfo() : id(NULL_AIRPORT_ID), name(NULL_INPUT_STRING),
                        latitude(INVALID_POSITION), longitude(INVALID_POSITION)
        { /* nothing to do */
        }
        string str() {
            return to_string(id) + ", " + name + ", " + to_string(latitude) + ", " + to_string(longitude);
        }
    };

    /**
     * @brief Structure used to store information of a single airport, information of routes is included
     */
    struct Airport 
    {
        AirportInfo info;
        
        /**
         * @brief Routes to other airports
         * @param Key   ID of the destination airport
         * @param Value Distance between the airports, calculated with function distance(Airport source, Airport Destination)
         */
        unordered_map<int, int> routes;
    };

    /**
     * @brief Transform degree into radian
     * 
     * @param degree Degree of angle
     * @return Radian
     */
    double radian(float degree) const {
        return degree * PI / 180.0;
    }

    /**
     * @brief Check string contains only letters, numbers, and space
     * 
     * @param str String to be checked
     * @return true if contains only letters, numbers, and space
     * @return false contains anything else than letters, numbers, and space
     */
    bool valid_string(string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        const char *array = str.c_str();
        for (size_t i = 0; i < str.length(); ++i) {
            if ((array[i] >= 'A' && array[i] <= 'Z') || (array[i] >= '0' && array[i] <= '9') || array[i] == ' ') continue;
            return false;
        }
        return str != NULL_INPUT && str != NULL_INPUT_STRING && str != "";
    }

    /**
     * @brief Store a csv string into a vector of string
     * 
     * @param csv CSV string
     * @param vec Vector to be stored
     * @return Length of csv
     * @return INT_FUNC_FAIL if failed
     */
    int segment_csv(string &csv, vector<string> &vec) {
        vec.clear();
        int s = 0;
        unsigned i = 0;
        vec.push_back("");
        while (i < csv.length()) {
            if (csv[i] == '\"') {
                ++i;
                while (csv[i] != '\"') {
                    if (i < csv.length()) {
                        vec[s] += csv[i++];
                    } else {
                        return INT_FUNC_FAIL;
                    }
                }
                ++i;
                continue;
            }
            if (csv[i] == ',') {
                vec.push_back("");
                ++s;
                ++i;
                continue;
            }
            vec[s] += csv[i++];
        }
        return s;
    }

    public : 
    /**
     * @brief Construct an empty Graph object
     */
    Graph();

    /**
     * @brief Construct a new Graph object with airports as nodes and routes as edges
     * 
     * @param airports_file File location of airport-extended.csv
     * @param routes_file   File location of routes.csv
     */
    Graph(const string &airports_file, const string &routes_file);

    /**
     * @brief Read airports and add nodes to the graph
     * 
     * @param airport_file File location of airport-extended.csv
     */
    void read_airports(const string &airport_file);

    /**
     * @brief Process airport string and return corresponding AirportInfo object
     * 
     * @param airport       A single string of airport information
     * @param airportinfo   The AirportInfo object to store the airport information
     * @return true if airport information is valid
     * @return false if airport information is invalid
     */
    bool process_airport(string &airport, AirportInfo &airportinfo);

    /**
     * @brief Read routes and add edges to the graph
     * 
     * @param routes_file File location of routes.csv
     */
    void read_routes(const string &routes_file);

    /**
     * @brief add edges to the graph
     * 
     * @param source        ID of the source airport
     * @param destination   ID of the source airport
     * @return true         Succeeded in adding the edge
     * @return false        Failed to add the edge
     */
    bool add_edge(const int &source, const int &destination);

    /**
     * @brief remove edges from the graph
     * 
     * @param source        ID of the source airport
     * @param destination   ID of the source airport
     * @return true         Succeeded in removing the edge
     * @return false        Failed to remove the edge
     */
    bool remove_edge(const int &source, const int & destination);

    /**
     * @brief Remove all edges from the graph
     * 
     */
    void clear_edge();

    /**
     * @brief Add edges of weight = INFINITY_WEIGHT to make a complete graph
     * 
     */
    void complete_graph();

    /**
     * @brief Create weight perfect matching pairs of graph
     * 
     * @return vector<pair<int, int>> a vector of weight perfect matching pairs
     */
    vector<pair<int, int>> weight_perfect_matching();

    /**
     * @brief Create a subgraph with given airports
     * 
     * @param airports ID of airports to create subgraph
     * @return Graph subgraph
     */
    Graph subgraph(vector<int> airports);

    /**
     * @brief Traverse the graph
     * 
     * @return true  if graph is connected
     * @return false if graph is not all connected
     */
    bool depth_first_traversal();

    /**
     * @brief Recursive traverse the graph
     * 
     * @param node    Nodes to traverse from
     * @param visited Hashtable containing status of traversal
     */
    void depth_first_traversal(int node, unordered_map<int, bool> &visited);

    /**
     * @brief Shortest path from the starting node to all other nodes in the graph
     * 
     * @param start                     ID of the starting node
     * @return pair<int, vector<int>>   Pairs of ID and shortest path, each int in the vector represent a step from the beginning
     */
    unordered_map<int, vector<int>> shortest_path(int start);

    /**
     * @brief Output the minimum spanning tree of the graph
     * 
     * @return vector<pair<int, int>> vector of edges (undirected), that are paired airport IDs
     */
    vector<pair<int, int>> minimum_spanning_tree();

    int is_eulerian_path();

    vector<int> eulerian_path();

    vector<int> traveling_salesman();

    /**
     * @brief Calculate the distance between two points on earth, assuming the earth is a perfect sphere
     * 
     * @param src_lat    Latitude of the source airport
     * @param src_long   Longitude of the source airport
     * @param dst_lat    Latitude of the destination airport
     * @param dst_long   Longitude of the destination airport
     * @return Distance
     */
    int distance(const float src_lat, const float src_long, const float dst_lat, const float dst_long) const;

    void print_graph(Graph &graph) {
        for (auto a : graph.data) {
            cout << "---" << a.first << "---" << endl;
            for (auto r : a.second.routes) {
            cout << r.first << ", " << r.second << endl;
            }
        }
    }

    /**
    void print_graph(Graph graph) {
        for (auto a : graph.data) {
            cout << "---" << a.first << "---" << endl;
            for (auto r : a.second.routes) {
            cout << r.first << ", " << r.second << endl;
            }
        }
    }
     * @brief Destroy the Graph object
     */
    ~Graph();
    
    /**
     * @brief Unordered map that contains all nodes and edges
     * @param Key   ID of the airport
     * @param Value Airport node with corresponding ID and the routes connected to it
     */
    unordered_map<int, Airport> data;

    /**
     * @brief Unordered map that maps every valid IATA or ICAO identifier to number ID
     * @param Key   IATA or ICAO identifier
     * @param Value Corresponding number ID
     */
    unordered_map<string, int> map;
};

#include "Graph.h"

using namespace std;

Graph::Graph()
{
    //nothing to do
}

Graph::~Graph()
{
    //nothing to do
}

Graph::Graph(const string &airports_file, const string &routes_file) {
    read_airports(airports_file);
    read_routes(routes_file);
}

void Graph::read_airports(const string &airport_file) {
    ifstream airports(airport_file);
    if (airports.is_open()) {
        string airportstr;
        while (getline(airports, airportstr)) {
            Airport airport;
            if (!process_airport(airportstr, airport.info)) continue;
            int id = airport.info.id;
            data.insert({id, airport});
        }
    }
}

bool Graph::process_airport(string &airport, Graph::AirportInfo &airportinfo) {
    vector<string> info;
    int s = segment_csv(airport, info);
    //check format correctness and availability of latitude, longitude
    if (s == INT_FUNC_FAIL || s != 13 || info[6] == "" || info[7] == "") {
        return false;
    }
    try {
        airportinfo.id = stoi(info[0]);
        airportinfo.latitude = stof(info[6]);
        airportinfo.longitude = stof(info[7]);
    } catch (const std::invalid_argument& e) {
        return false;
    }
    if (airportinfo.latitude < -90.0 || airportinfo.longitude < -180.0
        || airportinfo.latitude > 90.0 || airportinfo.longitude > 180.0) return false;
    if (!valid_string(info[1])) return false;
    airportinfo.name = info[1];
    if (valid_string(info[4])) map[info[4]] = airportinfo.id;
    if (valid_string(info[5])) map[info[5]] = airportinfo.id;
    return true;
}

void Graph::read_routes(const string &routes_file) {
    ifstream routes(routes_file);
    if (routes.is_open()) {
        string routestr;
        while (getline(routes, routestr)) {
            vector<string> route;
            if (segment_csv(routestr, route) != 8) {
                continue;
            }
            int source, destination;
            try {
                source = stoi(route[3]);
            } catch (const std::invalid_argument& e) {
                auto source_iter = map.find(route[2]);
                if (source_iter == map.end()) continue;
                source = (*source_iter).second;
            }
            try {
                destination = stoi(route[5]);
            } catch (const std::invalid_argument& e) {
                auto destination_iter = map.find(route[4]);
                if (destination_iter == map.end()) continue;
                destination = (*destination_iter).second;
            }
            add_edge(source, destination);
        }
    }
}

bool Graph::add_edge(const int &source, const int &destination) {
    auto src = data.find(source);
    auto dst = data.find(destination);
    if (src == data.end() || dst == data.end()) return false;
    int dist = distance(src->second.info.latitude, src->second.info.longitude, dst->second.info.latitude, dst->second.info.longitude);
    src->second.routes[destination] = dist;
    dst->second.routes[source] = dist;
    return true;
}

bool Graph::remove_edge(const int &source, const int &destination) {
    auto src = data.find(source);
    if (src == data.end()) return false;
    (*src).second.routes.erase(destination);
    return true;
}

void Graph::clear_edge() {
    for (auto &airport_iter : data) {
        airport_iter.second.routes.clear();
    }
}

void Graph::complete_graph() {
    for (auto &airport_iter1 : data) {
        Airport &source = airport_iter1.second;
        for (auto &airport_iter2 : data) {
            int destinationID = airport_iter2.first;
            if (source.info.id == destinationID) continue;
            if (source.routes.find(destinationID) == source.routes.end()) {
                source.routes[destinationID] = INFINITY_WEIGHT;
            }
        }
    }
}

Graph Graph::subgraph(vector<int> airports) {
    Graph subgraph;
    for (int airportID : airports) {
        auto airport_iter = data.find(airportID);
        if (airport_iter != data.end()) {
            subgraph.data.insert(*airport_iter);
        } else {
            return Graph();
        }
    }
    for (auto &airport_iter : subgraph.data) {
        int source = airport_iter.first;
        Airport &airport = airport_iter.second;
        vector<int> edges_to_remove;
        for (auto route : airport.routes) {
            int destination = route.first;
            if (subgraph.data.find(destination) == subgraph.data.end()) {
                edges_to_remove.push_back(destination);
            }
        }
        for(int destination : edges_to_remove) {
            subgraph.remove_edge(source, destination);
        }
    }
    return subgraph;
}

bool Graph::depth_first_traversal() {
    if (data.empty()) return true;
    unordered_map<int, bool> visited;
    for (auto airport : data) {
        visited.insert({airport.first, false});
    }
    pair<int, Airport> firstpair = *(data.begin());
    int begin = firstpair.first;
    depth_first_traversal(begin, visited);
    for (auto visit : visited) {
        if (!(visit.second))  {
            return false;
        }
    }
    return true;
}

void Graph::depth_first_traversal(int node, unordered_map<int, bool> &visited) {
    visited[node] = true;
    const unordered_map<int, int> &routes = data[node].routes;
    for (auto route : routes) {
        if (!visited[route.first]) {
            depth_first_traversal(route.first, visited);
        }
    }
}

int Graph::distance(const float src_lat, const float src_long, const float dst_lat, const float dst_long) const {
    double rad_src_lat = radian(src_lat);
    double rad_dst_lat = radian(dst_lat);
    float a = rad_src_lat - rad_dst_lat;
    float b = radian(src_long) - radian(dst_long);
    double s = 2 * asin(sqrt(pow(sin(a/2), 2) + cos(rad_src_lat) * cos(rad_dst_lat) * pow(sin(b/2), 2)));
    s *= EARTH_RADIUS;
    return (int)s;
}


unordered_map<int, vector<int>> Graph::shortest_path(int start) {
    const long long INF = 999999999999;

    unordered_map<int, vector<int>> map;
    for(auto airport_iter : data) {
        map[airport_iter.first].push_back(INF);
        map[airport_iter.first].push_back(-1);
    }
    
    priority_queue<pair<int, int> , vector<pair<int, int>>, greater<pair<int, int>> > pq;
    pq.push(pair<int, int> {0, start});
    map[start].push_back(0);

    while(!pq.empty()) {
        int node = pq.top().second;
        pq.pop();
        
        for (auto route_iter : data[node].routes) {
            int destinationID = route_iter.first;
            int distance = route_iter.second;
            

            if (map[destinationID][0] > map[node][0] + distance) {
                map[destinationID][0] = map[node][0] + distance;
                pq.push(pair<int, int> {map[destinationID][0], destinationID});
                map[destinationID][1] = node;
            }
        }
    }
    return map;
}
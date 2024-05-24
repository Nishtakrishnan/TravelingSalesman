#include "Graph.h"

// finding the eulerian path using hierholzer's algorithm in linear time
/**
 * @param list of airports we need to pass through
 * @return vector of edges that we go through in order
 *
 *
*/
vector<int> Graph::eulerian_path() {

    int store_start;
    int count = 0;
    std::unordered_map<int, std::vector<int>> dests;
    for (auto airport_iter : data) {
        int sourceID = airport_iter.first;
        if(count == 0) {
            store_start = sourceID;
            count++;
        }

        for (auto route_iter : airport_iter.second.routes) {
            int destinationID = route_iter.first;
            dests[sourceID].push_back(destinationID);
        }
    }
    vector<pair<int, pair<int, int>>> edges;
    if(is_eulerian_path() == -1) {
        return vector<int>();
    }
    //storing the temporary path 
    stack<int> temp_path;
    //actual eulerian path
    stack<int> euler_path;
    int start_vertex;
    if (is_eulerian_path() == 0) {

        start_vertex = store_start;

    } else {
        for (auto airport_iter : data) {
            int odd_count = 0;
            for (auto route_iter : airport_iter.second.routes) {
                odd_count++;
            }
            if (odd_count % 2) {
                start_vertex = airport_iter.first;
            }
        }
    }
    temp_path.push(start_vertex);
    while (!temp_path.empty()) {
        int temp = temp_path.top();

        if (dests[temp].size() == 0) {
            euler_path.push(temp);

            temp_path.pop();
        } else {
            temp_path.push(dests[temp].at(0));
            int destID = dests[temp].at(0);
            dests[temp].erase(dests[temp].begin(), dests[temp].begin() + 1);
            dests[destID].erase(std::find(dests[destID].begin(), dests[destID].end(), temp), std::find(dests[destID].begin(), dests[destID].end(), temp)+ 1);   
        }
    }
    vector<int> return_path;

    while (!euler_path.empty()) {
        return_path.push_back(euler_path.top());
        std::cout<<euler_path.top()<<" ";
        euler_path.pop();
    }
    return return_path;
}



int Graph::is_eulerian_path() {
  
    int odd_vert_count = 0;
    for (auto airport_iter : data) {
        int odd_count = 0;
        for (auto route_iter : airport_iter.second.routes) {
            odd_count++;
        }        if (odd_count % 2 == 1) {
            odd_vert_count++;
        }
    }
    
    if (odd_vert_count == 0 || odd_vert_count == 2) {
        return odd_vert_count;
    }
    
    return -1;
}
#pragma once
#include <map>

using namespace std;

class DisjointSets {
    public:
    void add_elements(int id);
    int find(int id);
    void set_union(int a, int b);
    size_t size();
    int size(int id);
    private:
    map<int, int> data;
    size_t elements;
};
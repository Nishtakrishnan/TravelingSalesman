#include "Dsets.h"

void DisjointSets::add_elements(int id) {
    if (data.find(id) == data.end()) {
        data[id] = -1;
        elements++;
    }
}

int DisjointSets::find(int id) {
    if (data.find(id) != data.end()) {
        if (data[id] < 0) return id;
        return find(data[id]);
    }
    return -1;
}

void DisjointSets::set_union(int a, int b) {
    if (data.find(a) == data.end() || data.find(b) == data.end()) return;
    int repA = find(a);
    int repB = find(b);
    if (repA == repB) return;
    if (size(repA) < size(repB)) {
        data[repB] += data[repA];
        data[repA] = repB;
    } else {
        data[repA] += data[repB];
        data[repB] = repA;
    }
}

size_t DisjointSets::size() {
    return elements;
}

int DisjointSets::size(int id) {
    if (data.find(id) != data.end()) return -(data[find(id)]);
    return 0;
}
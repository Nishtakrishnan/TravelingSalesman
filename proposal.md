Final Project Proposal (kdileep2, nidhinc2, nishtak2, shiming3) 

To solve: Create general search tool that finds the shortest path/circuit from the start through all these points and back to the start.

- We will be using the OpenFlights dataset.  
- We will download the airports and routes csv files from https://openflights.org/data.html.  
- We will directly extract data from the csv files.
- We will keep the columns that includes the data described in the graph definition (see below).

Graph

- Nodes are the airports.
- Edges are the connections between two location.
- Weights of edges are distances between two places on earth. We will use physical distances on a sphere with the latitude and 
  longitude of the airport as our weight function input for the edges.
- The graph will be undirected (we assume travel back and forth is the same, and connections are two sided)
- We will use adjacent lists method to store edges.

- unordered_map<int, Airport> airports(Airport ID as key, corresponding airport as value)

struct Airport (single node, from airports and routes)
- AirportInfo info(see below)
- unordered_map<int, int> routes(edges, routes added from routes, with destination Airport ID as key, distance as value)

struct AirportInfo (from airports)
- int id(Airport ID)
- string name, country(Name, Country)
- float latitude, longitude(Latitude, Longitude)

Two graph data algorithms: 

- Minimum Spanning Tree - Kruskal(covered)

  Input - Subgraph with points we want to travel through
  Output - MST

  O(mlog(n)) 
  where m is the number of edges and n is the number of vertices

- Eulerian path(uncovered)
  using Fleury's algorithm

  Input - Subgraph with points we want to travel through
  Output - Eulerian Circuit, if there is one

  O(m^2)
  where m is the number of edges

- Graph traversal: DFS

General Method: create a subgraph that contains all points assigned, use DFS to detect whether the airports are all connected, apply Christofides algorithm(uses both MST and Eulerian path) on the graph.

Timeline:

	Week 1: data acquisition, data processing (by 22nd November)
    
	Week 2, 3: complete DFS traversal and minimum spanning tree (by 6th December)
    
	Week 4: Fleury's algorithm, Christofide algorithm, and test general search (by 11th December)
    
	Remaining: Wrap up project and make final changes (by 11th December)



Helper functions:

1. Graph function, constructor, accepts string file location of aiports.dat and routes.dat, calls function 2 and 4
void function, read airports, accepts string file location of airports.dat, add nodes to the map<string, Airport>, calls function 3, 4 (structure Airport defined below), calls function 3 and 6
2. AirportInfo function, process airport, accepts a single line of string, return a defined macro of empty AirportInfo if not available, return a processed AirportInfo if available.
3. void function, read routes, accepts string file location of routes.dat, calls function 5
4. bool function, add edge, accepts source and destination, returns true if succeed adding an edge, returns false if failed (it can be a void function though, but just make it a bool so that we may have some test cases around, like adding an edge to some non-existing airports, calls function 6 and 7
5. string function, create identifier, probably simply add IATA and ICAO, make it easier while reading the code
6. float, calculate distance

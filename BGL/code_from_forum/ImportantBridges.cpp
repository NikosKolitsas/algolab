#include <climits>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// Graph representation libraries
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore

// Algorithms
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;
using namespace boost;

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

// Directed graph with int weights on edges.
typedef	adjacency_list<vecS, vecS, undirectedS,
	no_property, property<edge_component_t, std::size_t>
	>												Graph;		// Graph (undirected)
typedef	graph_traits<Graph>::edge_descriptor		Edge;		// Edge type
typedef graph_traits<Graph>::edge_iterator			EdgeIt;		// Iterator
typedef graph_traits<Graph>::vertex_descriptor 		Vertex;
typedef	property_map<Graph, edge_component_t>::type	ComponentMap; 

int main() {
	
	// Input settings
	ios_base::sync_with_stdio(false); 
	ifstream cin("ImportantBridges.txt");
	
	// Iterate over test cases
	int T;
	cin >> T;
	while (T--) {
		
		// #Vertices V, #Edges E
		int V, E;
		cin >> V >> E;
		
		// Create graph from edes
		Graph G(V);
		
		// Add edges to graph and weight map
		for (int i = 0; i < E; ++i) {
			
			// Source, target, cost
			int u, v;
			cin >> u >> v;
			u = u - 1;
			v = v - 1;
		
			// Add edge to graph
			Edge e;
			tie(e, tuples::ignore) = add_edge(u, v, G);
			
		}
		
		// Search for biconnected components and articulation point
		ComponentMap component = get(edge_component, G);
		int num_comps = biconnected_components(G, component);
		vector<Vertex> art_points;
		articulation_points(G, std::back_inserter(art_points));
		
		// Number of components
		int cCount[num_comps];
		for (int i = 0; i < num_comps; i++) {
			cCount[i] = 0;
		}
		
		// Count number of edges in each component
		EdgeIt ei, ei_end;
		for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
			//cout << (source(*ei, G) + 1) << "-" << (target(*ei, G) + 1) << " [" << component[*ei] << "]" << endl;
			cCount[component[*ei]]++;
	    }
	    
	    // Count number of bridges
	    int bridges = 0;
		for (int i = 0; i < num_comps; i++) {
			if (cCount[i] == 1) {
				bridges++;
			}
	    }
	    
	    // Output number of bridges found
	    //cout << "No. of bridges: " << bridges << endl;
	    cout << bridges << endl;
	    
	    // Output each bridge
		for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
			if (cCount[component[*ei]] == 1) {
				cout << (source(*ei, G) + 1) << " " << (target(*ei, G) + 1) << endl;
			}
	    }

	}
	
	return 0;

}


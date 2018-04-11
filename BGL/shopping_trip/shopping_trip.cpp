#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef	adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
	property<edge_capacity_t, long,
		property<edge_residual_capacity_t, long,
			property<edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
typedef	property_map<Graph, edge_capacity_t>::type		EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type		ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	graph_traits<Graph>::edge_descriptor			Edge;
typedef graph_traits<Graph>::edge_iterator			EdgeIt;	// Iterator

// Custom EdgeAdder that encapsulates the references to the graph and its
// property maps and always creates reverse edges with corresponding capacities.
//!!!!! modified to convert undirected graph to directed 
//(additing to both directions- both with capacity 1)
struct EdgeAdder {
	EdgeAdder(Graph & G, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge) 
		: G(G), capacity(capacity), rev_edge(rev_edge) {}

	void addEdge(int u, int v, long c) {
		Edge e, reverseE;
		tie(e, tuples::ignore) = add_edge(u, v, G);
		tie(reverseE, tuples::ignore) = add_edge(v, u, G);
		capacity[e] = c;
		capacity[reverseE] = c;	//!!!!edw anti gia 0 bazw c capacity
		rev_edge[e] = reverseE;
		rev_edge[reverseE] = e;
	}
	Graph &G;
	EdgeCapacityMap	&capacity;
	ReverseEdgeMap	&rev_edge;
};

void testcase();

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}

void testcase(){
	int n, m, s;
	cin >> n >> m >> s;

	// Create Graph and Maps
	Graph G(n+1);
	EdgeCapacityMap	capacity = get(edge_capacity, G);
	ReverseEdgeMap	rev_edge = get(edge_reverse, G);
	ResidualCapacityMap	res_capacity = get(edge_residual_capacity, G);
	EdgeAdder ea(G,capacity, rev_edge);

	// Add edges
	int start_vertex = 0;
	int super_sink = n;
	//read the stores locations (which nodes we should visit)
	//connect those nodes with the supersink with 1 capacity
	for(int i=0; i<s; i++){
		int store;
		cin >> store;
		ea.addEdge(store, super_sink, 1);
	}
	//read all the remaining edges of the graph
	for(int i=0; i<m; i++){
		int u, v;
		cin >> u >> v;
		ea.addEdge(u, v, 1);
	}

	// Calculate flow
	long flow = push_relabel_max_flow(G, start_vertex, super_sink);
//	cout << "Maximum Flow with the Push-Relabel Algorihtm: " << flow << endl;
	
	if(flow == s) 	cout << "yes\n";
	else 		cout << "no\n";

}

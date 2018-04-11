#include <climits>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore

using namespace std;
using namespace boost;

// Directed graph with int weights on edges.
typedef	adjacency_list<vecS, vecS, undirectedS,
		no_property,
		property<edge_weight_t, int> >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator
// Map edge -> weight.
typedef	property_map<Graph, edge_weight_t>::type	WeightMap;

void testcase();

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}

void testcase() {
	int V, E;	// 1st line: <ver_no> <edg_no>
	cin >> V >> E;
	Graph G(V);
	WeightMap wm = get(edge_weight, G);
	for (int i = 0; i < E; ++i) {
		int u, v, c;
		Edge e;			// Each edge: <src> <tgt> <cost>
		cin >> u >> v >> c;
		tie(e, tuples::ignore) = add_edge(u, v, G);
		wm[e] = c;
	}
	
	vector<Edge> mst; // edge list to store mst
	kruskal_minimum_spanning_tree(G, back_inserter(mst));
	vector<Edge>::iterator eit;
	// Go through minimum spanning tree
	long sum_of_mst = 0;
	for (eit = mst.begin(); eit != mst.end(); ++eit) {
		sum_of_mst += wm[*eit];
	}
	cout << sum_of_mst;

	// Compute shortest path in G. start from node 0
	vector<int> dist(V);
	vector<int> pred(V);
/*	dijkstra_shortest_paths(G, 0,
		predecessor_map(make_iterator_property_map(pred.begin(),
			get(vertex_index, G))).
    		distance_map(make_iterator_property_map(dist.begin(),
			get(vertex_index, G))));
*/ //to idio me to apopanw. kai ta 2 swsta
	dijkstra_shortest_paths(G, 0, distance_map(&dist[0]) );
	int furthest_distance = -1;
	for (int i=0; i<V; ++i){ //krata thn apostash tou kombou poy apexei perissotero
		//if (furthest_distance < dist[i])
		furthest_distance = max(furthest_distance, dist[i]);
	}
	cout << " " << furthest_distance << endl;
}

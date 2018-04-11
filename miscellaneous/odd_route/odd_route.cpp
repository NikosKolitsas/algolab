#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore

using namespace std;
using namespace boost;

// Directed graph with int weights on edges.
typedef	adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, int> >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator
// Map edge -> weight.
typedef	property_map<Graph, edge_weight_t>::type	WeightMap;


void testcase() {
	int n,m;	// 1st line: <ver_no> <edg_no>
	cin >> n >> m;
	int start,finish;
	cin >> start >> finish;
	
	Graph G(4*n);
	WeightMap weight = get(edge_weight, G);


	// Split all vertices u, 0 <= u < V(G) into 4 parts
	// 4*u (even length , even weight)
	// 4*u+1 (even length , odd weight)
	// 4*u+2 (odd length , even weight)
	// 4*u+3 (odd length , odd weight)
	// add (u -> v) from E(G) to H}
	for(int i=0; i<m; ++i){
		int u, v, w;
		cin >> u >> v >> w;
		Edge e;			// Each edge: <src> <tgt> <weight>
		tie(e,tuples::ignore) = add_edge(4*u  ,  4*v+2  +w%2, G);	weight[e] = w;
		tie(e,tuples::ignore) = add_edge(4*u+1,  4*v+3  -w%2, G);	weight[e] = w;
		tie(e,tuples::ignore) = add_edge(4*u+2,  4*v    +w%2, G);	weight[e] = w;
		tie(e,tuples::ignore) = add_edge(4*u+3,  4*v+1  -w%2, G);	weight[e] = w;
	}
	// Compute shortest path in G. start from node start(even,even) dhladh o 4*start. afou sto start point
	//to mhkos einai 0 kai to baros epishs 0.
	//proorismos to  4*finish+3. afou 8elew odd mhkos odd baros
	vector<int> dist(4*n);
	dijkstra_shortest_paths(G, 4*start, distance_map(&dist[0]) );
	//an den mporw na ftasw sto destination apo to start point tote to dist meta ton dijkstra paramenei sto apeiro
	//kai afou edw einai vector apo int to sygkrinw me to INT_MAX
	if(dist[4*finish+3]==INT_MAX)		cout << "no\n";
	else					cout << dist[4*finish+3] << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}

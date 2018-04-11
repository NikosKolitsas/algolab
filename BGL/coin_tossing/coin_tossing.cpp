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
struct EdgeAdder {
	EdgeAdder(Graph & G, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge) 
		: G(G), capacity(capacity), rev_edge(rev_edge) {}

	void addEdge(int u, int v, long c) {
		Edge e, reverseE;
		tie(e, tuples::ignore) = add_edge(u, v, G);
		tie(reverseE, tuples::ignore) = add_edge(v, u, G);
		capacity[e] = c;
		capacity[reverseE] = 0;
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
	int n, m;
	cin >> n >> m;

	// Create Graph and Maps
	Graph G(m+n+2);
	EdgeCapacityMap	capacity = get(edge_capacity, G);
	ReverseEdgeMap	rev_edge = get(edge_reverse, G);
	ResidualCapacityMap	res_capacity = get(edge_residual_capacity, G);
	EdgeAdder ea(G,capacity, rev_edge);

	// Add edges
	int super_source = n+m;
	int super_sink = n+m+1;
	for(int i=0; i<m; i++){
		int a, b, c;
		cin >> a >> b >> c;
		if(c==1)	ea.addEdge(super_source, a, 1);
		else if(c==2)	ea.addEdge(super_source, b, 1);
		else{ 	//we don't know who wins so we add 2 edges
			ea.addEdge(n+i, a, 1);
			ea.addEdge(n+i, b, 1);
		}
	}

	// connect super source with matches 
	for(int i=0; i<m; i++){
		ea.addEdge(super_source, n+i, 1);
	}
	//and super sink with players with capacity s read from input
	long long desired_flow = 0;
	for(int i=0; i<n; i++){
		int s;		//final score of each player
		cin >> s;
		ea.addEdge(i, super_sink, s);
		desired_flow += s;
	}

	if(desired_flow != m){
		cout << "no\n";
		return;
	}
		
	// Calculate flow
	long flow = push_relabel_max_flow(G, super_source, super_sink);
//	cout << "Maximum Flow with the Push-Relabel Algorihtm: " << flow << endl;
	
	if(desired_flow == flow) 	cout << "yes\n";
	else 				cout << "no\n";

	//test if the flow from players to super_sink equals the capacity- score given
	//if not the output no
/*	bool correct = true;
	for(int i=0; i<n; i++){
		Edge e;
		bool found;
		tie(e, found) = edge(i, super_sink, G);
		if(found == false) cout << "\n\n\n!!!!!!!!error!!!!!!!!!\n\n\n";
		if(res_capacity[e] != 0){
			correct = false;
			break;
		}	
	}
*/

}

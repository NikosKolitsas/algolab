#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
typedef graph_traits<Graph>::out_edge_iterator			OutEdgeIt;


// Custom EdgeAdder that encapsulates the references to the graph and its
// property maps and always creates reverse edges with corresponding capacities.
struct EdgeAdder {
	EdgeAdder(Graph & G, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge) 
		: G(G), capacity(capacity), rev_edge(rev_edge) {}

	void addEdge(int u, int v, long c) {
		Edge e, reverseE;

		//does not exist so add both of them
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

void testcase(){
	int n, m , s, d;
	cin >> n >> m >> s >> d;
	
	Graph G(2*n+2);
	EdgeCapacityMap capacity = get(edge_capacity, G);
	ReverseEdgeMap rev_edge = get(edge_reverse, G);
	ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
	EdgeAdder ea(G, capacity, rev_edge);
	
	vector<int> s_vertices(s); //set of sources
	vector<int> d_vertices(d); //set of destinations

	//read all the edges
	for(int i=0; i<m; i++){
		int u, v;
		cin >> u >> v;
		
		ea.addEdge(  u+n,   v,   1);
	}

	//read set of sources
	for(int i=0; i<s; i++){
		cin >> s_vertices[i];
	}
	//read set of destinations
	for(int i=0; i<d; i++){
		cin >> d_vertices[i];
	}

	//connect u with u'
	for(int u=0; u<n; u++){
		ea.addEdge(  u ,   u+n ,   1);
	}
	//connect with super_source
	int super_source = 2*n;
	int super_sink = 2*n+1;

	for(int si : s_vertices){
		ea.addEdge(  super_source ,   si ,   1);
	}	
	for(int di : d_vertices){
		ea.addEdge(  di+n ,   super_sink ,   1);
	}	

	//max-flow
	long flow = push_relabel_max_flow(G, super_source, super_sink);
	cout << flow << endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}

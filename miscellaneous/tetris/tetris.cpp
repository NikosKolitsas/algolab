#include <iostream>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
	property<edge_capacity_t, long,
		property<edge_residual_capacity_t, long,
			property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator			EdgeIt;	// Iterator
typedef graph_traits<Graph>::out_edge_iterator			OutEdgeIt;


struct EdgeAdder {
	EdgeAdder(Graph & G, EdgeCapacityMap &capacity,
		ReverseEdgeMap &rev_edge) : G(G), capacity(capacity),
			rev_edge(rev_edge) {}
	void addEdge(int u, int v, long c) {
		Edge e, rev;
		tie(e, tuples::ignore) = add_edge(u, v, G);
		tie(rev, tuples::ignore) = add_edge(v, u, G);
		capacity[e] = c;
		capacity[rev] = 0;
		rev_edge[e] = rev;
		rev_edge[rev] = e;
	}
	Graph &G;
	EdgeCapacityMap &capacity;
	ReverseEdgeMap &rev_edge;
};


void testcase(){
	int w, n;
	cin >> w >> n;
	
	Graph G(2*w+2);
	EdgeCapacityMap capacity = get(edge_capacity, G);
	ReverseEdgeMap rev_edge = get(edge_reverse, G);
	ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
	EdgeAdder ea(G, capacity, rev_edge);


	int source = w+1;
	int sink = w;
	vector<vector<int> > edge_count(w+1,  vector<int>(w+1,0));
	for(int i=0; i<n; i++){
		int u,v;
		cin >> u >> v;
		
		if(u==v)	continue;
		if(u > v)	swap(u,v);

		//etsi exoume DAG. akmes mono apo mikroterous se megaluterous kombous. oxi self loops
		edge_count[u][v]++;	
	}

	//now add the edges. all edges have capacity 1
	for(int i=0; i<=w-1; i++){
		for(int j=i+1; j<=w; j++){
			if(edge_count[i][j]!=0)
				ea.addEdge( w+1+ i, j, 1);
		}
	}
	//add edges between v and v'
	for(int i=1; i<=w-1; i++){
		ea.addEdge( i, w+1+ i, 1);
	}

	//now correct the capacity of edge  source-sink  dhladh 0-w
	Edge e;
	bool exist;
	tie(e, exist) = edge( source, sink, G);
	if(exist){ //correct the capacity
		capacity[e] = edge_count[0][w];
	}

	long flow = push_relabel_max_flow(G, source, sink);
	cout << flow << endl;
}


int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}


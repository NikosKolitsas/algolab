#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore

using namespace std;
using namespace boost;

// Directed graph
typedef	adjacency_list<vecS, vecS, directedS,
		no_property,
		no_property >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef	graph_traits<Graph>::vertex_descriptor	VertexDescriptor;// Vertex type
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator

void testcase(){
	int n,m;
	cin >> n >> m;

	Graph G(n);
	for(int i=0; i<m; i++){
		int u,v;
		cin >> u >> v;
	
		add_edge(u-1,v-1, G);
	}

	vector<int> cost(n);
	for(int i=0; i<n; i++){
		cin >> cost[i];
	}

	vector<int> scc(n);
	int scc_num = strong_components(G, &scc[0]);

	//see page 18. how many red scc_vertices exist. how many components with no in_eges. only out_edges
	vector<bool> no_in_edges(scc_num, true);
	EdgeIt ebeg, eend;
	// Iterate over all edges.
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		// ebeg is an iterator, *ebeg is a descriptor.
		int u = source(*ebeg, G), v = target(*ebeg, G);
		if (scc[u] != scc[v]) {//edge between scc.  (kai oxi adkmh within a scc)
			no_in_edges[scc[v]] = false;	//to v scc den exei in_edge
		}	
	}

	//gia ka8e scc briskw ton pio fthno kombo pou perixei. ayto 8a einai to kostos tou scc autoy.
	vector<int> scc_min_cost(scc_num, INT_MAX);
	for(int i=0; i<n; i++){
		int prev_cost = scc_min_cost[ scc[i]  ];

		scc_min_cost[ scc[i]  ] =    min( prev_cost  ,  cost[i]  );
	}

	//synoliko kostos= a8roisma tous kostous olwn twn no_in_egdes scc. kokkinwn kombwn.
	long total_cost=0;
	for(int i=0; i<scc_num; i++){
		if(no_in_edges[i])		total_cost += scc_min_cost[i];
	}
	cout << total_cost << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
	return 0;
}




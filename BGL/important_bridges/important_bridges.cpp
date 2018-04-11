#include <climits>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore
#include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;

namespace boost {
	enum edge_info_t { edge_info = 219 }; // A unique ID.
	BOOST_INSTALL_PROPERTY(edge, info);
}

// Directed graph with int weights on edges.
typedef	adjacency_list<vecS, vecS, undirectedS,
		no_property,
		property<edge_info_t, int> >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator
// Map edge -> weight.
typedef	property_map<Graph, edge_info_t>::type	InfoMap;

void testcase();

struct Edge_output{
	int u;  //u must always be smaller than v. u<=v
	int v;
	bool operator<(const Edge_output& rhs) const{
		if( u < rhs.u ) return true;	//return true αν ηδη σωστη η διαταξη. δηλαδη το struct ayto πρεπει να ειναι
		else if( u==rhs.u && v<=rhs.v) return true;		//αριστερα του struct της παραμετρου
		return false; 
	}
	Edge_output(Edge e, Graph G){
		int a = source(e,G)+1;
		int b = target(e,G)+1;
		u = min(a,b);
		v = max(a,b);
	}
};

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}

void testcase() {
	int V, E;
	cin >> V >> E;
	Graph G(V);  
	InfoMap im = get(edge_info, G);
	//diabazw ta edges kai kataskeyazw grafo
	for (int i = 0; i < E; ++i) {
		int u, v;
		Edge e;			// Each edge: <src> <tgt> <cost>
		cin >> u >> v;	//h eisodos einai sto [1,n] anti gia [0,n-1]
		tie(e, tuples::ignore) = add_edge(u-1, v-1, G);
	}
	
	int num_comp = biconnected_components(G, im);

	//ftiaxnw enan pinaka pou metra poses edges exei to ka8e component
	vector<int> count_edges_of_comp(num_comp, 0); //arxika counters sto zero
	EdgeIt ei, ei_end;
	//dhladh gia oles ta edges, briskw sto InfoMap to component tous kai auksanw ton counter
	for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
		count_edges_of_comp[ im[*ei]  ]++;
	}
	
	// Count number of bridges e.g. components with 1 edge 
	// loop on all edges, see in which component they belong. then test on count_edges_of_comp if
	// they are unique. and if yes then increase bridge_counter and add-store the edge to bridges vector
	//the number of bridges can also be found by simply counting assous "1" at count_edges_of_comp	
	vector<Edge_output> bridges;
	int bridges_counter = 0;
	for(tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
		if (count_edges_of_comp[  im[*ei]  ] == 1) {	//brhkame bridge
			bridges.push_back(  Edge_output(*ei, G)  );
			bridges_counter++;
		}
	}
	std::sort(bridges.begin(), bridges.end());

	cout << bridges_counter << endl;
	for(Edge_output eout : bridges){
		cout << eout.u  << " " << eout.v << endl;
	}
	
}

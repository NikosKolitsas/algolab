#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>  //gia to abs() absolute value

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graphviz.hpp>

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

		//check if edge already exists. if yes then just increase the capacity
		//the capacity of the reverseE remains zero. nothing change to reverseE
		bool found;
		tie(e, found) = edge(u, v, G);
		if(found == true){
//cout << "("<< u << "," << v << ")   edge found cap was:  " << capacity[e] << " now update to " << capacity[e]+c << endl;
			capacity[e] += c;
			return;
		}

		//does not exist so add both of them
		tie(e, tuples::ignore) = add_edge(u, v, G);
		tie(reverseE, tuples::ignore) = add_edge(v, u, G);
		capacity[e] = c;
		capacity[reverseE] = 0;
		rev_edge[e] = reverseE;
		rev_edge[reverseE] = e;
//cout << "("<< u << "," << v << ")" << "  cap[e]= " << capacity[e] << "    cap[revE]= " << capacity[reverseE] << endl;
	}
	Graph &G;
	EdgeCapacityMap	&capacity;
	ReverseEdgeMap	&rev_edge;
};

void testcase();
void print_edges(Graph);

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
	Graph G(n+2);
	EdgeCapacityMap	capacity = get(edge_capacity, G);
	ReverseEdgeMap	rev_edge = get(edge_reverse, G);
	ResidualCapacityMap	res_capacity = get(edge_residual_capacity, G);
	EdgeAdder ea(G,capacity, rev_edge);

	// read each vertex initial and demand and create edges 
	int super_source = n;
	int super_sink = n+1;
	for(int i=0; i<n; i++){
		int g, d;
		cin >> g >> d;
		if(g-d > 0){	//exw pleonasma ara enwsh me super_souce
			ea.addEdge(super_source, i, g-d);
		}else if(g-d < 0){  //elleima ara enwsh me super_sink
			ea.addEdge(i, super_sink, d-g);  //pros8etw to abs(g-d)
		}
	}
	// now read the edges 
	for(int i=0; i<m; i++){
		int f, t, c, C;
		cin >> f >> t >> c >> C;
		//check if it is self loop. if yes then create a new vertex and only connect it
		//with its other self.
		if(f==t){
			//Traits::vertex_descriptor vert = add_vertex(graph);
		//	t = add_vertex(G);
		}if(f!=t){
		ea.addEdge(f, t, C-c);
		ea.addEdge(f, super_sink, c);
		ea.addEdge(super_source, t, c);
		}
	}
//write_graphviz(std::cout, G);
//print_edges(G);

	// Calculate flow
	long flow = push_relabel_max_flow(G, super_source, super_sink);

	//test if all the outcoming edges of super_source are saturated
	//if not then output no  (no solution)
	bool correct = true;
	OutEdgeIt eit, eit_end;	
	for(tie(eit, eit_end) = out_edges(super_source, G);   eit!=eit_end;  ++eit){
		if(res_capacity[*eit] != 0){
			correct = false;
			break;
		}
	}
	if(correct)	cout << "yes\n";
	else 		cout << "no\n";

}


void print_edges(Graph G){
	EdgeCapacityMap	capacity = get(edge_capacity, G);
	ReverseEdgeMap	rev_edge = get(edge_reverse, G);

	cout << "\n\nprint_edges\n";
	EdgeIt ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei){
		Edge e = *ei;
		Edge revE = rev_edge[e];
    		cout << "(" << source(e, G) << ", " << target(e,G) << ")	" << "cap= " << capacity[e];
    		cout << " revE= (" << source(revE, G) << ", " << target(revE,G) << ")	" << "cap= " << capacity[revE] << endl;
	}

}

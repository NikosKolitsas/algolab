#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
property<edge_capacity_t, long,
property<edge_residual_capacity_t, long,
property<edge_reverse_t, Traits::edge_descriptor,
property<edge_weight_t, long> > > > >  Graph; // new!;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph , edge_weight_t>::type EdgeWeightMap; // new!
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef graph_traits<Graph>::vertex_descriptor Vertex;


struct EdgeAdder {
EdgeAdder(Graph & G, EdgeCapacityMap &capacity , EdgeWeightMap &weight, /*new*/
ReverseEdgeMap &rev_edge)
: G(G), capacity(capacity), weight(weight) /*new*/ ,
rev_edge(rev_edge) {}
void addEdge(int u, int v, long c, long w /*new*/) {
Edge e, rev;
tie(e, tuples::ignore) = add_edge(u, v, G);
tie(rev, tuples::ignore) = add_edge(v, u, G);
capacity[e] = c;
weight[e] = w; // new!
capacity[rev] = 0;
weight[rev] = -w; //new!
rev_edge[e] = rev;
rev_edge[rev] = e;
}
Graph &G;
EdgeCapacityMap &capacity;
EdgeWeightMap &weight; /*new*/
ReverseEdgeMap &rev_edge;
};

void testcase(){
	int n;
	cin >> n;
	

	Graph G(4*n);
EdgeCapacityMap capacity = get(edge_capacity, G);
ReverseEdgeMap rev_edge = get(edge_reverse, G);
ResidualCapacityMap res_capacity
= get(edge_residual_capacity, G);
EdgeWeightMap weight = get( edge_weight, G); /*new*/
EdgeAdder ea(G, capacity, weight    /*new*/ , rev_edge);

	//apo edw kai pera ola ta alla idia. mono to add_edge exei epipleon parametro

	vector<int> a(n);
	vector<int> c(n);
	vector<int> s(n);
	vector<int> p(n);
	vector<int> u(n-1);
	vector<int> e(n-1);
	for(int i=0; i<n; i++){
		cin >> a[i];
		cin >> c[i];
	}
	for(int i=0; i<n; i++){
		cin >> s[i];
		cin >> p[i];
	}
	for(int i=0; i<n-1; i++){
		cin >> u[i];
		cin >> e[i];
	}

	//all input is read
	
	//create edges
	int source = 2*n;
	int sink = 2*n+1;
	int f_offset = 2*n+2;
	int f_split = 3*n+1;

	//h megisth timh. 8a thn pros8esw wste mh arnhtikes times sto cost tou grafou
	int max_price = *( std::max_element(p.begin(), p.end()) );
	//link days with source. and days upper level with sink
	//also link days_lower_level with days_upper_level
	for(int i=0; i<n; i++){
		ea.addEdge( source , i , a[i], c[i]);		
		ea.addEdge( n+i , sink , s[i],   max_price-p[i]  );		
		ea.addEdge( i ,  n+i  , LONG_MAX,  0);		
	}

	//twra menon oi mayroi komboi kai oi mayres akmes. dhladh ola tou freezer
	for(int i=0; i<n-1; i++){
		//link days_lower_level with freezer
		//thn teleytaia mera (n-1) den apo8hkeyoume profanws gia thn epomenh
		ea.addEdge( i ,  f_offset+i  , LONG_MAX , e[i]);	//sto sxhma (1)
		//link freezeri with freezeri_split    //control capacity
		ea.addEdge(  f_offset+i ,  f_split+i  , u[i] ,   0  );		//sto sxhma (2)
		//link with days_upper_level	
		ea.addEdge( f_split+i ,  n+1+i  , LONG_MAX,  0);		//sto sxhma (3)
		//syndesh freezeri_split me freezeri+1
		if( i != n-2  )	//oxi ston teleytaio freezer
			ea.addEdge( f_split+i ,  f_offset+i+1   , LONG_MAX ,  e[i]);		//sto sxhma (4)
	}


	// Option 2: (only for non -negative weights !)
	successive_shortest_path_nonnegative_weights(G, source, sink);
	long cost = find_flow_cost(G);
	int flow = 0;
	//Iterate over all edges leaving the source
	OutEdgeIt eit , eend ;
	for(tie(eit,eend)=out_edges(vertex(source ,G) , G) ; eit != eend ; ++eit ) {
		flow += (  capacity[*eit] - res_capacity[*eit]  ) ;
	}
				
	//recalculate cost. remove added cost of max_price
	//this cost was added to every unit of flow exactly once.
	long correct_cost = cost - flow*max_price;
	long profit =  -correct_cost;
	
	long all_student_requests = std::accumulate(s.begin(), s.end(), 0);
	if(flow==all_student_requests)		cout << "possible ";
	else					cout << "impossible ";

	cout << flow << " " << profit << endl;
	
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}


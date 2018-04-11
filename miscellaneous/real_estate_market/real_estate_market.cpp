#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
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
typedef property_map<Graph, edge_capacity_t>::type 	EdgeCapacityMap;
typedef property_map<Graph , edge_weight_t>::type 	EdgeWeightMap; // new!
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type 	ReverseEdgeMap;
typedef graph_traits<Graph>::edge_descriptor 		Edge;
typedef graph_traits<Graph>::out_edge_iterator 		OutEdgeIt;
typedef graph_traits<Graph>::vertex_descriptor 		Vertex;


struct EdgeAdder {

	EdgeAdder(Graph& G, EdgeCapacityMap& capacity, EdgeWeightMap& weight, /*new*/
	  ReverseEdgeMap& rev_edge)
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


	Graph& G;
	EdgeCapacityMap& 	capacity;
	EdgeWeightMap& 		weight; /*new*/
	ReverseEdgeMap& 	rev_edge;
};

void testcase(){
	int N, M, S;
	cin >> N >> M >> S;

	Graph G(N+M+S+2);
	EdgeCapacityMap capacity = get(edge_capacity, G);
	ReverseEdgeMap rev_edge = get(edge_reverse, G);
	ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
	EdgeWeightMap weight = get( edge_weight, G); /*new*/
	EdgeAdder ea(G, capacity, weight    /*new*/ , rev_edge);


	int source = N+M+S;
	int sink = N+M+S+1;

	//read state limits
	for(int i=0; i<S; i++){
		int state_limit;
		cin >> state_limit;
		ea.addEdge( N+M+ i , sink ,  state_limit ,  0 );		
	}

	//read where every piece belongs
	for(int i=0; i<M; i++){
		int belongs_to;
		cin >> belongs_to;
		belongs_to--; //wste na arxizei apo to 0 kai oxi apo to 1
		ea.addEdge( N+ i , N+M+ belongs_to ,  1  ,  0 );			
	}

	//read all bids. store them. find max_bid. and then insert all edges
	vector<vector<int> > bid( N  ,  vector<int>(M) );
	int max_bid = -1; //akyrh timh
	
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			int temp;
			cin >> temp;
			if(max_bid < temp)	max_bid = temp;
			bid[i][j] = temp;
		}
	}
	//now insert all edges
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			ea.addEdge(  i , N+ j ,  1  ,  max_bid-bid[i][j]  );			
		}
	}

	//connect source with buyers
	for(int i=0; i<N; i++){
		ea.addEdge(  source , i ,  1  ,  0 );			
	}

	//graph complete!

		// Option 2: (only for non -negative weights !)
	successive_shortest_path_nonnegative_weights(G, source, sink);
	long flow_cost = find_flow_cost(G);
	int flow = 0;
	//Iterate over all edges leaving the source
	OutEdgeIt eit , eend ;
	for(tie(eit,eend)=out_edges(vertex(source ,G) , G) ; eit != eend ; ++eit ) {
		flow += (  capacity[*eit] - res_capacity[*eit]  ) ;
	}
				
	//recalculate cost. remove added cost of max_price
	//this cost was added to every unit of flow exactly once.
	long profit = -flow_cost + flow*max_bid;	
	cout << flow << " " << profit << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}

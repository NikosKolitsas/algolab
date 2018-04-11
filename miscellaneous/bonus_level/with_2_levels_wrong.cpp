#include <iostream>
#include <vector>

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
typedef property_map<Graph, edge_capacity_t>::type 		EdgeCapacityMap;
typedef property_map<Graph , edge_weight_t>::type 		EdgeWeightMap; // new!
typedef property_map<Graph, edge_residual_capacity_t>::type 	ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type 		ReverseEdgeMap;
typedef graph_traits<Graph>::edge_descriptor 			Edge;
typedef graph_traits<Graph>::edge_iterator			EdgeIt;	// Iterator
typedef graph_traits<Graph>::out_edge_iterator 			OutEdgeIt;
typedef graph_traits<Graph>::vertex_descriptor 			Vertex;


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
	int n2 = n*n;
	
	Graph G(2*n2 + 2);
	EdgeCapacityMap capacity = get(edge_capacity, G);
	ReverseEdgeMap rev_edge = get(edge_reverse, G);
	ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
	EdgeWeightMap weight = get( edge_weight, G); /*new*/
	EdgeAdder ea(G, capacity, weight    /*new*/ , rev_edge);


	int source = 2*n2;
	int sink = 2*n2+1;

	int max_v=0; //ayto 8a to pros8esw se oles tis akmes afou tous balw arnhtiko proshmo
	int debug_sum=0;
	vector<vector<int> > a( n, vector<int>(n)  );
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cin >> a[i][j];
			max_v = max( max_v   ,  a[i][j]  );
debug_sum += a[i][j];
		}
	}

	//create main body of graph   n^2 grid
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			//diasxish kata grammes. ka8e kombo syndese ton me ton deksia kai ton katw
			//argmuments:  from, to, capacity, weight=cost
			if(j+1<n){	//dyndesh me ton deksia an yparxei.  
				ea.addEdge(    i*n+j ,     i*n+j+1 ,  1,  max_v - a[i][j+1]  );//level 0
				ea.addEdge(n2+ i*n+j , n2+ i*n+j+1 ,  1,  max_v - 0  );//level 1
				
			}if(i+1<n){	//dyndesh me ton katw an yparxei.
				ea.addEdge(    i*n+j ,     (i+1)*n+j ,  1,  max_v -  a[i+1][j]  );//L0
				ea.addEdge(n2+ i*n+j , n2+ (i+1)*n+j ,  1,  max_v -  0  );//L1
			}
		}
	}

	//syndesh source me start
	ea.addEdge( source , 0 ,  1 , max_v - a[0][0] );
	ea.addEdge( source , n2,  1 , max_v - 0 );	//source me start'  (start tou L0)
	ea.addEdge( n2-1 , sink , 2 ,  0 );

	
	//syndesh level0 with level1. ka8e kombos tou L0 me ton antistoixo tou L1.
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
		//	ea.addEdgeUndirected(       i*n+j ,  n2+  i*n+j ,  1,  0  );
			ea.addEdge(       i*n+j ,  n2+  i*n+j ,  1,  0  );
			ea.addEdge(  n2+  i*n+j ,       i*n+j ,  1,  0  );
		}
	}



	//grafos einai etoimos
	// Option 2: (only for non -negative weights !)
	successive_shortest_path_nonnegative_weights(G, source, sink);
	long flowcost = find_flow_cost(G);
	int flow = 0;
	//Iterate over all edges leaving the source
	OutEdgeIt eit , eend ;
	for(tie(eit,eend)=out_edges(vertex(source ,G) , G) ; eit != eend ; ++eit ) {
		flow += (  capacity[*eit] - res_capacity[*eit]  ) ;
	}
				
	//recalculate cost. remove added cost of max_v
	//this cost was added to every unit of flow exactly (2*n-1) times.
	long profit =  -flowcost +   flow * (2*n-1) * max_v;
	
	cout <<  profit << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}

void print_map(ResidualCapacityMap& res_capacity, Graph G){
	EdgeIt eit , eend ;
        for(tie(eit,eend)=edges(G) ; eit != eend ; ++eit ) {
		cout << "(" << source(*eit, G) << " , " << target(*eit,G) << ")    res_cap=  " << res_capacity[*eit] << endl;
	}
}
void print_map(EdgeWeightMap& weight, Graph G){
	EdgeIt eit , eend ;
        for(tie(eit,eend)=edges(G) ; eit != eend ; ++eit ) {
		cout << "(" << source(*eit, G) << " , " << target(*eit,G) << ")    weight=  " << weight[*eit] << endl;
	}
}

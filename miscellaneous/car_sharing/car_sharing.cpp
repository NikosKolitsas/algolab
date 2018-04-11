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
	int N, S;
	cin >> N >> S;
	
	//arxika mono source, sink kai ta S stations gia t=0 (syndesh me source apey8eias)
	//kai ta S stations gia t=100.000 syndesh me sink apey8eias
	//meta 8a pros8etw kombous th stigmh pou diabazw tis akmes. to teliko megethos profanw megalytero
	Graph G(2+ 2*S); 
	EdgeCapacityMap capacity = get(edge_capacity, G);
	ReverseEdgeMap rev_edge = get(edge_reverse, G);
	ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
	EdgeWeightMap weight = get( edge_weight, G); /*new*/
	EdgeAdder ea(G, capacity, weight    /*new*/ , rev_edge);


	int source = 0;
	int sink = 1;
	long unique_vertex_id = 2 + 2*S;
	long P = 100;	//ayto einai to max_v
	vector<std::map<int,long> > station(S);

	//syndesh source me stations. ta stations exoun offest arithmhshs 2
	//syndesh kai me sink. ta stations auta exoun offset s+2
	for(int i=0; i<S; i++){
		int l;
		cin >> l;	//arxiko apo8ema amaksiwn stous sta8mous
		ea.addEdge( source  ,     2+i,        l ,  0); //mhdeniko kostos
		ea.addEdge( S+2  +i ,   sink , LONG_MAX ,  0); //mhdeniko kostos
		//tous bazw kai sto map tou ka8e station	
		station[i][0] = 2  +i;
		station[i][100000] = S+2  +i;
	}

	//diabazw ola ta edges
	for(int i=0; i<N; i++){
		int s,t,d,a,p;
		cin >> s >> t >> d >> a >> p;
		s--; t--;  //bash to 0

		//elegxos an yparxei o kombos  s-d   dhladh station==s kai xronos==d
		if(station[s].count(d) == 0) //den yparxei ara ton pros8etw
			station[s][d] = unique_vertex_id++;
		//elegxos an yparxei o kombos  t-a   dhladh station==t kai xronos==a
		if(station[t].count(a) == 0) //den yparxei ara ton pros8etw
			station[t][a] = unique_vertex_id++;

		//capacity==1 mono 1 amaksi ana booking. cost= 
		ea.addEdge(  station[s][d] ,  station[t][a] ,  1 ,  (a-d)*P - p );
	}


	//syndesh twn diadoxikwn kombwn tou ka8e station metaksy tous.
	for(int i=0; i<S; i++){//gia ka8e station
		//genika to prev deixnei ston prohgoumeno pair tou map
		auto prev = station[i].begin(); //ayto deixnei ston kombo time=0 tou station i
		// to cur arxika deixnei sto 2o stoixeio tou map
		for(auto cur= ( ++station[i].begin() ); cur!= station[i].end();  ++cur, prev++){
			ea.addEdge(  prev->second ,  cur->second ,   LONG_MAX /*apeirh xwrhtikothta*/  ,
				  (cur->first - prev->first) * P ); //cost = diafora_xrwnwn * P
		}
	}


	//grafos einai etoimos
	// Option 2: (only for non -negative weights !)
	successive_shortest_path_nonnegative_weights(G, source, sink);
	long flowcost = find_flow_cost(G);
	long flow = 0;
	//Iterate over all edges leaving the source
	OutEdgeIt eit , eend ;
	for(tie(eit,eend)=out_edges(vertex(source ,G) , G) ; eit != eend ; ++eit ) {
		flow += (  capacity[*eit] - res_capacity[*eit]  ) ;
	}
				
	//recalculate cost. remove added cost of max_v==P==100  (hardcoded)
	//this cost was added to every unit of flow exactly 100.000 times.
	long profit =  -flowcost +   flow * 100000 * P;
	
	cout <<  profit << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}


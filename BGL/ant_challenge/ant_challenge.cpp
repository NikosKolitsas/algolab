#include <climits>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore

#include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;

// Directed graph with int weights on edges.
typedef	adjacency_list<vecS, vecS, undirectedS,
		no_property,
		property<edge_weight_t, int> >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator
// Map edge -> weight.
typedef	property_map<Graph, edge_weight_t>::type	WeightMap;

void testcase();

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}

void testcase() {
	int V, E, species, a, b;	// 1st line: <ver_no> <edg_no>
	cin >> V >> E >> species >> a >> b;
	Graph G(V);   	//se ayton ton grafo 8a kanw prim (species fores) gia na brw to network tou ka8e zwou
	Graph G_all(species*V+2);
	WeightMap wm = get(edge_weight, G);
	WeightMap wm_all = get(edge_weight, G_all);

	vector<Edge> my_edge_descriptors(E);
	vector<vector<int> > edge_weights( E, vector<int>(species)  );  //E x species   E:grammes species:sthles
	for (int i = 0; i < E; ++i) {	//diabazw input
		int u, v;
		Edge e;			// Each edge: <src> <tgt> <cost>
		cin >> u >> v;
		tie(e, tuples::ignore) = add_edge(u, v, G);
		my_edge_descriptors[i] = e;
		
		for(int j=0; j<species; j++)
			cin >> edge_weights[i][j];  
	}
	
	//diabazw to start point tou ka8e species
	vector<int> hive(species);
	for(int i=0; i<species; i++)
		cin >> hive[i];
	//for each species adjust the cost of the edges and run prim algorithm
	vector<int> p(V); // predecessor vector
	for(int i=0; i<species; i++){
		for(int j=0; j<E; j++)	//kata sthles diabazw ta barh twn akmwn
			wm[  my_edge_descriptors[j]  ] = edge_weights[j][i];
		prim_minimum_spanning_tree(G, &p[0], root_vertex(hive[i]) );

		//twra exw tis akmes tou species i opote tis pros8etw ston megalo syn8eto grafo.
		for (int j = 0; j < V; ++j) {
			Edge e; bool success;
			tie(e, success) = edge(j, p[j], G);	//pairnw mia mia akmh tou MST kai thn pros8etw ston megalo
			if (success) {	//mallon elegxei oti yparxei ayth h akmh. px o root tou MST exei parent ton eauto tou
				int u = i*V+j; //to ena akro ths akmhs
				int v = i*V+p[j]; //to ena akro ths akmhs
				Edge e_new;
				tie(e_new, tuples::ignore) = add_edge(u, v, G_all);
				wm_all[e_new] = wm[e];
			}
		}
	}

	//pros8etw tis syndeseis me to prohgoumeno epipedo
	//ton kombo i ton enwnw me ton i-V me akmh kostous 0
	for(int i=1; i<species; i++){	//gia ola ta epipeda
		for(int j=0; j<V; j++){ //gia olous tous kombous
			Edge e;
			tie(e, tuples::ignore) = add_edge(i*V+j, (i-1)*V+j, G_all);
			wm_all[e] = 0;
		}
	}
	//pros8etw super Source sth 8esh V*species 
	//kai super Target sth 8esh V*species+1  dhladh kai ta dyo sto telos
	//enwnw super Source me ta   a   olwn twn epipedwn
	int super_source = V*species, super_target = V*species+1;
	for(int i=0; i<species; i++){
		Edge e;
		tie(e, tuples::ignore) = add_edge(super_source, i*V+a, G_all);
		wm_all[e] = 0;	
		tie(e, tuples::ignore) = add_edge(super_target, i*V+b, G_all);
		wm_all[e] = 0;	
	}

	// Compute shortest path in G_all from super_source to super_target0
	vector<long long> dist(num_vertices(G_all));
//	cout << "num_vertices(G_all) = " << num_vertices(G_all);
//	write_graphviz(std::cout, G_all);		
	dijkstra_shortest_paths(G_all, super_source, distance_map(&dist[0]) );
	cout << dist[super_target] << endl;
}

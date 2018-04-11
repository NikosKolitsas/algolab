#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, 
		no_property,
		property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator      EdgeIt; // Iterator

// Map edge -> weight.
typedef property_map<Graph, edge_weight_t>::type        WeightMap;

void print_edges(Graph);

void testcase(){
	int n, m, k, start, y;
	cin >> n >> m >> k >> start >> y;
	int finish = k*n + y;	//ston aykshmeno grafo autos 8a einai o proorismos

	Graph G(n);  //initial graph 
	Graph G_all;
	WeightMap wm = get(edge_weight, G);
	WeightMap wm_all = get(edge_weight, G_all);
	//store here all edges that are rivers
	vector<Edge> rivers;
	for(int i=0; i<m; i++){	//read all edges
		int a, b, c, d;
		cin >> a >> b >> c >> d;
	
		Edge e;			// Each edge: <src> <tgt> <cost>
		tie(e, tuples::ignore) = add_edge(a, b, G);
		wm[e] = c;
		if(d==1)	rivers.push_back(e);
	}

	for(int i=0; i<=k; i++){
		copy_graph(G, G_all);
	}
	assert(num_vertices(G_all) == (k+1)*n);

	//connect the different levels  metaksy tous	
	for(Edge e : rivers){
		int u = source(e, G_all);
		int v = target(e, G_all);
		int c = wm[e];

		for(int i=0; i<k; i++){
			Edge new_e;	
			tie(new_e, tuples::ignore) = add_edge( i*n+u , (i+1)*n+v , G_all);
			wm_all[new_e] = c;
			tie(new_e, tuples::ignore) = add_edge( i*n+v , (i+1)*n+u , G_all);
			wm_all[new_e] = c;
		}
	}
	//cout << "num_vertices(G_all) = " << num_vertices(G_all) << endl;
	//print_edges(G_all);
	//kanw dijkstra
	vector<int> dist(num_vertices(G_all));
	dijkstra_shortest_paths(G_all, start, distance_map(&dist[0]));
	cout << dist[finish] << endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--){	
		testcase();
	}
	return 0;
}

void print_edges(Graph G){
        cout << "\n\nprint_edges\n";
        WeightMap wm = get(edge_weight, G);

        EdgeIt ei, ei_end;
        for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei){
                Edge e = *ei;
                cout << "(" << source(e, G) << ", " << target(e,G) << ")        " << "weight= " << wm[e] << endl;
        }
}


#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp> // tuples::ignore
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property,
	property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIterator;
typedef property_map<Graph, edge_weight_t>::type WeightMap;


using namespace std;
using namespace boost;

void testcase() {
	int n, start;
	cin >> n >> start;
	start--;

	Graph G(n);
	WeightMap wm = get(edge_weight, G);


	vector<vector<int> > d( n ,  vector<int>(n,0)  );
	for(int i=0; i<n-1; i++){
		for(int j=i+1; j<n; j++){
			int cost;
			cin >> cost;
			d[i][j] = cost;
			d[j][i] = cost;
			Edge e; // Each edge: <src> <tgt> <cost>
			tie(e, tuples::ignore) = add_edge(i, j, G); 
			wm[e] = cost;
		}
	}
	
	vector<int> p(n); // predecessor vector
	prim_minimum_spanning_tree(G, &p[0] ,  root_vertex(start) );

	//gia ka8e akmh pou den anoikei sto prim dentro kanw to ekshs:
	// a) thn pros8etw sto dentro.
	// b) briskw ta dyo akra ths (kombous) kai paw pros ta panw sto prim dentro mexri ta dyo monopatia
	// na synanth8oyn. ekei exw brei kai diasxisei ton kyklo pou dhmioyrghthhke. opote kata thn disxish
	// ypologizw thn megalyterh akmh tou dentrou. aythn sto telos thn afairw. 

	// neo spanning_tree = prim_tree + new_edge - max_old_edge     >= prim_tre

	//se ayto 8a kanoume tis dfs
	vector<vector<int> > graph(n); //grafos gia to prim dentro.
	long MST_cost = 0;
	for(int i=0; i<n; i++){
		if(  p[i]!=i  ){ 	//isxyei mono ston root. p[start]==start
			graph[i].push_back(  p[i] );
			graph[ p[i] ].push_back(  i );	//undirected

			MST_cost += d[i][p[i]];
		}
	}

	//n fores dfs. arxizw apo olous tous kombous
	vector<vector<int> > max_edge(n , vector<int>(n));
	// DFS
	for(int dfs_s=0; dfs_s<n; dfs_s++){
		vector<int> stack;
		vector<bool> visited(n,false);
		stack.push_back( dfs_s );
		visited[ dfs_s ] = true;
		while (!stack.empty()) {
			int u = stack.back();
			stack.pop_back();
			//for all children
			for(int v : graph[u]){
				if(visited[v])	continue;
				//briskw to max edge apo root mexri kai ta paidia mou
				max_edge[ dfs_s ] [ v ] = max(  max_edge[dfs_s] [u] ,  d[u][v]  );
				max_edge[ v ] [ dfs_s  ] = max_edge[ dfs_s ] [ v ];
				
				stack.push_back( v );
				visited[v] = true;
			}
 		}
	}

	//now iterate over all edges of G. 
	long min_additional_cost = LONG_MAX; 
	EdgeIterator eit, eend;
	for (tie(eit, eend) = edges(G); eit != eend; ++eit) {
		// eit is EdgeIterator, *eit is EdgeDescriptor
		int u = source(*eit, G), v = target(*eit, G);
		//if(u==v)	continue; //self loop
		if( u==p[v] || v==p[u]  ) continue; //edge belongs to prim tree
		
		//we add the edge u-v and remove the heaviest in the circle
		long added_cost = d[u][v] - max_edge[u][v];
		if(min_additional_cost > added_cost)	min_additional_cost = added_cost;
	}

	cout << MST_cost+min_additional_cost << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T; // First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}



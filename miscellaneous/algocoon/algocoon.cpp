#include <iostream>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
	property<edge_capacity_t, long,
		property<edge_residual_capacity_t, long,
			property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator			EdgeIt;	// Iterator
typedef graph_traits<Graph>::out_edge_iterator			OutEdgeIt;


struct EdgeAdder {
	EdgeAdder(Graph & G, EdgeCapacityMap &capacity,
		ReverseEdgeMap &rev_edge) : G(G), capacity(capacity),
			rev_edge(rev_edge) {}
	void addEdge(int u, int v, long c) {
		Edge e, rev;
		tie(e, tuples::ignore) = add_edge(u, v, G);
		tie(rev, tuples::ignore) = add_edge(v, u, G);
		capacity[e] = c;
		capacity[rev] = 0;
		rev_edge[e] = rev;
		rev_edge[rev] = e;
	}
	Graph &G;
	EdgeCapacityMap &capacity;
	ReverseEdgeMap &rev_edge;
};


void testcase(){
	int n, m;
	cin >> n >> m;
	
	Graph G(n);
	EdgeCapacityMap capacity = get(edge_capacity, G);
	ReverseEdgeMap rev_edge = get(edge_reverse, G);
	ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
	EdgeAdder ea(G, capacity, rev_edge);

	for(int i=0; i<m; i++){
		int u, v, c;
		cin >> u >> v >> c;
		ea.addEdge(u, v, c);
	}

	//briskw mia sta8erh arxh sthn tyxh. estw ton kombo 0. kai allazw
	//olous tous pi8anous proorismous. apo 1 ews (n-1) to sink.
	//gia ka8e zeugari source-target eksetazw kai to antistrofo dhladh flow me arxh to target kai 
	//destination to source. afoy o grafos mh symmetrikos. dhladh gia source dokimazw apo 1 ews n-1 
	//enw to target mou panta sta8ero sto 0.
	int source = 0;
	int best_sink = -1; //akyrh tim
	int best = INT_MAX; 
	bool reverse = false; 
	for(int sink=1; sink<n; sink++){
		long flow = push_relabel_max_flow(G, source, sink);
		if(flow < best){
			best = flow;
			best_sink = sink;
			reverse = false;		
		}
		//reverse check. me arxh to sink kai destination to source
		flow = push_relabel_max_flow(G, sink, source);
		if(flow < best){
			best = flow;
			best_sink = sink;
			reverse = true;
		}
	}

	//gia to best_sink ksanaypologizw to max-flow
	if(reverse==true)	std::swap(source, best_sink);	
	
	long min_flow = push_relabel_max_flow(G, source, best_sink);

	//bfs on residual graph to find S and V-S
	vector<bool> visited(n, false);
	std::queue<int> Q;
	Q.push(source);
	visited[source] = true;
	while(!Q.empty()){
		int u = Q.front();
		Q.pop();
		//for all children	
		graph_traits<Graph>::out_edge_iterator edge_i, edge_end;
		for(tie(edge_i, edge_end) = out_edges(u, G); edge_i != edge_end; ++edge_i){
			//do something (the vertices of the edge are *vert_i and target(*edge_i, g)
			//similarly the edge is *edge_i
			int v = target(*edge_i, G);
			if( visited[ v ]  ==false   &&   res_capacity[ *edge_i ] > 0 ){
				Q.push(v);
				visited[ v ]  = true;
			}
		}
	}

	vector<int> S;
	for(int i=0; i<n; i++){
		if(visited[i])		S.push_back(i);
	}
	std::sort(S.begin(), S.end());
	cout << min_flow << endl;
	cout << S.size();
	for(int s : S)
		cout << " " << s;
	cout << endl;

}


int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}


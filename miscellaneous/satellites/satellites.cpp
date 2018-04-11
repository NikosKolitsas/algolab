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
	int g, s, l;
	cin >> g >> s >> l;
	
	int n = g + s + 2;
	Graph G(n);
	EdgeCapacityMap capacity = get(edge_capacity, G);
	ReverseEdgeMap rev_edge = get(edge_reverse, G);
	ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
	EdgeAdder ea(G, capacity, rev_edge);
	
	//edges between left and right nodes
	for(int i=0; i<l; i++){
		int u, v;
		cin >> u >> v;
		ea.addEdge(u, g+v, 1);
	}

	int source = g+s;
	int sink = g+s+1;
	//connect nodes with source and sink
	for(int i=0; i<g; i++){
		ea.addEdge(source, i, 1);
	}
	for(int i=0; i<s; i++){
		ea.addEdge(g+i, sink, 1);
	}

	long flow = push_relabel_max_flow(G, source, sink);

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

	//to min vertex cover einai oi not visited toy aristerou epipedoy kai oi visited tou deksiou
	vector<int> left_nodes;
	vector<int> right_nodes;
	for(int i=0; i<g; i++){ //aristero epipedo
		if(visited[i] == false)		left_nodes.push_back(i);
	}
	for(int i=0; i<s; i++){ //deksio epipedo
		if(visited[g+i] == true)	right_nodes.push_back(i);
	}
	std::sort(left_nodes.begin(), left_nodes.end());
	std::sort(right_nodes.begin(), right_nodes.end());
	cout << left_nodes.size() << " " << right_nodes.size() << endl;
	for(int x : left_nodes)
		cout << x << " ";
	for(int x : right_nodes)
		cout << x << " ";
	cout << endl;

}


int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}










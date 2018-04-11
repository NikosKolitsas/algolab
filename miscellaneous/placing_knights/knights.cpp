#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore

using namespace std;
using namespace boost;

// Directed graph with int weights on edges.
typedef	adjacency_list<vecS, vecS, undirectedS,
		no_property,
		no_property >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef	graph_traits<Graph>::vertex_descriptor	VertexDescriptor;// Vertex type
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator



void testcase(){
	int n;
	cin >> n;

	vector<vector<int> > node_map(n,  vector<int>(n, -1));//-1 means holes
	int cnt = 0;	//node counter. counts non holes number
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			int temp;
			cin >> temp;
			if(temp == 1)		node_map[i][j] = cnt++;
		}		
	}
	//number of nodes == cnt.  node numbering is 0 ...to.. cnt-1
	
	Graph G(cnt);
	
	//diasxizw pinaka kai syndew ton grafo. 	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			//if is a hole
			if( node_map[i][j] == -1 )	continue;
			//diaforetika enwse to me ta parakatw
			if( i-2>=0  && j+1<n /*entos pinaka*/  
			&&    node_map[i-2][j+1] != -1  ){ /*not a hole*///create edge
				add_edge( node_map[i][j]  ,  node_map[i-2][j+1] , G );
			}

			if( i-1>=0  && j+2<n   
			&&    node_map[i-1][j+2] != -1  ){ 
				add_edge( node_map[i][j]  ,  node_map[i-1][j+2] , G );
			}

			if( i+1<n  && j+2<n   
			&&    node_map[i+1][j+2] != -1  ){ 
				add_edge( node_map[i][j]  ,  node_map[i+1][j+2] , G );
			}

			if( i+2<n  && j+1<n   
			&&    node_map[i+2][j+1] != -1  ){ 
				add_edge( node_map[i][j]  ,  node_map[i+2][j+1] , G );
			}
		}		
	}
	
	const VertexDescriptor NULL_VERTEX = graph_traits<Graph>::null_vertex();
	vector<VertexDescriptor> mate(cnt);
	edmonds_maximum_cardinality_matching(G, &mate[0]);

	int matches_counter = 0;
	for(unsigned int i = 0; i < cnt; i++)
		if(mate[i] != NULL_VERTEX && i < mate[i])
			matches_counter++;

	int max_independent_set = cnt - matches_counter;
	cout << max_independent_set << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
	return 0;
}


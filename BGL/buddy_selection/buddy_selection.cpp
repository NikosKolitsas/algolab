#include <climits>
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

void testcase();

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}

void testcase() {
	int V, c, f;
	cin >> V >> c >> f;
	Graph G(V);  

	//diabazw eisodo. ka8e student einai ena set apo hobbies
	vector<vector<string> > student_hobbies(V, vector<string>(c) );
	for(int i=0; i<V; ++i){	//gia ka8e student
		for(int j=0; j<c; j++){
			cin >> student_hobbies[i][j];
		}
	}

	//twra sygkrinw  dyadika olous tous student metaksy tous kai metrw omoiothtes
	for(int i=0; i<V; ++i){	//gia ka8e student
		for(int j=i+1; j<V; j++){ //sygkrish me ka8e allo ma8hth
			//kane tomh twn dyo set. dhladh ta koina stoixeia twn vector
			int intersection_size = 0;
			vector<string> v1=student_hobbies[i],  v2 = student_hobbies[j];
			//sygkrinw ena ena ta stoixeia tou v1 me ola ta stoixeia tou v2
			for(int ii=0; ii<c; ++ii){
				if (  find( v2.begin(), v2.end(), v1[ii])  !=  v2.end()  )
					intersection_size++;
			}

			if(intersection_size >= f+1)
				add_edge(i, j, G);
		}
	}

	//I have constructed the graph. there is edge only between nodes that share
	//at least f+1 common interests. Now do maximum matching algorithm.	
	const VertexDescriptor NULL_VERTEX = graph_traits<Graph>::null_vertex();
	vector<VertexDescriptor> mate(V);
	edmonds_maximum_cardinality_matching(G, &mate[0]);

	int matches_counter = 0;
	for(unsigned int i = 0; i < V; i++)
		if(mate[i] != NULL_VERTEX && i < mate[i])
			matches_counter++;

	if(matches_counter == V/2)	cout << "not optimal\n";	
	else 	cout << "optimal\n";
}





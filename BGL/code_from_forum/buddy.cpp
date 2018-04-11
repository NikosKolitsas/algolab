/* 
 * Algorithm Lab
 * Daniel Yu
 * Exercise 4 - Buddy
 */
 
#include <set>
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/graph_traits.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;


typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIterator;

typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::vertex_iterator VertexIterator;

void testcase(){
	// Optimization
	std::ios_base::sync_with_stdio(false);

	// Read n, c and f
	int n,c,f;
	cin >> n >> c >> f;

	// Read all the characterstics from each students
	vector<set<string> > studentPreferences(n);
	for (int i=0; i<n; ++i){
		for (int j=0; j<c; ++j){
			string preference;
			cin >> preference;
			studentPreferences[i].insert(preference);
		}
	}

	// Initialize the Graph
	Graph G(n);

	// Initialize
	int edge_count = 0;

	// Go through all possible edges
	for(int i=0; i<n; ++i){
		for(int j=i; j<n; ++j){
			// Calculate the number of common preferences
			set<string> sp1 = studentPreferences[i];
			set<string> sp2 = studentPreferences[j];
			set<string> common;

			set_intersection(sp1.begin(),sp1.end(),sp2.begin(),sp2.end(),
                  std::inserter(common,common.begin()));

			int n_common = common.size();

			// Check if the number of common preferences is bigger than f
			if (n_common > f){
				// if yes, insert it to the graph
				add_edge(i,j,G);
				edge_count++;
			}
		}
	}

	// Check if there is enough edges
	if (edge_count*2 < n){
		cout << "optimal" << endl; 
		return;
	}

	// Calculate the maximal cardinality matching
	vector<Vertex> mate(edge_count);
	edmonds_maximum_cardinality_matching(G, &mate[0]);

	// output the result
	if (matching_size(G, &mate[0]) * 2 == n)
		cout << "not optimal" << endl;
	else
		cout << "optimal" << endl;
}

int main (void) {
    // Optimization
    std::ios_base::sync_with_stdio(false);
 	
 	int t;
 	cin >> t;

 	while (t--) testcase();
 	
    return 0;
}


#include <climits>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore
#include <boost/function_output_iterator.hpp>

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
int my_set_intersection( std::set<string>::iterator, std::set<string>::iterator,
			std::set<string>::iterator, std::set<string>::iterator);

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
	vector<set<string> > student_hobbies(V);
	for(int i=0; i<V; ++i){	//gia ka8e student
		for(int j=0; j<c; j++){
			string hobby;
			cin >> hobby;
			student_hobbies[i].insert(hobby);
		}
	}

	//twra sygkrinw  dyadika olous tous student metaksy tous kai metrw omoiothtes
	//h tomh synolwn argh giati antigrafei ta koina stoixeia 
	//anti gia output iterator tou pernaw aplo counter
	int intersection_size = 0;
    	auto counter = [&intersection_size](string){ ++intersection_size; };  // C++11

	for(int i=0; i<V; ++i){	//gia ka8e student
		for(int j=i+1; j<V; j++){
			//kane tomh twn dyo set
			intersection_size = 0;
			set<string> s1=student_hobbies[i],  s2 = student_hobbies[j];
		/*	std::set_intersection(s1.begin(), s1.end(),
   						s2.begin(), s2.end(),
						boost::make_function_output_iterator(counter)  );
		*/	intersection_size = my_set_intersection( s1.begin(), s1.end(), s2.begin(), s2.end() );
			if(intersection_size >= f+1)
				add_edge(i, j, G);
		}
	}

/*		aplo set intersection
	std::vector<string> v_intersection;   
	for(int i=0; i<V; ++i){	//gia ka8e student
		for(int j=i+1; j<V; j++){
			//kane tomh twn dyo set
//			std::vector<string> v_intersection;	//mallon se ka8e loop mhdenizetai san initialization
			set<string> s1=student_hobbies[i],  s2 = student_hobbies[j];
//			v_intersection.clear();
			std::set_intersection(s1.begin(), s1.end(),
   						s2.begin(), s2.end(),
                          			std::back_inserter(v_intersection));
		
			if(v_intersection.size() >= f+1)
				add_edge(i, j, G);
		}
	}
*/	
	//I have constructed the graph. there is edge only between nodes that share
	//at least f+1 common interests. Now do maximum matching algorithm.	
	const VertexDescriptor NULL_VERTEX = graph_traits<Graph>::null_vertex();
	vector<VertexDescriptor> mate(V);
	edmonds_maximum_cardinality_matching(G, &mate[0]);

	int matches_counter = 0;
	for(unsigned int i = 0; i < V; i++)
		if(mate[i] != NULL_VERTEX && i < mate[i])
			matches_counter++;

//	cout << "mathces= " << matches_counter << "   and  V/2= " << V/2 << endl;
	if(matches_counter == V/2)	cout << "not optimal\n";	
	else 	cout << "optimal\n";
}



int my_set_intersection( std::set<string>::iterator first1, std::set<string>::iterator last1,
			 std::set<string>::iterator first2, std::set<string>::iterator last2){
	int intersection_counter = 0;
	while (first1!=last1 && first2!=last2){
    		if (*first1<*first2) ++first1;
    		else if (*first2<*first1) ++first2;
   	 	else {
        		intersection_counter++; ++first1; ++first2;
    		}
 	}
	return intersection_counter;
}



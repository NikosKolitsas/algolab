#include <climits>
#include <iostream>
#include <vector>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore

#include <boost/graph/copy.hpp>
#include <boost/graph/transpose_graph.hpp>

#include <boost/graph/graph_utility.hpp>

using namespace std;
using namespace boost;

// Directed graph with int weights on edges.
typedef	adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, int> >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef	graph_traits<Graph>::vertex_descriptor  VertexDescriptor;
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator
// Map edge -> weight.
typedef	property_map<Graph, edge_weight_t>::type	WeightMap;

void print_edges(Graph);

int main() {
	Graph G1(4);  
	Graph G2;
	Graph G_T;
	Graph G_merge;

	Graph G_loop;

	Edge e;
	WeightMap wm = get(edge_weight, G1);
	tie(e, tuples::ignore) = add_edge(0, 1, G1);
	wm[e] = 1;
	tie(e, tuples::ignore) = add_edge(0, 2, G1);
	wm[e] = 2;
	tie(e, tuples::ignore) = add_edge(1, 2, G1);
	wm[e] = 12;
	tie(e, tuples::ignore) = add_edge(1, 3, G1);
	wm[e] = 13;
//	print_graph(G1);	
	print_edges(G1);
	cout << endl;

	/*4 fores o G1*/
	for(int i=0; i<4; i++){
		copy_graph(G1, G_loop);
	}
	print_edges(G_loop);

	wm = get(edge_weight, G2);
	tie(e, tuples::ignore) = add_edge(0, 1, G2);
	wm[e] = 1;
	tie(e, tuples::ignore) = add_edge(1, 2, G2);
	wm[e] = 12;
	tie(e, tuples::ignore) = add_edge(2, 3, G2);
	wm[e] = 23;
//	print_graph(G2);	
	print_edges(G2);
	cout << endl;

	//copy G1 graph to G2 
  	typedef std::map<VertexDescriptor, VertexDescriptor> Map; 
       	Map m;
        associative_property_map<Map> pm(m); 
        copy_graph(G1, G2, orig_to_copy(pm)); 
  
//	print_graph(G2);	
	print_edges(G2);
	cout << endl;
	cout << "mapping from G1 vertices to G2:\n";
        for (Map::iterator p = m.begin(); p != m.end(); ++p) 
            std::cout << p->first << " -> " << p->second << std::endl;

	//copy_graph(G1, G2);
	//vector<VertexDescriptor> my_orig_to_copy(4);
	//copy_graph(G1, G2,    boost::orig_to_copy( &my_orig_to_copy[0] )   );



	//tranpose graph   the mapping is identical so no need for orig_to_copy 
  	transpose_graph(G1, G_T);
       	/*Map m2;
        associative_property_map<Map> pm2(m2); 
  	transpose_graph(G1, G_T,  orig_to_copy(pm2));
  	*/
  
	cout << "\nG_T:\n";
//	print_graph(G_T);	
	print_edges(G_T);
	cout << "\nG1 is unaffected:\n";
//	print_graph(G1);	
	print_edges(G1);

  	//print_graph(G_T, name_map);	
}



void print_edges(Graph G){
	cout << "\n\nprint_edges\n";
	WeightMap wm = get(edge_weight, G);

	EdgeIt ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei){
		Edge e = *ei;
    		cout << "(" << source(e, G) << ", " << target(e,G) << ")	" << "weight= " << wm[e] << endl;
	}
}























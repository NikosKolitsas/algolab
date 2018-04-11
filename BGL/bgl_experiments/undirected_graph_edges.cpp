#include <climits>
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>


using namespace std;
using namespace boost;

// Directed graph with int weights on edges.
typedef adjacency_list<vecS, vecS, undirectedS,
                no_property,
                property<edge_weight_t, int> >  Graph;
typedef graph_traits<Graph>::edge_descriptor    Edge;   // Edge type
typedef graph_traits<Graph>::vertex_descriptor  VertexDescriptor;
typedef graph_traits<Graph>::edge_iterator      EdgeIt; // Iterator
// Map edge -> weight.
typedef property_map<Graph, edge_weight_t>::type        WeightMap;


void print_edges(Graph);

int main() {
        Graph G1(4);
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
      print_graph(G1);        
        print_edges(G1);
        cout << endl;
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


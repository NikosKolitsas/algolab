#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/config.hpp>
#include <boost/graph/transpose_graph.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property,no_property > Graph;
typedef graph_traits<Graph>::vertex_descriptor 		Vertex;
typedef graph_traits<Graph>::edge_descriptor 		Edge;
typedef graph_traits<Graph>::vertices_size_type 	vertices_size_type;
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;

#define MAX_INT std::numeric_limits<unsigned int>::max()

void testcase();

int main() {
	ios_base::sync_with_stdio(false);
	int T;	// First input line: Number of testcases.
	cin >> T;
	while (T--) testcase();
}

void testcase(){
	int n, m;
	cin >> n >> m;
	int s, t;
	cin >> s >> t;
	

	Graph G(n);
	for(int i=0; i<m; i++){
		int si, ti;
		cin >> si >> ti;
		Edge e;
		tie(e, tuples::ignore) = add_edge(si, ti, G);	
	}

	//make predecessor and distance vectors for bfs
	//works with unsigned long, unsigned int, Vertex.
	//opwsdhpote omws unsigned!!!!
	vector<unsigned int> p( num_vertices(G) );
	p[s] = s;
	//vector< vertices_size_type > d( num_vertices(G), 0 );
	//works with unsigned long, unsigned int
	vector< unsigned int> d( num_vertices(G), 0 );
	
	breadth_first_search(G, s, 
   	    visitor(
		make_bfs_visitor(
    		  std::make_pair(record_distances(&d[0] , on_tree_edge())  ,
    			         record_predecessors(&p[0],  on_tree_edge()  ) ) ) ) );

	//elegxos an eftasa sto t. mporei na mhn yparxei kanena monopati.
	if(d[t] == 0 &&  s!=t ){
		cout << "no\n";
		return;
	}

	//briksw olous tous verices V opou mporei na loksodromhsei. kai tous apo8hkeyw ws true
	vector<bool> out_of_path_vertices(n, false);
	//traverse the path. start from predecessor of target  mexri kai ton start. 
	//at each vertice I check for out edges
	for(unsigned int current = p[t];  true; current = p[current]){ 
		OutEdgeIt edge_i, edge_end;		
		for(tie(edge_i, edge_end) = out_edges( current, G); edge_i != edge_end; ++edge_i){
			out_of_path_vertices[  target(*edge_i, G) ] = true;
		}
		if(current == s) break; //exw ftasei kai eksetasei hdh ton start ara feygw
	}
	//now all the vertices on path from s to t are also true. so I make them again false
	for(unsigned int current = t;  true; current = p[current]){ 
		out_of_path_vertices[current] = false;	
		if(current == s) break; //exw ftasei kai eksetasei hdh ton start ara feygw
	}


	//now i construct the Transpose graph and do a BFS starting from t
	Graph G_T;
	transpose_graph(G, G_T);
	vector< unsigned int> d_T( num_vertices(G), 0 );
	
	breadth_first_search(G_T, t, 
   	    visitor(
		make_bfs_visitor(
    		  record_distances(&d_T[0] , on_tree_edge())  ) ) );

	//loop ston pinaka out_of_path_vertices otan exei timh true dhladh pi8anh loksodromhsh
	//koitaw to mhkos = bfs_dist_G(s -> V) + bfs_dist_G_T(t -> V)
	//krataw to mikrotero apo ola ayta
	unsigned int next_path = MAX_INT;
	for(int i=0; i<n; i++){
		if(out_of_path_vertices[i] == true &&  d_T[i]!=0 ){
			unsigned int new_dist = d[i] + d_T[i];
			if( new_dist < next_path )
				next_path = new_dist;
		}
	}

	if(next_path != MAX_INT ) //dhladh ontws yparxei next_path
		cout << next_path << endl;
	else 	cout << "no\n";

}//telos testcase



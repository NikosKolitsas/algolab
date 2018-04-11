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
	
	//so they are zero based
	s -= 1;
	t -= 1;

	Graph G(n);
	for(int i=0; i<m; i++){
		int si, ti;
		cin >> si >> ti;
		Edge e;
		tie(e, tuples::ignore) = add_edge(si-1, ti-1, G);	
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


	//now i construct the Transpose graph and do a BFS starting from t
	Graph G_T;
	vector<unsigned int> p_T( num_vertices(G) );
	p_T[t] = t;
	transpose_graph(G, G_T);
	vector< unsigned int> d_T( num_vertices(G), 0 );
	
	breadth_first_search(G_T, t, 
   	    visitor(
		make_bfs_visitor(
    		  std::make_pair(record_distances(&d_T[0] , on_tree_edge())  ,
    			         record_predecessors(&p_T[0],  on_tree_edge()  ) ) ) ) );


	//to next path mporei na akolou8hsei ena opoiodhpote allo monopati. estw mesw tou v. 
	//arkei/o v na einai prosbasimos apo thn bfs_G(s->v) dhladh d[v]!=0 
	//kai antistoixa prosbashmos apo thn bfs_G_T(t->v). a8roizw ta mhkh kai krataw to 
	//mikrotero apo ola. den lambanw omws ypopshn osa vertices anoikoyn hdh sto 
	//sortest path giati tote den einai loksodromhsh alla to idio monopati.
	//markarw ta vertices tou shortest path me false.
	vector<bool> out_of_path_vertices(n, true);
	for(unsigned int current = t;  true; current = p[current]){ 
		out_of_path_vertices[current] = false;	
		if(current == s) break; //exw ftasei kai eksetasei hdh ton start ara feygw
	}

	unsigned int min_path, min_index; 
	tie(min_path, min_index) = next_path(out_of_path_vertices, d, d_T);
	if(min_index==-1) 	cout << "no\n";
	else if(s==t){
		Edge e;
		bool exist;
		tie(e,exist) = edge(s,s,G); //test if self loop
		if(exist == false){ //no self loop so we must do the test again
			//make all the predecessors of min_index false both in G and G_T
			for(int v=min_index; true; v=p[v]){
				
			}
		}
	}
	

	//edw eksetazw mia idiazoysa periptwsh. thn periptwsh pou s==t
	//an s==t kai yparxei kai self-loop dhladh edge(s,s) tote synexisw kanonika afou
	//to next_path pou exw brei einai to syntomotero meta tou path: s,s
	//an omws den yparxei self loop tote prepei na brw dyo monopatia

	if(next_path != MAX_INT ) //dhladh ontws yparxei next_path
		cout << next_path << endl;
	else 	cout << "no\n";

}//telos testcase


void invalidate_predecessor_path(vector<bool>& valid_vertices, unsigned int s, unsigned int t)


//epistrefw to mhkos tou path kai ton xarakthristiko kombo
pair<unsigned int, unsigned int> next_path(vector<bool>& out_of_path_vertices, 
				vector<unsigned int>& d,   vector<unsigned int>& d_T){
	//loop ston pinaka out_of_path_vertices otan exei timh true dhladh pi8anh loksodromhsh
	//koitaw to mhkos = bfs_dist_G(s -> V) + bfs_dist_G_T(t -> V)
	//krataw to mikrotero apo ola ayta
	unsigned int min_path = MAX_INT;
	unsigned int min_index = -1;
	for(int i=0; i<n; i++){
		if(out_of_path_vertices[i] == true && d[i]!=0 &&  d_T[i]!=0 ){
			unsigned int new_dist = d[i] + d_T[i];
			if( new_dist < min_path ){
				min_path = new_dist;
				min_index = i;
			}
		}
	}
	return make_pair(min_path, min_index);
}


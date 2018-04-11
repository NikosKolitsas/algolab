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

#define MAX_INT std::numeric_limits<int>::max()

void testcase();
void invalidate_predecessor_path(vector<bool>&, vector<int>&, int, int);
pair<int, int> next_path(vector<bool>&, vector<int>&,  vector<int>&);
bool path_contains_self_loops(Graph&, vector<int>&, int, int);

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
//	s -= 1;
//	t -= 1;

	Graph G(n);
	for(int i=0; i<m; i++){
		int si, ti;
		cin >> si >> ti;
		Edge e;		//zero based
	//	tie(e, tuples::ignore) = add_edge(si-1, ti-1, G);	
		tie(e, tuples::ignore) = add_edge(si, ti, G);	
	}

	//make predecessor and distance vectors for bfs
	//works with unsigned long, unsigned int, Vertex.
	//opwsdhpote omws unsigned!!!!
	vector<int> p( num_vertices(G) );
	p[s] = s;
	//vector< vertices_size_type > d( num_vertices(G), 0 );
	//works with unsigned long, unsigned int
	vector< int> d( num_vertices(G), 0 );
	
	breadth_first_search(G, s, 
   	    visitor(
		make_bfs_visitor(
    		  std::make_pair(record_distances(&d[0] , on_tree_edge())  ,
    			         record_predecessors(&p[0],  on_tree_edge()  ) ) ) ) );


	//now i construct the Transpose graph and do a BFS starting from t
	Graph G_T;
	vector<int> p_T( num_vertices(G) );
	p_T[t] = t;
	transpose_graph(G, G_T);
	vector< int> d_T( num_vertices(G), 0 );
	
	breadth_first_search(G_T, t, 
   	    visitor(
		make_bfs_visitor(
    		  std::make_pair(record_distances(&d_T[0] , on_tree_edge())  ,
    			         record_predecessors(&p_T[0],  on_tree_edge()  ) ) ) ) );

	//arxizoyn oi elegxoi
	//elegxos an eftasa sto t. mporei na mhn yparxei kanena monopati.
	if(d[t] == 0 &&  s!=t ){
		cout << "no\n";
		return;
	}
	//to next path mporei na akolou8hsei ena opoiodhpote allo monopati. estw mesw tou v. 
	//arkei/o v na einai prosbasimos apo thn bfs_G(s->v) dhladh d[v]!=0 
	//kai antistoixa prosbashmos apo thn bfs_G_T(t->v). a8roizw ta mhkh kai krataw to 
	//mikrotero apo ola. den lambanw omws ypopshn osa vertices anoikoyn hdh sto 
	//sortest path giati tote den einai loksodromhsh alla to idio monopati.
	//markarw ta vertices tou shortest path me false.
	vector<bool> out_of_path_vertices(n, true);
	int min_path = d[t];
	int second_path, second_index; 
	int third_path, third_index; 

	invalidate_predecessor_path(out_of_path_vertices, p, s, t);
	tie(second_path, second_index) = next_path(out_of_path_vertices, d, d_T);


	if(s!=t){
		if(path_contains_self_loops(G, p, s, t)  ){
			second_path = min( min_path+1, second_path );
		} 
	} //s==t
	else if(s==t && path_contains_self_loops(G, p, s, s) ){ 
		//follow the self loop twice. so second_path always =2
		cout << "2\n";
		return;
	}
	//edw eksetazw mia idiazoysa periptwsh. thn periptwsh pou s==t
	//kai den yparxei kai self-loop dhladh edge(s,s) 
	//opote an to second path exei self loop
	else if(s==t && second_index!=-1 && 
		!path_contains_self_loops(G, p, s, s) ) { 
		//!=-1 shmainei oti yparxei kyklos s->...min_index..->s
		//opote psaxnw h gia self loop tou second path h gia third path
		//make all the predecessors of min_index false both in G and G_T
		invalidate_predecessor_path(out_of_path_vertices, p, s, second_index);
		invalidate_predecessor_path(out_of_path_vertices, p_T, s, second_index);
		tie(third_path, third_index) = next_path(out_of_path_vertices, d, d_T);
		if(path_contains_self_loops(G, p, s, second_index) ||
			path_contains_self_loops(G, p_T, s, second_index) ){
			second_path = min( second_path+1,  third_path);
		}else	//kanw dyo fores ton kyklo second_path
			second_path = min( 2*second_path,  third_path);
	}
	
	if(second_path == MAX_INT) 	cout << "no\n";
	else				cout << second_path << endl;

}//telos testcase


void invalidate_predecessor_path(vector<bool>& valid_vertices, vector<int>& p, int s, int t){
	for(int current = t;  true; current = p[current]){ 
		valid_vertices[current] = false;	
		if(current == s) break; //exw ftasei kai eksetasei hdh ton start ara feygw
	}
}

bool path_contains_self_loops(Graph& G, vector<int>& p, int s, int t){
	Edge e;		 
	bool exist;
	for(int u=t;  true;  u=p[u]){ 
		tie(e,exist) = edge(u,u,G); //test if self loop
		if(exist)	return true;
		if(u == s) 	break; 
	}
	return false;
}

//epistrefw to mhkos tou path kai ton xarakthristiko kombo
pair<int, int> next_path(vector<bool>& out_of_path_vertices, 
				vector<int>& d,   vector<int>& d_T){
	//loop ston pinaka out_of_path_vertices otan exei timh true dhladh pi8anh loksodromhsh
	//koitaw to mhkos = bfs_dist_G(s -> V) + bfs_dist_G_T(t -> V)
	//krataw to mikrotero apo ola ayta
	int n = out_of_path_vertices.size();
	int min_path = MAX_INT;
	int min_index = -1;
	for(int i=0; i<n; i++){
		if(out_of_path_vertices[i] == true && d[i]!=0 &&  d_T[i]!=0 ){
			int new_dist = d[i] + d_T[i];
			if( new_dist < min_path ){
				min_path = new_dist;
				min_index = i;
			}
		}
	}
	return make_pair(min_path, min_index);
}


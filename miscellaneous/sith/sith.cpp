#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

//BOOST
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

// unDirected graph .
typedef	adjacency_list<vecS, vecS, undirectedS,
		no_property,
		no_property >	Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef	graph_traits<Graph>::vertex_descriptor	VertexDescriptor;// Vertex type
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator



//CGAL proximity
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>	    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> 			    Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;
typedef Triangulation::Vertex_handle	     Vertex_handle;

typedef K::Point_2 P;
typedef K::Segment_2 S;

int DEBUG=0;
void print_vector(vector<int>&);

//eksetazw an ta points [k....n-1] exoun component megethous k
//k einai to mid ths dyadikhs anazhthshs
bool is_solvable( vector< std::pair<P,int> >& pts, long r2, int k){
	int n = pts.size();
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin()+k, pts.end());

	//construct boost graph
	//olous tous kombous tous bazw label ton arithmo tous meion k. wste na arxizoyn apo 0 ews n-k-1
	Graph G(n-k);
	//iterate over all Delaunay edges. if square_length <= r^2 then we add them to the boost graph 
	// we construct  T= G tomh D
	for (Edge_iterator eit = t.finite_edges_begin();  eit != t.finite_edges_end(); ++eit){
		//K::FT edge_length = t.segment(*eit).squared_length() ;
		Triangulation::Vertex_handle v1 = eit->first->vertex((eit->second + 1) % 3);
		Triangulation::Vertex_handle v2 = eit->first->vertex((eit->second + 2) % 3);
		K::FT edge_length = CGAL::squared_distance(v1->point(), v2->point());
		if(edge_length <= r2)		
			add_edge( v1->info() -k , v2->info() -k ,  G);
	}

	//we compute connected components
	vector<int> component( n-k );
    	int num = connected_components(G, &component[0]);	

	//we compute the size of each component. we are interested in the biggest
	vector<int> component_size(num,0);	//arxika ola ta component exoun 0 vertices
	for(int i=0; i<n-k; i++){
		component_size[ component[i]  ]++;
	}
	int max_comp_size = *( max_element(component_size.begin(),   component_size.end())  );

	if(DEBUG){
		cout << "\n\nk= " << k << endl;
		cout << "num=  " << num << endl;
		cout << "component\n";
		print_vector(component);
		cout << "component_size\n";
		print_vector(component_size);
		cout << "max_comp_size= " << max_comp_size << endl;
		if(max_comp_size >= k)		cout << "max_comp_size >= k \n";
		else				cout << "max_comp_size <  k\n";
	}

	if(max_comp_size >= k)		return true;
	else				return false;
}

//kanei mono binary search tipota allo. epistrefei to megalytero k opou dinei yes. yparxei dhladh cc megethous k
//gia ka8e k pou eksetazw lambanw ypopsin mono ta shmeia [k,n-1]
int binary_search( vector< std::pair<P,int> >& pts, long r2){
	//dyadikh anazhthsh
	int n = pts.size();
	int lo = 0;
	int hi = n/2 + 1;
	int mid;

	while(lo < hi){
		mid = lo + (hi-lo+1)/2 ;    //it rounds up
		if( is_solvable(pts, r2, mid) == true )
			lo = mid;
		else
			hi = mid-1;
	}

	//panta yparxei toylaxiston mia lysh opote den eksetazw an ontws to lo einai lysh
	return lo;         // lo is the greatest R for which is_solvable is true
}

void testcase(){
	long n,r;
	cin >> n >> r;
	long r2 = r*r;
	
	vector< std::pair<P,int> > pts(n);
	for(int i=0; i<n; i++){
		int x,y;
		cin >> x >> y;
		//twra ka8e vertex tou Delaunay 8a exei id ton int.
		pts[i] =  std::make_pair(  P(x,y) , i) ;
	}

	cout << binary_search(pts, r2) << endl;
}


int main(){
	ios_base::sync_with_stdio(false);

/*	std::cout << "max(int) = " << std::numeric_limits<int>::max() << endl;
	std::cout << "max(long) = "<< std::numeric_limits<long>::max() << endl;
	std::cout << "max(long long) = "<< std::numeric_limits<long long>::max() << endl;
	std::cout << "max(double) = "<< std::numeric_limits<double>::max() << endl;    

	max(int) = 2147483647
	max(long) = 2147483647
	max(long long) = 9223372036854775807
	max(double) = 1.79769e+308
*/
	int T;
	cin >> T;
	while(T--){	
		testcase();
	}
	return 0;
}

void print_vector(vector<int>& v){
	for(int x : v)
		cout << x << ", ";

	cout << endl; 
}

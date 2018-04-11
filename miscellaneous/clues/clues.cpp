#include <iostream>
#include <algorithm>
#include <vector>

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
typedef Triangulation::Vertex_handle				    Vertex_handle;

typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

typedef K::Point_2 P;
typedef K::Segment_2 S;

//if there is a pair of vertices that have square distance <= r2 then return false. else return true
bool check_no_edges( vector< std::pair<P,int> >& station,
		     vector<default_color_type>& color_partition, double r2, string color){
	
	int n = station.size();
	default_color_type test_color;
	if(color=="white")	test_color =  color_traits<default_color_type>::white();
	else			test_color =  color_traits<default_color_type>::black();
	//kanw iterate se ola ta shmeia kai dialegw mono ta aristera h mono ta deksia.
	//ton kanw ton pinaka megethos n an kai einai sigoyra <=
	vector<P> partition;
	partition.reserve(n);
	for(int i=0; i<n; i++){
		if( color_partition[i]==test_color  ) //idio xrwma me ayto pou psaxnw
			partition.push_back( station[i].first  );
	}

	// construct triangulation
	Triangulation t;
	t.insert(partition.begin(), partition.end());

	//construct boost graph
//	Graph G( station.size()  );
	//iterate over all Delaunay edges. if square_length <= r^2 then return false.
	//if no such an edge exists then return true 
	for (Edge_iterator eit = t.finite_edges_begin();  eit != t.finite_edges_end(); ++eit){
		K::FT edge_length = t.segment(*eit).squared_length() ;
		if(edge_length <= r2)		
			return false;
	}
	return true;

}

void testcase(){
	int n,m,r;
	cin >> n >> m >> r;
	
	vector< std::pair<P,int> > station(n);
	for(int i=0; i<n; i++){
		int x,y;
		cin >> x >> y;
		//twra ka8e vertex tou Delaunay 8a exei id ton int.
		station[i] =  std::make_pair(  P(x,y) , i) ;
	}

		// construct triangulation
	Triangulation t;
	t.insert(station.begin(), station.end());

	//construct boost graph
	Graph G(n);
	double r2 = pow(r,2);
	//iterate over all Delaunay edges. if square_length < r^2 then we add them to the boost graph 
	// we construct  T= G tomh D
	for (Edge_iterator eit = t.finite_edges_begin();  eit != t.finite_edges_end(); ++eit){
		//K::FT edge_length = t.segment(*eit).squared_length() ;
		Triangulation::Vertex_handle v1 = eit->first->vertex((eit->second + 1) % 3);
		Triangulation::Vertex_handle v2 = eit->first->vertex((eit->second + 2) % 3);
		K::FT edge_length = CGAL::squared_distance(v1->point(), v2->point());
		if(edge_length <= r2)		
			add_edge( v1->info() , v2->info() ,  G);
	}

	//boost test if T  is bipartite.
	vector<default_color_type> partition(n);
	bool bipartite_flag = is_bipartite ( G,  get(vertex_index, G) , &partition[0]);

	//if(bipartite_flag==false) //neither G is bipartite. print n for all clues
	if(bipartite_flag==true){ //verify that G is also bipartite
		//check that no edges between nodes from the left partition. no monochromatic edges
		//returns true if no edges. false if exist
		bool left_part = check_no_edges( station, partition, r2, "white");
		bool right_part = check_no_edges( station, partition, r2, "black");

		if( !left_part || !right_part  )	bipartite_flag = false;

	}
	//now we know if G truely bipartite or not
	//now we compute the routing as previous  erwthamta

	//we compute connected components
		
	vector<int> component( n );
    	connected_components(G, &component[0]);	

	//for every clue. check if a and b connect to the same component.
	//which means the nearest vertext of a and the nearest vertex of b are in the same component
	for(int i=0; i<m; i++){
		int ax, ay, bx, by;
		cin >> ax >> ay >> bx >> by;

		if(bipartite_flag==false){
			cout << "n";
			continue;
		}
	
		P pa(ax,ay);
		P pb(bx,by);
		if( CGAL::squared_distance( pa ,  pb) <= r2){
			//einai konta ara epikoinwnoun apey8eias. xwris na xrhsimopoioyn diktyo.
			cout << "y";
			continue;
		}

		Vertex_handle na = t.nearest_vertex( pa );	
		Vertex_handle nb = t.nearest_vertex( pb );	
		if( CGAL::squared_distance( pa ,  na->point() ) > r2
		   ||  CGAL::squared_distance( pb ,  nb->point() ) > r2){
			//to a h to b apexei poly apo to diktyo ara den syndeontai kan me ayto.
			cout << "n";
			continue;
		}

		int a_component = component[ na->info()  ];
		int b_component = component[ nb->info()  ];
		if(a_component==b_component)	cout << "y";
		else				cout << "n";
		
	}
	cout << endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--){	
		testcase();
	}
	return 0;
}

#include <iostream>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;
typedef K::Point_2 P;

//https://judge.inf.ethz.ch/doc/cgal/doc_html/Spatial_sorting/Spatial_sorting_2sp_sort_using_property_map_2_8cpp-example.html#a4
//https://judge.inf.ethz.ch/doc/cgal/doc_html/Spatial_sorting/Spatial_sorting_2sp_sort_using_property_map_2_8cpp-example.html#a4
#include <CGAL/spatial_sort.h>
#include <CGAL/Spatial_sort_traits_adapter_2.h>
#include <CGAL/property_map.h>
typedef CGAL::Spatial_sort_traits_adapter_2<K, 
  CGAL::First_of_pair_property_map< std::pair<P,int>  >
> Search_traits_2;


int main(){
   ios_base::sync_with_stdio(false);
   std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
   while(true){
	// read number of points
	std::size_t n;
	std::cin >> n;
        if(n==0)  break;        //telos

	// read points
	//on each loop this vector is reinitialized (like calling the default constructor at each loop)
	vector<P> pts;
	pts.reserve(n);
	for (std::size_t i = 0; i < n; ++i) {
		int x,y;
		cin >> x >> y;
		pts.push_back( P(x,y) );
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());

	//for every query point- calculate its distance from the nearest old point
	//use CGAL::spatial_sort() to make it faster
	int m;
	cin >> m;
	std::vector< pair<P,int> > query_pts;
	query_pts.reserve(m);
	for(int i =0; i<m; i++){
		int x,y;
		cin >> x >> y;
		query_pts.push_back( std::make_pair(P(x,y) , i ) );
	}
	Search_traits_2 traits;
	CGAL::spatial_sort(query_pts.begin(), query_pts.end(), traits);
	//twra ta erwthmata mperdemena px   (P,6), (P,3)
	//sorted_output[4] apanta to erwthma 5
	vector<K::FT> sorted_output(m); //for each query output distance from nearest vertex
	for(int i =0; i<m; i++){
		Triangulation::Face_handle hint;
		P q = query_pts[i].first;
    		Triangulation::Vertex_handle v = t.nearest_vertex( q , hint );
		hint = v->face();

		K::FT nearest_dist = CGAL::squared_distance(v->point(),  q );
		sorted_output[  query_pts[i].second  ]  =  nearest_dist;
	}

	//print all output in order
	//without decimal digits
	for(K::FT x : sorted_output)
		cout << CGAL::to_double(x) << endl;
	
   }
}



















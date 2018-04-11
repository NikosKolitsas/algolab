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

int main(){
   std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
   while(true){
	// read number of points
	std::size_t n;
	std::cin >> n;
        if(n==0)  break;        //telos

	// read points
	//on each loop this vector is reinitialized (like calling the default constructor at each loop)
	std::vector<K::Point_2> pts;
	pts.reserve(n);
	for (std::size_t i = 0; i < n; ++i) {
		K::Point_2 p;	
		std::cin >> p;
		pts.push_back(p);
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());

	//for every new point- calculate its distance from the nearest old point
	int m;
	cin >> m;
	for(int i =0; i<m; i++){
		K::Point_2 p;	
		std::cin >> p;
		Triangulation::Vertex_handle v = t.nearest_vertex(p);		

		K::FT nearest_dist = CGAL::squared_distance(v->point(), p);
		//without decimal digits
 		cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) 
			<< CGAL::to_double(nearest_dist) << endl;
	}
   }
}



















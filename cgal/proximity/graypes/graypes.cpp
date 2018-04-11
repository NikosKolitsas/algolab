#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

double ceil_to_double(const K::FT&);

int main(){
   std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
   while(true){
	// read number of points
	std::size_t n;
	std::cin >> n;
        if(n==0)  break;        //telos

	// read points
	std::vector<K::Point_2> pts;
	pts.reserve(n);
	for (std::size_t i = 0; i < n; ++i) {
		//an den htan inexact construction kernel tote diabasma me int gia taxythta
		K::Point_2 p;	
		std::cin >> p;
		pts.push_back(p);
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());

	//Delaunay triangulation includes always the nearest neighbor of every point
	//so we are looking for the shortest edge of the triangulation
	K::FT min_dist = std::numeric_limits<double>::max();
	for (Edge_iterator eit = t.finite_edges_begin();  eit != t.finite_edges_end(); ++eit){
		// get the vertices of e
		Triangulation::Edge e = *eit;
		Triangulation::Vertex_handle v1 = e.first->vertex((e.second + 1) % 3);
		Triangulation::Vertex_handle v2 = e.first->vertex((e.second + 2) % 3);
		K::FT current_dist = CGAL::squared_distance(v1->point(), v2->point());
		if( current_dist < min_dist)
			min_dist = current_dist; 	
	}
	//sqrt of min_dist. then divide it with 2. and then mult with 100 for output
	min_dist = sqrt(min_dist);
	min_dist = min_dist*50;  // * 50= *100/2
	std::cout << ceil_to_double(min_dist) << endl;
   }
}


double ceil_to_double(const K::FT& x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}





















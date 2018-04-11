#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertices_iterator;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Edge  Edge;


double time_from_distance(K::FT&);
double ceil_to_double(const K::FT&);

int main(){
   ios_base::sync_with_stdio(false);
   std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
   while(true){
	// read number of points
	std::size_t n;
	std::cin >> n;
        if(n==0)  break;        //telos

	int l, b, r, top;
	cin >> l >> b >> r >> top;

	// read points
	//on each loop this vector is reinitialized (like calling the default constructor at each loop)
	std::vector<K::Point_2> pts;
	pts.reserve(n);

	for (std::size_t i = 0; i < n; ++i) {
		int x, y;	
		cin >> x >> y;
		pts.push_back(K::Point_2(x,y));
	}

	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());

	//here i store the min dist from obstacle for every vertex 
	vector<K::FT> nearest_obstacle_dist;
	nearest_obstacle_dist.reserve(n);
	//for every point- calculate its distance from the nearest other point
	//and compare with the distance from the walls
	for(Vertices_iterator vit = t.finite_vertices_begin();  vit != t.finite_vertices_end(); ++vit){
		Vertex_handle v1 = vit;
		//measure distance from the 4 walls. keep the smallest. then store the value: 4 * d^2
		double x = v1->point().x();
		double y = v1->point().y();

		double dwall = min(    min(x-l, r-x)  ,   min(top-y,  y-b)    );
		K::FT min_dist = 4 * dwall * dwall;

		// find all finite incident vertices to v
		Triangulation::Vertex_circulator c = t.incident_vertices(v1);
		do {	
			if(c==0) break; //this vertice is alone (with no incident vertices)
			if (t.is_infinite(c)) continue; //infinite vertex 
			Vertex_handle v2 = c;
			K::FT current_dist = CGAL::squared_distance(v1->point(), v2->point());
				
			if( current_dist < min_dist)
				min_dist = current_dist; 	
		} while (++c != t.incident_vertices(v1));

		//now we have found the min dist for an obstacle for this vertex
		//store it in a vector
		nearest_obstacle_dist.push_back(min_dist);
	}
	
	vector<K::FT>& v = nearest_obstacle_dist; //alias just for shortname
	vector<K::FT>::iterator minIt, maxIt; 
	tie(minIt, maxIt) = std::minmax_element(v.begin(), v.end());
	double first =  time_from_distance( *minIt  );
	double last  =  time_from_distance( *maxIt );
	//median
	std::nth_element(v.begin(), v.begin() + v.size()/2, v.end());
	K::FT dstored = v[v.size()/2];
	double median =  time_from_distance(dstored);

	cout << first << " " << median << " " << last << endl;
   }//end of while
}//end of main

double time_from_distance(K::FT& d){
	K::FT temp = sqrt( sqrt(d/4) - 0.5 );
	return ceil_to_double(temp);
}

double ceil_to_double(const K::FT& x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

















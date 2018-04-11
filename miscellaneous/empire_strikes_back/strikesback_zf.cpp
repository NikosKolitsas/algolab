#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


//for proximity and Delaunay
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

typedef K::Point_2 P;
typedef K::Segment_2 S;


//for linear programming:
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif
// program and solution types
typedef CGAL::Quadratic_program<K::FT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

//debugging
template class vector<vector<K::FT> >;

void testcase(){
	int a,s,b;
	cin >> a >> s >> b;

	int e;
	cin >> e;
	
	vector<P> asteroids(a);
	vector<int> density(a);
	vector<P>  hunters(b);
	vector<P>  shots(s);
	//read all asteroids a
	for(int i=0; i<a; i++){
		int x, y, d;
		cin >> x >> y >> d;
		
		asteroids[i] = P(x,y);
		density[i] = d;
	}
	//read all shots  s
	for(int i=0; i<s; i++){
		int x, y;
		cin >> x >> y;
		
		shots[i] = P(x,y);
	}
	//read all bounty hunters b
	for(int i=0; i<b; i++){
		int x, y;
		cin >> x >> y;
		
		hunters[i] = P(x,y);
	}

	//end of input

	//Delaunay stous bounty hunters
	// construct triangulation
	Triangulation t;
	if(b > 0){ 
	  t.insert(hunters.begin(), hunters.end());
	}

	// by default, we have a nonnegative QP with Ax >= b
  	Program qp (CGAL::LARGER, true, 0, false, 0);  // ola ta energy shots >=0, no upper bound
	//for each shot find the nearest hunter => the maximum radius	
	// and then all the asteroids that are affected by it.
	for(int i=0; i<s; i++){
		bool infinite_circle = false;
		K::FT max_radius = DBL_MAX;
		Triangulation::Vertex_handle v;
		if(b==0){	//oxi hunters=> infinite radius=> incide circle
			infinite_circle = true; 
		}else{
			v = t.nearest_vertex( shots[i]  );		

			max_radius = CGAL::squared_distance( v->point(), shots[i] );		
		}
		//test every asteroid if inside circle.
		for(int j=0; j<a; j++){
			K::FT dist = CGAL::squared_distance(  shots[i] ,  asteroids[j] );		
			if( infinite_circle || dist < max_radius){//if not in circle. do not set value. default is zero!
				//arg1=variable_num, arg2=constr_num, arg3=value
				qp.set_a( i ,  j ,  1.0/max(1.0, dist)  );
			}
		}
	}

	//for every asteroid set B matrix.	
	for(int j=0; j<a; j++){
		qp.set_b( j , density[j] );	
	}

	//now set constraint of total energy. constraint number a
	for(int i=0; i<s; i++){
		qp.set_a( i ,  a ,  1);
	}
	qp.set_b( a , e );	
	qp.set_r( a, CGAL::SMALLER);  // <= e

		
	Solution qp_sol = CGAL::solve_quadratic_program(qp, ET());
  //	assert (qp_sol.solves_quadratic_program(qp));

  	// output
  	if (qp_sol.is_infeasible()) {
    		std::cout << "n\n";
  	} else if(qp_sol.is_unbounded()){	//den prepei na ftasei pote edw
    		std::cout << "unbounded\n";
  	}else if(qp_sol.is_optimal()){
    		cout << "y\n";
  	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}


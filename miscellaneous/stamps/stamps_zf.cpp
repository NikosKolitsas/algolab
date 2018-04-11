#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <stdexcept>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
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
//dokimase to kai me Gmpzf ET;   mallon 8a leitoyrgei kai den xreiazetai meta na dhlwnw K::FT
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
	int lamps_num, stamps_num, walls_num;
	cin >> lamps_num >> stamps_num >> walls_num;

	vector<P> stamps(stamps_num);
	vector<int> M(stamps_num);	//maximum light

	vector<P> lamps(lamps_num);
	vector<S> walls(walls_num);

	for(int i=0; i<lamps_num; i++){
		int x,y;
		cin >> x >> y;
				
		lamps[i] = P(x,y);
	}

	for(int i=0; i<stamps_num; i++){
		int x,y, max_light;
		cin >> x >> y >> max_light;
				
		stamps[i] = P(x,y);
		M[i] = max_light;
	}

	for(int i=0; i<walls_num; i++){
		int a,b,c,d;
		cin >> a >> b >> c >> d;
	
		walls[i] =  S(   P(a,b)     ,  P(c,d)   );			
	}



	//diabasa ola to input.
	//ypologizw thn epidrash tou ka8e lamp se ka8e stamp. kai elegxw an yparxei wall endiamesa
	vector<vector<K::FT> > effect(stamps_num,    vector<K::FT>(lamps_num)   );
	//gia ka8e zeygari   lampas-grammatoshmou    koitaw an parembaletai estw kai enas toixos
	for(int s=0; s<stamps_num; s++){
		for(int l=0; l<lamps_num; l++){
			S my_seg( stamps[s]  ,  lamps[l]   );
			//an estw kai enas toixos endiamesa. tote effect=0
			bool visible = true;
			for(S wall : walls){
				if(  CGAL::do_intersect( my_seg   ,  wall  ) ){
					visible = false;	//empodio endiamesa
					break;
				}
			}
			if(visible)	effect[s][l] = 1.0 / my_seg.squared_length();
			else		effect[s][l] =0;
		}
	}


	//linear programming twra

	// by default, we have a nonnegative LP with Ax <= b
	//lower bound to 1. ka8ws light intensity apokleietai arnhtikh
	//upper bound den yparxei ara false
	Program qp (CGAL::SMALLER, true, 1, true, pow(2,12));
  	// constraints:
  	for(int s=0; s<stamps_num; s++){
		for(int l=0; l<lamps_num; l++){	/*metablhth-sthlh l, periorismos-grammh s*/
			qp.set_a( l,  2*s, effect[s][l]);  //intensity <=M
			qp.set_a( l,  2*s+1, effect[s][l]);	//intensity >=1
		}
		qp.set_b(  2*s ,  M[s] ); //default  ...<= M
		qp.set_b(  2*s+1,  1 );   // >=1
		qp.set_r(  2*s+1,  CGAL::LARGER);
  	} //telos twn periorismwn
	for(int l=0; l<lamps_num; l++)
		qp.set_c(l, 1);

	Solution s = CGAL::solve_quadratic_program(qp, ET());
  	assert (s.solves_quadratic_program(qp));

  	// output
  	if (s.is_infeasible()) {
    		std::cout << "no\n";
  	} else if(s.is_unbounded()){	//den prepei na ftasei pote edw
    		std::cout << "unbounded\n";
  	}else if(s.is_optimal()){
    		cout << "yes\n";
  	}
//	cout << s << endl;
}


int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}


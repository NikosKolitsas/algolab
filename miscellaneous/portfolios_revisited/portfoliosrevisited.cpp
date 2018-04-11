#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact floating-point type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>			//leitourgei kai me Gmpzf kai Gmpz. kai ta dyo swsta kai sxedon idioi xronoi. 0.343 zf enw 0.328 z
typedef CGAL::Gmpz ET; 
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

using namespace std;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

//eksetazw gia constraint: profit >=R  yparxei lysh me variance<=V.
bool is_solvable(Program& qp, int V, int R){
    	qp.set_b(0, R);  //default >=

	// solve the program, using ET as the exact type
	Solution s = CGAL::solve_quadratic_program(qp, ET());
	//assert (s.solves_quadratic_program(qp));

  	// output
  	if (s.is_infeasible()) {	
    		return false;
  	} else if(s.is_unbounded()){	//den prepei na ftasei pote edw
    		std::cout << "unbounded\n";
  	}else if(s.is_optimal()){
    		if(s.objective_value() <= V) 	return true;
    		else 				return false;
  	}
}

//kanei mono binary search tipota allo
int max_profit_for_V(Program& qp, int V, int max_profit){
	//dyadikh anazhthsh
	int lo = 0;
	int hi = max_profit;
	int mid;

	while(lo < hi){
		mid = lo + (hi-lo+1)/2 ;    //it rounds up
		if( is_solvable(qp, V, mid) == true )
			lo = mid;
		else
			hi = mid-1;
	}

	//panta yparxei toylaxiston mia lysh opote den eksetazw an ontws to lo einai lysh
	return lo;         // lo is the greatest R for which is_solvable is true
}


void testcase(int n, int m){
	vector<int> c(n);	//to kostos agoras mias metoxhs ths ka8e ependyshs
	vector<int> r(n);	//to expected kerdos ths ka8e metoxhs (apo to ka8e temaxio) 

	double best_r_c_rate = 0;
	for(int i=0; i<n; i++){
		cin >> c[i] >> r[i];   //kostos kai expected return

		//bres thn pio kerdofora ependysh aneksarthtou riskou. dhladh megisto ri/ci
		best_r_c_rate = max( best_r_c_rate , (r[i]*1.0)/c[i]  );
	}

	// by default, we have a nonnegative QP with Ax >= b
	// CGAL::SMALLER, CGAL::LARGER.    kai stous periorimous mporw epishs na balw isothta me CGAL::EQUAL
	// ola ta assets ta epilegw se posothta >=0. no upper bound.
 	Program qp (CGAL::LARGER, true, 0, false, 0);  

	// constraint 0 and constraint 1
  	for(int i=0; i<n; i++){
		//arg1:variable_num,  arg2: constraint_num, arg3:value
    		qp.set_a(i, 0, r[i]);  // r0x0 + r1x1 +...r(n-1)x(n-1) >= R
    		qp.set_a(i, 1, c[i]);  // cost[0]x0 + cost[1]x1 +...cost(n-1)x(n-1) <= C (toy kefalaiou)
  	}

    	qp.set_r(1, CGAL::SMALLER);  

	//twra diabazw ta covariances
	//// we need to specify the entries of 2D, on and below the diagonal
  	for(int i=0; i<n; i++){
  		for(int j=0; j<n; j++){
			int temp;
			cin >> temp;	//diabazw olo to input an kai den to xreiazomai. nxn=n^2 input
			if(j<=i)	qp.set_d(i, j,  2*temp);
		}
  	}


	for(int investor=0; investor<m; investor++){
    		int C, V;
    		cin >> C >> V; 
		//to R 8a to 8etw sthn dyadikh anazhthsh oxi edw
    		//qp.set_b(0, R);  //default >=
    		qp.set_b(1, C);
    		//telos twn periorismwn

		//bres to megalytero kerdos R pou mporw na parw exontas risko-variance mikrotero apo V
		//kanw dyadikh anazhthsh sto kerdos R. ka8e fora blepw an yparxei dynath lysh gia ayto to R
		//without taking into consideration the variance max_profit is:
		int max_profit = C*best_r_c_rate;	
		cout <<     max_profit_for_V(qp, V, max_profit)   << endl;
  }
}

int main() {
//  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

  while(true){
	int n, m;
	cin >> n >> m;
	if(n==0 && m==0) break; //telos programmatos
	
	testcase(n,m);
  }
  return 0;
}






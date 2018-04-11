#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact floating-point type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET; 
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

using namespace std;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void my_min_problem(int, int);

void read_input_values(int, vector<int>&, vector<int>&, vector<vector<int> >&);

double floor_to_double(const CGAL::Quotient<ET>&);
double ceil_to_double(const CGAL::Quotient<ET>&);

int main() {
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

  while(true){
	int n, m;
	cin >> n >> m;
	if(n==0 && m==0) break; //telos programmatos
	
	my_min_problem(n,m);
  }
  return 0;
}


void my_min_problem(int n, int m){
  vector<int> cost(n);  //default initialization is zero "0"
  vector<int> r(n);	//expected return
  vector<vector<int> > cov( n, vector<int>(n)  ); //diastash mxn  dhladh n vectors to ka8ena mege8os n
	  //den ton xreiazomai ton pinaka ayton mporw na ta balw kateyueian sto Program

  read_input_values(n, cost, r, cov);	

  // by default, we have a nonnegative QP with Ax >= b
  Program qp (CGAL::LARGER, true, 0, false, 0);  // ola ta assets ta epilegw se posothta >=0
  // now set the non-default entries: 
  // enforce a bounded problem: 

  //minimize risk
  //edw apla antigrafw ton pinaka covariance matrix ston pinaka D tou Problem (multiple with 2)
  //// we need to specify the entries of 2D, on and below the diagonal
  for(int i=0; i<n; i++){
  	for(int j=0; j<=i; j++){
		qp.set_d(i, j,  2*cov[i][j]);
  	}
  }

  // constraint 0 and constraint 1
  for(int i=0; i<n; i++){
    	qp.set_a(i, 0, r[i]);  // r0x0 + r1x1 +...r(n-1)x(n-1) >= R
    	qp.set_a(i, 1, cost[i]);  // cost[0]x0 + cost[1]x1 +...cost(n-1)x(n-1) <= C (toy kefalaiou)
  }
  for(int investor=0; investor<m; investor++){
    	int C, R, V;
    	cin >> C >> R >> V; 
    	qp.set_b(0, R);  //default >=
    	qp.set_b(1, C);
    	qp.set_r(1, CGAL::SMALLER);  //!!! EQUAL isws einai kalytero
    	//telos twn periorismwn
    	// solve the program, using ET as the exact type
  	Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
  	assert (s.solves_quadratic_program(qp));

  	// output
  	if (s.is_infeasible()) {
    		std::cout << "No.\n";
  	} else if(s.is_unbounded()){	//den prepei na ftasei pote edw
    		std::cout << "unbounded\n";
  	}else if(s.is_optimal()){
    		if(s.objective_value() <= V) cout << "Yes.\n";
    		else cout << "No.\n";
  	}
  }
}


void read_input_values(int n, vector<int>& cost, vector<int>& r, vector<vector<int> >& cov){
	//arxikopoihmenoi hdh se katallhles diastaseis oi pinakes. opote apla diabazw input
	//prosoxh xwris push_back giati hdh exw arxikopoihsei megethos
	for(int i=0; i<n; i++){
		cin >> cost[i] >> r[i];   //kostos kai expected return
	}
	//twra diabazw ton pinaka me ta covariances nxn dimensions
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cin >> cov[i][j];  //faghto i, systatiko j
		}
	}			
}	




// Quotient<ET>  s.objective_value()   alla kalw prwta thn to_double kai meta kanw idia floor kai ceil me ta palia
//!!!!prosoxh to -0.2  8a to kanei output san -0   kai oxi san 0 sketo alla me - proshmo
double ceil_to_double(const CGAL::Quotient<ET>& x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

double floor_to_double(const CGAL::Quotient<ET>& x){
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}









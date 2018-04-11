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

void read_input_values(int, int, vector<int>&, vector<int>&,
			 vector<int>&, vector<vector<int> >&);

double floor_to_double(const CGAL::Quotient<ET>&);
double ceil_to_double(const CGAL::Quotient<ET>&);

int main() {
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  while(true){
	int n, d;
	cin >> n;
	if(n==0) break; //telos programmatos
	cin >> d;
	my_min_problem(n,d);
  }
  return 0;
}


void my_min_problem(int n, int d){

  // by default, we have a nonnegative QP with Ax <= b

  Program qp (CGAL::SMALLER, false, 0, false, 0);
  // now set the non-default entries: 
  // enforce a bounded problem: 
  qp.set_l(d, true, 0);

  //minimize risk
  //build constraints
  for(int i=0; i<n; i++){ 	//gia kathe yperepipedo
	long long metro = 0;
	for(int j=0; j<d; j++){ //diabazw ta a1 a2 ...ad   kai ypologizw kai to metro toy ka8etou dianysmatos  
		int a;		//gia ayto sum of squares
		cin >> a;
    		qp.set_a(j, i, a); //metablhth j, constraint i, value = a
		metro += a*a;
	}	//exw mexri twra toys syntelestes a0x0+a1x1+..a(d-1x(d-1)
	//twra to    r*|w|   (number of variable=d)
 //       cout << "squared metro= " << metro << endl;
	double temp = std::sqrt(metro);
	metro = temp;
//	cout << "double sqrt = " << temp << endl;
//	cout << "metro long long sqrt = " << metro << endl;
	qp.set_a(d, i, metro);

	int b;
	cin >> b;
	qp.set_b(i,b);
  }

  //miximize r => minimize -r
  qp.set_c(d, -1); 

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(qp, ET());
  assert (s.solves_quadratic_program(qp));

  // output
  if (s.is_infeasible()) {
    std::cout << "none\n";
  } else if(s.is_unbounded()){	//den prepei na ftasei pote edw
    std::cout << "inf\n";
  }else if(s.is_optimal()){
    std::cout << floor_to_double(-s.objective_value()) << endl;
//cout << "objective_value()=  " << s.objective_value() << endl;
//    std::cout << std::floor(std::sqrt(CGAL::to_double(-s.objective_value()))) << endl;
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



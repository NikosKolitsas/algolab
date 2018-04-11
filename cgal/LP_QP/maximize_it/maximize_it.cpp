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
//typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void my_max_problem(int, int);
void my_min_problem(int, int);

double floor_to_double(const CGAL::Quotient<ET>&);
double ceil_to_double(const CGAL::Quotient<ET>&);

int main() {
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

  while(true){
	int p,a,b;
	cin >> p;
	if(p==0) break; //telos programmatos
	cin >> a >> b;
	
	if(p==1) my_max_problem(a,b);
	else	 my_min_problem(a,b);
  }
  return 0;
}


void my_max_problem(int a, int b){
  // by default, we have a nonnegative QP with Ax <= b
  Program qp (CGAL::SMALLER, true, 0, false, 0);  //X,Y>=0 den exoun omws uper bound 

  // now set the non-default entries: 
  const int X = 0; 
  const int Y = 1;

  // constraint 0:   X + Y <= 4
  qp.set_a(X, 0, 1);  
  qp.set_a(Y, 0, 1); 
  qp.set_b(   0, 4);

  // constraint 1:  4X + 2Y <= ab
  qp.set_a(X, 1, 4);  
  qp.set_a(Y, 1, 2); 
  qp.set_b(   1, a*b);

  // constraint 2:  -X + Y <= 1
  qp.set_a(X, 2, -1);  
  qp.set_a(Y, 2, 1); 
  qp.set_b(   2, 1);

  // objective function: max bY - aX^2  =>  min -bY + aX^2
  // we need to specify the entries of 2D, on and below the diagonal
  qp.set_d(X, X,  2*a); //  a X^2

  qp.set_c(Y, -b); // -bY
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
  assert (s.solves_quadratic_program(qp));

  // output
  //if (s.status() == CGAL::QP_INFEASIBLE) {
  if (s.is_infeasible()) {
    std::cout << "no\n";
  } else if(s.is_unbounded()){
    std::cout << "unbounded\n";
  }else if(s.is_optimal()){
    //assert (s.status() == CGAL::QP_OPTIMAL);
    //  std::cout << ( -CGAL::to_double(s.objective_value()) ) << endl;
    std::cout << floor_to_double(-s.objective_value()) << endl;
  }
}


void my_min_problem(int a, int b){
  // by default, we have a nonnegative QP with Ax >== b
  Program qp (CGAL::LARGER, false, 0, false, 0);  //X,Y <=0 uper bound omws to w den exei opote to bazw
						  //argotera san periorismo sto ka8ena ksexwrista
  // now set the non-default entries: 
  const int X = 0; 
  const int Y = 1;
  const int W = 2;

  // enforce a bounded problem: 
  qp.set_u (X, true, 0);  //einai akribws to idio me to na baloume ton periorismo ston orismo toy Program qp(...true,0)
  qp.set_u (Y, true, 0);  //mono pou twra einai gia mia metablhth. epishs einai l<= x <= u  (kai isothtat dhladh)

  // constraint 0:   X + Y >= -4
  qp.set_a(X, 0, 1);  
  qp.set_a(Y, 0, 1); 
  qp.set_b(   0, -4);

  // constraint 1:  4X + 2Y + W >= -ab
  qp.set_a(X, 1, 4);  
  qp.set_a(Y, 1, 2); 
  qp.set_a(W, 1, 1); 
  qp.set_b(   1, -a*b);

  // constraint 2:  -X + Y >= -1
  qp.set_a(X, 2, -1);  
  qp.set_a(Y, 2, 1); 
  qp.set_b(   2, -1);

  // objective function: min aX^2 + bY + W^2
  // we need to specify the entries of 2D, on and below the diagonal
  qp.set_d(X, X,  2*a); //  a X^2
  qp.set_d(W, W,  2); //   W^2

  qp.set_c(Y, b); // +bY
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_quadratic_program(qp, ET());
  assert (s.solves_quadratic_program(qp));

  // output
  if (s.is_infeasible()) {
    std::cout << "no\n";
  } else if(s.is_unbounded()){
    std::cout << "unbounded\n";
  }else if(s.is_optimal()){
    //std::cout << CGAL::to_double(s.objective_value()) << endl;
    std::cout << ceil_to_double(s.objective_value()) << endl;
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




































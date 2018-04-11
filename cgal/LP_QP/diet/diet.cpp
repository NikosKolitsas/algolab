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
	int n, m;
	cin >> n >> m;
	if(n==0 && m==0) break; //telos programmatos
	
	my_min_problem(n,m);
  }
  return 0;
}


void my_min_problem(int n, int m){
  vector<int> nutr_min(n);  //default initialization is zero "0"
  vector<int> nutr_max(n);
  vector<int> prices(m);
  vector<vector<int> > nutrients( m, vector<int>(n)  ); //diastash mxn  dhladh m vectors to ka8ena mege8os n

  read_input_values(n, m, nutr_min, nutr_max, prices, nutrients);	

  // by default, we have a nonnegative QP with Ax <= b
  Program qp (CGAL::SMALLER, true, 0, false, 0);  // ola ta faghta se posothta >=0 w
  // now set the non-default entries: 
  // enforce a bounded problem: 
  // constraints:
  for(int i=0; i<n; i++){	//periorismos kata sthles (to idio nutrient gia ola ta diaforetika foods)
	for(int j=0; j<m; j++){ //gia ola ta food koitaw thn periekthkothta sto sygkekrimeno nutrient
		qp.set_a(j, 2*i, nutrients[j][i]);	//minimum  periektikothta apo ayto to nutrient
		qp.set_a(j, 2*i+1, nutrients[j][i]);	//maximum  periektikothta apo ayto to nutrient
	}
	qp.set_b(2*i, nutr_min[i]);
	qp.set_r(2*i, CGAL::LARGER);  // all the foods together >= min

	qp.set_b(2*i+1, nutr_max[i]);
  } //telos twn periorismwn

  //minimize price
  for(int i=0; i<m; i++){
	qp.set_c(i, prices[i]); 	//to i food exei price tade
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_nonnegative_linear_program(qp, ET());
  assert (s.solves_quadratic_program(qp));

  // output
  if (s.is_infeasible()) {
    std::cout << "No such diet.\n";
  } else if(s.is_unbounded()){	//den prepei na ftasei pote edw
    std::cout << "unbounded\n";
  }else if(s.is_optimal()){
    //std::cout << CGAL::to_double(s.objective_value()) << endl;
    std::cout << floor_to_double(s.objective_value()) << endl;
  }
}


void read_input_values(int n, int m, vector<int>& nutr_min, vector<int>& nutr_max,
			 vector<int>& prices, vector<vector<int> >& nutrients){
	//arxikopoihmenoi hdh se katallhles diastaseis oi pinakes. opote apla diabazw input
	//prosoxh xwris push_back giati hdh exw arxikopoihsei megethos
	for(int i=0; i<n; i++){
		cin >> nutr_min[i] >> nutr_max[i];
	}
	//twra diabazw ena ena ola ta foods
	for(int i=0; i<m; i++){
		cin >> prices[i];
		//twra ola ta nutrients pou periexei to sygkekrimeno food
		for(int j=0; j<n; j++){
			cin >> nutrients[i][j];  //faghto i, systatiko j
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









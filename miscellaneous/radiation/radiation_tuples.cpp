#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <tuple>

// choose exact floating-point type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET; 
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

using namespace std;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;  //!!!!input type bazw exact type epeidh x1^30 poly megalo
typedef CGAL::Quadratic_program_solution<ET> Solution;

//eksetazw gia constraint: profit >=R  yparxei lysh me variance<=V.
bool is_solvable(vector<tuple<int, int, int> >& healthy   , vector<tuple<int, int, int> >& tumor, int degree ){
	int h = healthy.size();
	int t = tumor.size();

	// by default, we have a nonnegative QP with Ax <= b
	// CGAL::SMALLER, CGAL::LARGER.    kai stous periorimous mporw epishs na balw isothta me CGAL::EQUAL
	//oi syntelestes tou polywnymou painoun oti timh 8eloun
 	Program qp (CGAL::SMALLER, false, 0, false, 0);  

	//ena periorismo gia to ka8e healthy kyttaro
	for(int cell=0; cell<h; cell++){
		int variable_cnt = 0; //poia metablhth eimaste twra px c0, c1, c2....
		for (int i = 0; i <= degree; ++i){
			for (int j = 0; j <= degree-i; ++j){
				for (int k = 0; k <= degree-i-j; ++k) {
					/* term x_1^i * x_2^j * x_3^k */
					double temp = pow(get<0>(healthy.at(cell)), i) *
              						pow(get<1>(healthy.at(cell)), j) *
              						pow(get<2>(healthy.at(cell)), k);
					//      pw(p.x1,i) *         pw(p.x2,j)    *          pw(p.x3,k);

					//arg1:     variable_num,  arg2: constraint_num, arg3:value
					qp.set_a(  variable_cnt++	, cell,  temp );
				}
			}
		}
		//ta healthy <=-1 
		//arg1: constraint_num,  arg2: value
    		qp.set_b(cell, -1);  //default <=	
	}

	//ena periorismo gia to ka8e tumor cell
	for(int cell=0; cell<t; cell++){
		int variable_cnt = 0; //poia metablhth eimaste twra px c0, c1, c2....
		for (int i = 0; i <= degree; ++i){
			for (int j = 0; j <= degree-i; ++j){
				for (int k = 0; k <= degree-i-j; ++k) {
					/* term x_1^i * x_2^j * x_3^k */
					double temp = pow(get<0>(tumor.at(cell)), i) *
              						pow(get<1>(tumor.at(cell)), j) *
              						pow(get<2>(tumor.at(cell)), k);
					//      pw(p.x1,i) *         pw(p.x2,j)    *          pw(p.x3,k);

					//arg1:     variable_num,  arg2: constraint_num, arg3:value
					qp.set_a(  variable_cnt++	, h+ cell,  temp );
				}
			}
		}
		//ta tumor >=1 
		//arg1: constraint_num,  arg2: value
    		qp.set_b( h+  cell, 1);  //default <=
    		qp.set_r( h+  cell, CGAL::LARGER);  
	}

	// solve the program, using ET as the exact type
	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution s = CGAL::solve_quadratic_program(qp, ET(), options);
	//assert (s.solves_quadratic_program(qp));

  	// output
  	if (s.is_infeasible()) {	
    		return false;
  	}else if(s.is_unbounded()){	//den prepei na ftasei pote edw
    		std::cout << "unbounded\n";
  	}else if(s.is_optimal()){
		return true;
  	}
}

//kanei mono binary search tipota allo
/*
εδω θελουμε το πρωτο true dhladh to 4.
the first element for which p is true
f f f t t t t
*/
int binary_search(vector<tuple<int, int, int> >& healthy   , vector<tuple<int, int, int> >& tumor,
		  int lo, int hi ){
	//dyadikh anazhthsh
	vector<bool> solved(hi+1  , -1); //-1: den exoume eksetasei to predicate, 0:false,  1:true
	solved[hi] = 1;  //thn kaloume mesw ths binary_search_exp_growth. ara sigoura to teleytaio true
	int mid;

	while(lo < hi){
		mid = lo + (hi-lo)/2 ;    //it rounds down
		if( is_solvable( healthy,  tumor , mid) == true ){
			hi = mid;
			solved[mid] = 1; //true;
		}
		else{
			solved[mid] = 0; // false;
			lo = mid+1;
		}
	}

	if(solved[lo]==0) //me kanena polywnymo den einai diaxwrisima
		return -1;
	if(solved[lo]==-1){ //den eksetasthke pote (einai to teleytaio poy emeine)
		if( is_solvable( healthy,  tumor , lo) == false ) 	return -1;
	}//eite eixe ypologistei apo prin se true. eite ypologisthke apo thn panw grammh
	else	return lo;      // lo is the least x for which p(x) is true 
}

// psaxnw polywnyma bathmou 0,1,2,4,8,16,32 to prwto pou 8a to epilyei 8a kanw dyadikh anazhthsh metaksy 
// previous and now. px metaksy 4 kai 8
int binary_search_exp_growth(vector<tuple<int, int, int> >& healthy   , vector<tuple<int, int, int> >& tumor ){
	bool solution_exist=false;
	int lo = 0;
	int hi = 0;

	while(hi <= 30){
		if( is_solvable( healthy,  tumor , hi) == true ){
			solution_exist = true;
			break;
		}
		else{	//eksetazw to epomeno diasthma
			if(hi==30) 	break; 	//eksetasa kai to teleytaio
			lo = hi+1;
			if(hi==0)	hi=1;
			else		hi = min( hi*2 , 30);
		}
	}

	if(solution_exist==false) //me kanena polywnymo den einai diaxwrisima
		return -1;
	else{
		if(hi==lo)		return lo; //brhkame hdh th lysh
		else return 		binary_search(healthy, tumor, lo, hi);			
		// lo is the least x for which p(x) is true 
	}
}

void testcase(){
int healthy_count, cancer_count;
    cin >> healthy_count >> cancer_count;

	vector<tuple<int, int, int> > healthy(healthy_count);
    	for(int healthy_index = 0; healthy_index < healthy_count; healthy_index++) {
      		int x, y, z;
      		cin >> x >> y >> z;

      		healthy[healthy_index] = make_tuple(x, y, z);
    }

    // read in cancer cell location
    vector<tuple<int, int, int> > tumor(cancer_count);
    for(int cancer_index = 0; cancer_index < cancer_count; cancer_index++) {
      int x, y, z;
      cin >> x >> y >> z;

      tumor[cancer_index] = make_tuple(x, y, z);
    }
	int result = binary_search_exp_growth( healthy ,  tumor );
	if(result==-1)		cout << "Impossible!\n";
	else			cout <<  result << endl;
}


int main() {
	//ios_base::sync_with_stdio(false);
	cin.sync_with_stdio(false);
  	cout.sync_with_stdio(false);
	int T; // First input line: Number of testcases.
	cin >> T;
	while (T--) 
		testcase();
}



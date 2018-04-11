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
typedef CGAL::Quadratic_program<ET> Program;  //!!!!input type bazw exact type epeidh x1^30 poly megalo
typedef CGAL::Quadratic_program_solution<ET> Solution;

//eksetazw gia constraint: profit >=R  yparxei lysh me variance<=V.
bool is_solvable(vector<vector<int> >& healthy   , vector<vector<int> >& tumor, int degree ){
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
					double temp = pow(healthy[cell][0], i ) *
							pow(healthy[cell][1], j ) * pow(healthy[cell][2], k );
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
					double temp = pow(tumor[cell][0], i ) *
							pow(tumor[cell][1], j ) * pow(tumor[cell][2], k );
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
int binary_search(vector<vector<int> >& healthy   , vector<vector<int> >& tumor,
		  int lo, int hi ){
	//dyadikh anazhthsh
	vector<bool> solved(hi+1  , -1); //-1: den exoume eksetasei to predicate, 0:false,  1:true
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
int binary_search_exp_growth(vector<vector<int> >& healthy   , vector<vector<int> >& tumor ){
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
	int h,t;
	cin >> h >> t;

	vector<vector<int> > healthy( h, vector<int>(3)  );
	vector<vector<int> >   tumor( t, vector<int>(3)  );
	for(int i=0; i<h; i++){
		cin >> healthy[i][0] /*x*/ >> healthy[i][1] /*y*/ >> healthy[i][2] /*z*/;
	}
	for(int i=0; i<t; i++){
		cin >> tumor[i][0] /*x*/ >> tumor[i][1] /*y*/ >> tumor[i][2] /*z*/;
	}


    // search upper lower
    int hi = 1;
    do {
      hi = hi * 2;
      //cout << "inc: for: " << exp_value << " is: " << can_solve(exp_value, healthy, cancer) << endl;
    } while(hi <= 30 && !is_solvable(healthy, tumor, hi));

    // do binary search for the right dimension
    int lo = max( 0, hi / 2 -1 );
    hi = min(30, hi);
 

    //dyadikh anazhthsh
     	vector<int> solved(hi+1  , -1); //-1: den exoume eksetasei to predicate, 0:false,  1:true
        int mid;
	int result;
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
                result = -1;
        if(solved[lo]==-1){ //den eksetasthke pote (einai to teleytaio poy emeine)
                if( is_solvable( healthy,  tumor , lo) == false )       result = -1;
        }//eite eixe ypologistei apo prin se true. eite ypologisthke apo thn panw grammh
        else    result = lo;      // lo is the least x for which p(x) is true 
     
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


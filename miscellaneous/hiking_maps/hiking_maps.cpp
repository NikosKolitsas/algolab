#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <stdexcept>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

template class vector<vector<P> >;

void swap(P& p1, P& p2){
	P temp = p1;
	p1 = p2;
	p2 = temp;
}

/*arg1 vector of q0..q5 points of triangle, arg2 point to test */
bool inside_triangle(vector<P>& tr, P p){
	//prepei na einai left_turn gia oles tis akmes tou trigwnou
	if( !right_turn(tr[0],tr[1],p)  &&  !right_turn(tr[2],tr[3],p)  &&  !right_turn(tr[4],tr[5],p) ){
		return true;
	}else 	return false;
}

void print_points(vector<P>& points){
	for(P p : points){
		cout << "(" << p.x() << " , " << p.y() << ") , ";
	}
	cout << endl;
}

void testcase(){
	int m;	//m points,   m-1 segments
	int n;  // n triangles
	cin >> m >> n;
	
	vector<P> points(m);
	vector<vector<P> > triangles(n, vector<P>(6));
	vector<vector<P> >& tr = triangles; //syntomografia
	

	for(int i=0; i<m; i++){
		int x,y;
		cin >> x >> y;
		points[i] = P(x,y);
	}

	for(int i=0; i<n; i++){
//		int q0, q1, q2, q3, q4, q5;
//		cin >> q0 >> q1 >> q2 >> q3 >> q4 >> q5;		
	/*	if(right_turn(q0,q1,q2))	swap(q0,q1);
		if(right_turn(q2,q3,q4))	swap(q2,q3);
		if(right_turn(q4,q5,q0))	swap(q4,q5);
*/
		for(int j=0; j<6; j++){
			int x,y;
			cin >> x >> y;
			triangles[i][j] = P(x,y); 	
		}
		//store them in left turn order. so that every interior point of the triangle is 
		//leftturn(q0,q1,?), leftturn(q2,q3,?) kok.....
		if(right_turn(tr[i][0],tr[i][1],tr[i][2]))	swap(tr[i][0],tr[i][1]);
		if(right_turn(tr[i][2],tr[i][3],tr[i][4]))	swap(tr[i][2],tr[i][3]);
		if(right_turn(tr[i][4],tr[i][5],tr[i][0]))	swap(tr[i][4],tr[i][5]);
		//cout << "swap3: " ; print_points(tr[i]);
	}

	//main algorithm
	//8esh i = apo posa triangles kalyptetai to segment i
	vector<int> covered(m-1, 0);  //m-1 segments yparxoun
	vector<vector<int> > tr_cover(n);

	int start = 0; //b
	//to i einai san to e inclusive ths ekfwnhshs  [start,i]
	int uncovered = m-1; // all segments initially uncovered
	int best = n; 	//an ta parw ola ta trigwna
	for(int i=0; i<n; i++){	//for all triangles
		//koita poia segments periexei
		//dhladh sygkrish me ola ta m points
		for(int j=0; j<m-1; j++){	//sygkrinw trigwno i me segm j  (pj,pj+1)
			if( inside_triangle( tr[i] , points[j] )  
				&&  inside_triangle( tr[i] , points[j+1] )){
			
				tr_cover[i].push_back(j);	//kalyptei to j segm.

				if(covered[j]==0)	//prin akalypto alla twra kalyptetai
					uncovered--;

				covered[j]++;
			}
		}//exw eksetasei ola ta segments kai points gia ayto to trigwno

		//an plhres tote proxwrw to start oso mporw
		while(uncovered==0){
			if(best > i-start+1)	best = i-start+1;
			//meiwnw to cover oswn segm kalyptontai apo to start triangle ka8ws 8a proxwrhsw
			for(int x : tr_cover[start]){
				covered[x]--;
				if(covered[x]==0)	uncovered++;
			}
			start++;
		}	
	}	
	cout << best << endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--){	
		testcase();
	}
	return 0;
}


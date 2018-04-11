#include <iostream>
#include <vector>

using namespace std;


class recursion{
public:
  //diastaseis nxn. 0 shmainei mh apo8hkeymeno ara prepei na to ypologisw
  vector<vector<int> >& is_stored;
  vector<int>& A;
  vector<int>& B;
  vector<int>& sumA;
  vector<int>& sumB;
  
  recursion(vector<vector<int> >& is_stored, vector<int>& A,
  		vector<int>& B, vector<int>& sumA, vector<int>& sumB):
		is_stored(is_stored), A(A), B(B), sumA(sumA), sumB(sumB){}
  
  int rec_try(int i, int j){
  	int temp;
  	if(i==1){
  		temp = A[1]*sumB[j];	
  		is_stored[i][j] = temp;
  		return temp;
  	}
  	if(j==1){
  		temp = B[1]*sumA[i];	
  		is_stored[i][j] = temp;
  		return temp;
  	}
  	//take all. mallon peritto ayto. san dummy megalh arxikopoihsh
  	int best = sumA[i]*sumB[j]; 
  	//take only 1 element from B.    and a elemnts from A
  	for(int a=1; a<=i-1; a++){
  		int cost = (sumA[i] - sumA[i-a]) * B[j];
  		if(is_stored[i-a][j-1]==0) 	rec_try(i-a,j-1);
  
  		temp = cost + is_stored[i-a][j-1];
  		if(temp < best)		best = temp;
  	}
  
  
  	//take only 1 element from A.    and b elemnts from B
  	for(int b=1; b<=j-1; b++){
  		int cost = (sumB[j] - sumB[j-b]) * A[i];
  		if(is_stored[i-1][j-b]==0) 	rec_try(i-1,j-b);
  
  		temp = cost + is_stored[i-1][j-b]; 
  		if(temp < best)		best = temp;
  	}
  
  	is_stored[i][j] = best;
  	return best;
  }
};//end of class recursion

void testcase(){
	int n;
	cin >> n; 
	
  	vector<vector<int> > is_stored(n+1, vector<int>(n+1,0));	
  	vector<int> A(n+1);
  	vector<int> B(n+1);
  	vector<int> sumA(n+1);
  	vector<int> sumB(n+1);
	//initialize extra first cell to zero
	A[0]=0; B[0]=0; sumA[0]=0; sumB[0]=0;
	
	for(int i=1; i<n+1; i++){
		int temp;
		cin >> temp;
		A[i] = temp-1;
		sumA[i] = sumA[i-1] + A[i];	
	}
	for(int i=1; i<n+1; i++){
		int temp;
		cin >> temp;
		B[i] = temp-1;
		sumB[i] = sumB[i-1] + B[i];	
	}

	recursion rec(is_stored,A,B,sumA,sumB);	
	cout << rec.rec_try(n,n) << endl;	
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


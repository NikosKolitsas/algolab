#include <iostream>
#include <vector>

using namespace std;

class recursion{
public:
  //diastaseis (n+1)x(n+1) dhladh ena parapanw se ka8e diastash.
  // -1 shmainei mh apo8hkeymeno ara prepei na to ypologisw
  vector<vector<int> >& store;
  vector<int>& A;
  vector<int>& sumA;
  
  recursion(vector<vector<int> >& store, vector<int>& A, vector<int>& sumA):
		store(store), A(A), sumA(sumA) {}
  
  int rec_try(int i, int j){
  	if(i==j){
  		store[i][j] = A[i];
  		return A[i];
  	}
	
	//select from the front
	if(store[i+1][j]==-1) 	rec_try(i+1,j);
	int select_front = A[i] +   ( sumA[j]-sumA[i]  -   store[i+1][j] );

	//select from the back
	if(store[i][j-1]==-1) 	rec_try(i,j-1);
	int select_back = A[j] +    ( sumA[j-1]-sumA[i-1]   -      store[i][j-1]  );

  	store[i][j] = max( select_front , select_back  );
  	return store[i][j];
  }
};//end of class recursion

void testcase(){
	int n;
	cin >> n; 
	
  	vector<vector<int> > store(n+1, vector<int>(n+1,-1)); //akyrh timh -1	
  	vector<int> A(n+1);
  	vector<int> sumA(n+1);
	//initialize extra first cell to zero
	A[0]=0;  sumA[0]=0; 
	
	for(int i=1; i<n+1; i++){
		int temp;
		cin >> temp;
		A[i] = temp;
		sumA[i] = sumA[i-1] + A[i];	
	}

	recursion rec(store,A,sumA);	
	cout << rec.rec_try(1,n) << endl;	
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





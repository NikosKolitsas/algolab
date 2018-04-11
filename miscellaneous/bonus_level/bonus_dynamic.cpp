#include <iostream>
#include <vector>

using namespace std;

void testcase(){
	int n;
	cin >> n;
	int n2 = n*n;
	
	vector<vector<int> > store(n, vector<int>(n)); //nxn array store optimum of previous step
	vector<vector<int> > a( n, vector<int>(n)  );
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cin >> a[i][j];
		}
	}

	
	//gia oles tis n diagwnious. to a8roisma i+j == s  sta8ero se ka8e diagwnio
	//to (0,0) to prospername. einai h bash mas. opote n-1 fores ektelw to loop
	store[0][0] = a[0][0];
	for(int s=1; s<n; s++){	//i+j==s
		for(int path1=0; path1<=s; path1++){
			for(int path2=0; path2<=s; path2++){

			}
		}
	}	

	cout <<  profit << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
void testcase();

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--) testcase();
}

void testcase(){
	int n;
	cin >> n; //number of values to sum
	double sum=0;
	double temp;
	for(int i=0; i<n; i++){
		cin >> temp;
		sum += temp;
	}
	cout << sum << endl;
}















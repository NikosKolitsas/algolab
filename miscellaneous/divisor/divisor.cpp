#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//returns:  [2,3,5,7,11....]   xwris to 1
void SieveOfEratosthenes(int n, vector<int>& result){
    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
    result.clear();
    vector<bool> prime(n+1,true);
    int upperBoundSquareRoot =   (int)  sqrt( (double)  n);
    for (int p=2; p<=upperBoundSquareRoot; p++){
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true){
	    result.push_back(p);
            // Update all multiples of p
            for (int i=p*2; i<=n; i += p)
                prime[i] = false;
        }
    }

    return;
}


int smallest_prime_divisor(int a, vector<int>& prime){
	//int upperBoundSqrt = (int) sqrt( (double) a);	
	int upperBoundSqrt =  sqrt( a);	
	for(int x: prime){
		if( x > upperBoundSqrt ) break;
		
		if( (a%x) == 0 )	return	a/x;
	}	
	return 1;
}

// px gia a=8 returns   [8,4,2,1]]
void find_path(int a, vector<int>& prime, vector<int>& path){
	int cur = a;
	path.push_back(a);
	while(cur!=1){
		cur = smallest_prime_divisor(cur, prime);
		path.push_back(cur);
	}
	return;
}

int common_suffix_length(vector<int>& path_a , vector<int>& path_b){
	//iterate apo to telos pros thn arxh. back_iterator
	int cnt = 0;
	for(auto a_rit = path_a.rbegin(),  b_rit = path_b.rbegin(); 
		a_rit != path_a.rend()  &&  b_rit != path_b.rend();
		++a_rit, ++b_rit){
		
		if(*a_rit !=  *b_rit)	break;

		cnt++;
	}
	return cnt;
}

void testcase(){
	int n,c;
	cin >> n >> c;

    	vector<int> prime;
	SieveOfEratosthenes(n, prime);		

	for(int i=0; i<c; i++){
		int a,b;
		cin >> a >> b;

		vector<int> path_a;
		vector<int> path_b;
		find_path(a, prime, path_a);					
		find_path(b, prime, path_b);

		int common_length = common_suffix_length(path_a, path_b);

		cout << ( path_a.size() - common_length ) +  (path_b.size() - common_length) << endl;					
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}

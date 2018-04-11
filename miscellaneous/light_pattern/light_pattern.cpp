#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

int distance(vector<bool>& state, int begin, vector<bool>& pattern){
	int cnt=0;
	for(int i=0; i<pattern.size(); i++){
		if(state[begin+i] != pattern[i]) cnt++;
	}
	return cnt;
}

void testcase(){
	int N, k, pattern_num;
	cin >> N >> k >> pattern_num;
	
	vector<bool> state(N);	//initial state of bulbs
	for(int i=0; i<N; i++){
		int temp;
		cin >> temp;
		state[i] = (temp==0) ? false : true;
	}	

	//extract pattern from int. convert it to bool array
	//an exei 1 o int se mia 8esh tote true. an 0 tote false
	vector<bool> pattern(k);
	int mask=1;	//initially test k bit and store it as first bit-pattern (reverse order)
	mask = (mask << (k-1));  // if k=4 then mask= 1000
	for(int i=0; i<k; i++){
		//logical 'and' with mask. at each loop shift the mask. test only 1 bit at a time
		pattern[i] = (pattern_num  &  mask) == 0 ? false : true;
		mask = (mask >> 1); 	//paw sto epomeno bit
	}
	
	int prev_opt=0;
	int prev_rev_opt=0;
	//diasxizw ola ta bulbs ana k (ana mhkos pattern dhladh) ena perasma
	for(int i=0; i<N/k; i++){
		int dist = distance(state, i*k, pattern);
		int rev_dist = k-dist;
		
		int new_opt = min(  prev_opt+dist  ,  prev_rev_opt+1+rev_dist);

		int new_rev_opt = min(  prev_rev_opt+ rev_dist   ,   prev_opt+1+dist);
		
		prev_opt = new_opt;
		prev_rev_opt = new_rev_opt;
	//	cout << "i= " << i << "  prev_opt= " << prev_opt << "   prev_rev_opt= " << prev_rev_opt << endl;
	}
	cout << prev_opt << endl;

	return;
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
















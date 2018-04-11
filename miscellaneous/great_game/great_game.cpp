#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;
const int MAX_INT = std::numeric_limits<int>::max();
const int MIN_INT = std::numeric_limits<int>::lowest(); 

void testcase(){
	int n, m;
	cin >> n >> m;
	int r, b;
	cin >> r >> b;

	r = r-1;
	b = b-1;

	vector<vector<int> > my_graph(n);

	for(int i=0; i<m; i++){ //diabazw ta edges
		int v, u;
		cin >> v >> u;
		v = v-1; u = u-1;
		//ftiaxnw ton antistrofo ara apo ton u ston v!!!!!
		my_graph[u].push_back(v);
	}

	//bfs modified!
	vector<int> v_min(n, MAX_INT);
	vector<int> v_max(n, MIN_INT);

	v_min[n-1] = 0;
	v_max[n-1] = 0;

	//ksekiname apo to telos (n-1) kai pame pros thn arxh mexri ton
	//teleytaio kombo pou mas endiaferei.
	//ta min kai max values ka8e kombou enhmeronontai mono apo ta epomena tou
	//px gia ton kombo 5 mono oi kombou 6,7,8....end mporoun na ton enhmerwsoun afou dag me mono pros ta empros akmes.
	//opote gia ton current kombo kserw oti oi times tou de 8a ksana allaksoun
	int finish = min(r,b);
	for(int v=n-1; v>finish; v--){
		int mymin = v_min[v];
		int mymax = v_max[v];
		//for all children
		for(int u : my_graph[v]){
			int new_min = 1 + mymax;
			int new_max = 1 + mymin;

			//update child's min and max values
			v_min[u] = min(  v_min[u] , new_min  );
			v_max[u] = max(  v_max[u] , new_max  );
		}
	}

	int red_moves = v_min[r];
	int black_moves = v_min[b];
	cout << "red_moves= " << red_moves << "    black_moves= " << black_moves << endl;
	if(red_moves == 1) {
		cout << "0\n";
		return;
	}
	int red_cnt = 1;
	int black_cnt = 0;
	while(true){
		black_cnt += 2;
		if(black_cnt >= black_moves){
			cout << "1\n";
			return;
		}	
		red_cnt += 2;
		if(red_cnt >= red_moves){
			cout << "0\n";
			return;
		}	
	}

}	

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}

#include <iostream>
#include <vector>

using namespace std;

void testcase() {

	int n;
	cin >> n;

	int a[n][n];
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			cin >> a[i][j];

	/* Mario can only move down or to the right.
	 * Princess can only move up or to the left.
	 * We can invert the path of the princess and represent the state
	 * of the problem as state[][][] where the dimensions are :
	 * 1st dim -> The diagonal Mario and the Princess are moving on.
	 *			  Note that the diagonal stays the same as long as both
	 *			  Mario and the Princess do one step at a time.
 	 * 2nd dim -> The row of Mario.
	 * 3rd dim -> The row of the princess.
	 * (The columns can be deducted from the rows as : diagonal - row)
	 *
	 * The value of the array is the total value of coins when starting 
	 * from the specific state. Thus we are looking for the value state[0][0][0].
	 */
	
	vector<vector<vector<int>>> state(2*n - 1, vector<vector<int>>(n, vector<int>(n, 0)));
	
	/* Precompute all the bounds instead of checking every time with a function */
	vector<vector<vector<bool>>> in_bounds(2*n , vector<vector<bool>>(n+1, vector<bool>(n+2, false)));

	for (int d=2*n-2; d>=0; d--) {
		for (int x1=0; x1<n; x1++) {
			for (int x2=0; x2<n; x2++) {
				state[d][x1][x2] = 0;

				int y1 = d - x1;
				int y2 = d - x2;

				in_bounds[d][x1][x2] =  y1 >= 0 && y2 >= 0
									 && y1 < n  && y2 < n;
			}
		}
	}

	for (int d=2*n-2; d>=0; d--) {
		for (int x1=0; x1<n; x1++) {
			int y1 = d - x1;
			if ((y1 >= n) || (y1 < 0)) { continue; } //Out of bounds
			for (int x2 = 0; x2<n; x2++) {
				int y2 = d - x2;
				if ((y2 >= n) || (y2 < 0)) { continue; }
				
				/* Now (x1, y1) is the point where we are on the first route
				 * and (x2, y2) is the point we are on the return route.
				 * Both can move either to the right or down so we can go to one of:
				 * + [d+1][x1]  [x2]   : Both go right.
				 * + [d+1][x1+1][x2]   : First goes down, second goes right.
				 * + [d+1][x1]  [x2+1] : First goes right, second goes down.
				 * + [d+1][x1+1][x2+1] : Both go down.
				 * We just have to choose the maximum of these.
				 */

				int m = 0;
				if (in_bounds[d+1][x1][x2])     { m = max(m, state[d+1][x1][x2]); }
				if (in_bounds[d+1][x1+1][x2])   { m = max(m, state[d+1][x1+1][x2]); }
				if (in_bounds[d+1][x1][x2+1])   { m = max(m, state[d+1][x1][x2+1]); }
				if (in_bounds[d+1][x1+1][x2+1]) { m = max(m, state[d+1][x1+1][x2+1]); }

				/* The coins taken are the sum of coins of the two squares plus the maximum */
				state[d][x1][x2] = a[x1][y1] + a[x2][y2] + m;

				/* DO NOT count doubles */
				if ((x1 == x2) && (y1 == y2)) {
					state[d][x1][x2] -= a[x1][y1];	
				}
			}
		}

	}
	
	cout << state[0][0][0] << endl;

}

int main() {
	int t;
	cin >> t;

	while (t--) {
		testcase();
	}

	return 0;
}
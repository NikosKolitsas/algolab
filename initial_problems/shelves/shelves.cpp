#include <iostream>
#include <cmath>

using namespace std;

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--){
		int l,m,n;
		cin >> l >> m >> n;
		
		int bestA=0, bestB=0, best=l;
		int a,b;
		if(n <= std::sqrt(l)){
			int l_minus_am = l+m; //kai meiwnetai kata m se ka8e loop
			for(a=0; a<=l/m; a++){
				l_minus_am -= m;
				b = l_minus_am / n; 
				int temp = l_minus_am - b*n;
				if(temp < best){
					best = temp;
					bestA = a;
					bestB = b;
				} 
			}
		}else{
			int maxB = l/n;
			int l_minus_bn = l- maxB*n  -n; //kai auksanetai kata n se ka8e loop
			for(b=maxB; b>=0; b--){
				l_minus_bn += n;
				a = l_minus_bn / m; 
				int temp = l_minus_bn-a*m;
				if(temp < best){
					best = temp;
					bestA = a;
					bestB = b;
				} 
			}
		}
		cout << bestA << " " << bestB << " " << best << endl;
	}
	return 0;		
}

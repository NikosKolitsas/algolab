#include <iostream>
#include <vector>

using namespace std;


void print_vector(vector<int>& v){
	for(int i=0; i<v.size(); i++)
		cout << v[i] << ", ";
}

/*
int even_subvectors(vector<int> v){
	int size = v.size();
	vector<int> S(size);
	//calculate the sum from the start up until current
	long sum=0;
	for(int i=0; i<size; i++){
		sum += v[i];
		S[i] = sum;
	}
}
*/
//pairnei ena dianysma pou exei times 0 kai 1 mono.
//epistrefei posa even subvectors yparxoun
int even_subvectors(vector<int>& v){
	int size = v.size();
	//how many "even S" I have encounter so far. I am at position j and count
	//the even S from 0 to j-1 position [0,j-1]. Odds are j-E
	long E=0;	
	long total_even=0; 
	int S = v[0];
	//an to prwto einai even tote exw synanthsei 1 alliws 0
	if(S == 0) 	{E++; total_even++;}
	for(int j=1; j<size; j++){
		S = ( (S+v[j])  % 2);
		if(S == 0){ //even
			total_even += E+1;
		}else	total_even += j-E;
		//eimai sth 8esh j ara prohgountai alla j stoixeia, E apo ayta even, j-E odd
		//enhmerwnw to E symfwna me to current S gia ton epwmeno gyro
		if(S == 0) 	E++;
	}
	return total_even;
}

void reduce_columns(vector<vector<int> >& S, int i1, int i2, vector<int>& v){
	int columns = v.size();
	if(i1==0){
		for(int j=0; j<columns; j++)
			v[j] =  S[i2][j] % 2;
		return;
	}
	for(int j=0; j<columns; j++){
		v[j] = ( S[i2][j] - S[i1-1][j] ) %2;
	}	
}

void column_sums(vector<vector<int> >& m, vector<vector<int> >& S){
	int rows = m.size();
	int columns = m[0].size();
	//h prwth grammh tou S idia me ton m
	for(int j=0; j<columns; j++){
		S[0][j] = m[0][j];
	}
	//diasxizw ton m kata grammes gia taxythta
	for(int i=1; i<rows; i++){
		for(int j=0; j<columns; j++){
			S[i][j] = S[i-1][j] + m[i][j];
		}
	}
}

void testcase(){
	int n;
	cin >> n; //matrix is nxn dimensions
	vector<vector<int> > m(n, vector<int>(n));
	//read input
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			cin >> m[i][j];

	//calculate sums of columns
	vector<vector<int> > S(n, vector<int>(n));
	column_sums(m, S);

	//for all different i1, i2 combinations do the calculation
	vector<int> reduced_vector(n);
	long long result=0;
	for(int i1=0; i1<n; i1++){
		for(int i2=i1; i2<n; i2++){
			reduce_columns(S, i1, i2, reduced_vector);
			result += even_subvectors(reduced_vector);
/*			cout << "i1= " << i1 << "  i2= " << i2 <<endl;
			print_vector(reduced_vector); 
			cout << "\n result= " << result << endl;
*/		}		
	}		
	cout << result << endl;
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

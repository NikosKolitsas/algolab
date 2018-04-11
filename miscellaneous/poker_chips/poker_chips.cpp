#include <iostream>
#include <vector>
#include <assert.h>
#include <unordered_map>

using namespace std;

//0 score αν αφαιρεσουμε 0 ή 1, 8 points an afairesoume 5	

class recursion{
public:
  int score[6] = {0,0,1,2,4,8}; 

  vector<vector<vector<vector<vector<long> > > > >& is_stored; 
  vector<vector<int> >& colour;	//the colour of each poker chips
  int n;
  
  recursion(
  	vector<vector<vector<vector<vector<long> > > > >& is_stored, 
  	vector<vector<int> >& colour, int n):
		is_stored(is_stored), colour(colour), n(n){}


  //me ti katastash arxizeis. dhladh posa exei h ka8e sou stoiba
  long rec_try(vector<int> state){

  	if(zero_state(state)){
		storeF(state, 0);
		return 0;
	}
	
	unordered_map<int,vector<int> > colours_found_and_positions;
	for(int i=0; i<n; i++){ //κοιταω τα χρωματα στις κορυφες και τα αποθηκευω καθως και τις θεσεις τους
		if(state[i]==0) continue; //empty column
			
		int current_colour = colour[i][state[i]]; //top element (colour) of the current stack
		//to εχουμε ξανασυναντησει το χρωμα ή δημιουργει καινουργιο vector αν το βρισκουμε πρωτη φορα
		colours_found_and_positions[current_colour].push_back(i); 
		//ή επεκτεινει το ηδη υπαρχον με το current position
	}	
	//iterate over all key,value pairs of Map.  colour,[positions]
	//and for every different colour test the score of every subset
	long best = -1;
	for(auto& colour_map : colours_found_and_positions){ 
		vector<int>& p = colour_map.second; //ta positions aytou tou xrwmatos
		
		//all possible subsets except empty
		int k = p.size(); 
		for(int i=1; i< (1 << k) /*pow(2,k)*/; i++){ //px gia k=3 (3 elements) [1,8) for subsets
			//to state pou 8a prokypsei afou afairesw tis epiloges mou
			vector<int> new_state = state;
			int cnt = 0; //plh8os tou subset autou 	    
			//extract one digit at a time
			for(int j=0; j<k; j++){// 1, 10, 100, 1000
				int mask = (1 << j);
				int digit = i & mask;
				if(digit!=0){	//pairnw to j element tou set-vector
					new_state[p[j]]--;
					cnt++;
				}
			}
			//an oxi apo8hkeymeno, tote ypologise to gia prwth fora
			if( is_storedF(new_state) == -1)	rec_try(new_state);
			int current_choice_score = score[cnt] + is_storedF(new_state);
			if(best < current_choice_score)		best = current_choice_score; 			
		}
	}
		
  	storeF(state, best);
  	return best;
  }

  //helper functions
  bool zero_state(vector<int>& state){
	for(int i=0; i<5; i++){
		if(state[i]!=0) return false;
	}
	return true;
  }

  inline long is_storedF(vector<int>& s){
	return is_stored[s[0]]  [s[1]]   [s[2]]   [s[3]]   [s[4]];
  }
  inline void storeF(vector<int>& s, long value){
	is_stored[s[0]]  [s[1]]   [s[2]]   [s[3]]   [s[4]] = value;
  }
};//end of class recursion

void testcase(){
	int n;	//stoibes
	cin >> n; 
	
	vector<vector<int> > colour(n);	//the colour of each poker chips

	//το υψος της καθε στηλης οταν ξεκινα το προβλημα. ο πινακας D χρησιμοποιειται μονο
	//για δεικτοδοτηση του πινακα
	vector<int> initial_state(5,0); 
	for(int i=0; i<n; i++){	//size of each column
		cin >> initial_state[i]; 
	}
	//oses diastaseis einai 0 den xrhsimopoioyntai opote tis kanw mege8os 1 san aplo stoixeio dhladh
	//kai oxi pinakas. stis ypoloipes tous pros8etw 1. an px h sthlh1 exei 2 chips tote 3 katastaseis: na einai adeia-0
	//na exei 1, na exei 2.
	vector<int> D(5);  	
	for(int i=0; i<5; i++){	
		if(initial_state[i] != 0)	D[i] = initial_state[i]+1;
		else  				D[i] = 1;
	}

	//here we store the results.
	//initialize to -1 (not computed)
	vector<vector<vector<vector<vector<long> > > > > is_stored( 
					D[0], vector<vector<vector<vector<long> > > >(
					D[1], vector<vector<vector<long> > >(
					D[2], vector<vector<long> >(
					D[3], vector<long>(
					D[4], -1    )))));

	//1 epipleon stoixeio
	//to prwto stoixeio ths ka8e sthlhs antistoixei sthn adeia sthlh
	for(int i=0; i<n; i++){	//for every column
		colour[i].reserve(D[i]);
		colour[i].push_back(-1); //akyro xrwma -1 gia adeia sthlh, oxi xrhsimo
		for(int j=0; j<initial_state[i]; j++){	//we read all the colours of the chips
			int temp;
			cin >> temp;
			colour[i].push_back(temp); 
		}
	}

	recursion rec(is_stored, colour, n);	
	cout << rec.rec_try(initial_state) << endl;	

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




#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


char smart_merge(char v1, char v2){
	if(v1==0 || v2==0) return 0;
	if(v1==2 && v2==3) return 0;	
	if(v1==3 && v2==2) return 0;	
	if(v1==1) return v2; //v2!=1
	return v1;
}


/**βαζει την τιμη value στις θεσεις του πινακα dest που βρισκουμε στον πινακα source.*/
void partially_fill_vector(vector<int>& source, vector<char>& dest, int value){
	for(int i : source){
		dest[i] = value;
	}
}

int main ( void ) {
	std::ios_base::sync_with_stdio(false);
	int cases;		//plhthos peiramatwn
	cin >> cases ;
	
	for ( int c =0; c < cases ; c ++) {
		unsigned int n;		//πληθος νομισματων
		cin >> n;
		unsigned int k; 	//πληθος μετρησεων
		cin >> k;
	//cout << "n= " << n << "         k= " << k << "   result=     ";	
		//0 οταν σιγουρα δεν ειναι fake. 1 αρχικοποιηση (δεν ξερω τιποτα για αυτο). 2 ελαφρυτερο.  3 βαρυτερο
		vector<char> coin_state(n+1,1);		//τα αρχικοποιω ολα στο 1.  μηκος n+1 γιατι η θεση 0 δεν χρησιμοποιειται.

		vector<int> first_group;
		vector<int> second_group;
		vector<char> swap_vector(n+1,0);		//otan εχω ανισοτητα. τοτε ολα οσα δεν συμμετεχουν γνησια.

		for(unsigned i=0; i<k; i++){ 	//gia oles tis k metrhseis 
			int Pi;
			cin >> Pi; 	//πληθος νομισματων στη ζυγαρια
			first_group.clear();
			second_group.clear();
			
			for(int l1=0; l1<Pi; l1++){
				int temp;
				cin >> temp;
				first_group.push_back(temp);
			}
			for(int l1=0; l1<Pi; l1++){
				int temp;
				cin >> temp;
				second_group.push_back(temp);
			}
			char result;
			cin>> result;
			//cout << "result char is " << result << endl;				

			
			if(result == '='){	//βαλτα ολα τα νομισματα στη κατηγορια 0, γνησια δηλαδη
				partially_fill_vector(first_group, coin_state, 0);
				partially_fill_vector(second_group, coin_state, 0);
			}else{ 
				fill(swap_vector.begin(), swap_vector.end(),0);
				if(result == '<'){		//και οσα δεν ειναι σε κανενα απο τα δυο συνολα καντα 0 γιατι γνησια!
					partially_fill_vector(first_group, swap_vector, 2);
					partially_fill_vector(second_group, swap_vector, 3);
				}else{ 	// result=='>'	//και οσα δεν ειναι σε κανενα απο τα δυο συνολα καντα 0 γιατι γνησια!
					partially_fill_vector(first_group, swap_vector, 3);
					partially_fill_vector(second_group, swap_vector, 2);
				}
				transform(coin_state.begin(),coin_state.end(),swap_vector.begin(),coin_state.begin(),smart_merge);				
			}
			

	//cout << "\niteration " << i << "coin state array  ";
//		for(int i : coin_state)
//			cout << i << ", ";

		}//telos twn metrhsewn
		//εξεταζω αν το 1 απο τα δυο συνολα αδειο και το αλλο να εχει μονο 1 στοιχειο.
		int elafry_cnt=0;
		int bary_cnt=0;
		int fake_index=-1; //akyro noumero
		for(unsigned int l1=1; l1<=n; l1++){
			switch (coin_state[l1]){
				case 2:
					elafry_cnt++;
					fake_index = l1;
					break;
				case 3:
					bary_cnt++;
					fake_index = l1;
					break;
			}
		}
		if(   ((elafry_cnt==0) && (bary_cnt==1))  || ((elafry_cnt==1) && (bary_cnt==0))   ){
//cout << "\nunique solution one set empty, the other with only one\n";
			cout << fake_index << endl;
		}else if(elafry_cnt>0 || bary_cnt>0){	//τοτε δεν το βρηκαμε επιστρεφουμε 0
			cout << 0 << endl;
		}else{ 	//ψαχνω μηπως εχει μεινει μονο ενας ασσος. τοτε ειναι αυτο αλλιως δεν το βρηκα και επιστρεφω 0.
			int count_1=0;
			for(unsigned int l1=1; l1<=n; l1++){
				if(coin_state[l1]==1){
					count_1++;
					fake_index=l1;
				}
			}
			if(count_1>1){
//cout << "\nno solution\n";
				cout << 0 << endl;
			}else{
//cout << "\n only one 1 left \n";
				cout << fake_index << endl;
			}
		}
/*	cout << "\ncoin state array\n";
		for(int i : coin_state)
			cout << i << ", ";
*/
	}
	return 0;
}


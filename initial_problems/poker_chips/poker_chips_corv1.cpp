#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <tuple>
#include <unordered_map>

using namespace std;

void resize_vector(vector<long>&, vector<int>& );
long access_vector(vector<long>&, vector<int>&, vector<int>& );
void store_vector(vector<long>&, vector<int>&, vector<int>&, long);
long stored(vector<int> );

//aux functions
void print_vector(vector<int> );
void print_vector(vector<long> );

/*ta kanw public tous pinakes aytous oratoi apo pantou*/
vector<vector<int> > colour;	//the colour of each poker chips
vector<long> store_table;	//here we store the results.
vector<int> dimensions;  	//Dimensions array. for each column has its size

	      //0 score αν αφαιρεσουμε 0 ή 1, 8 points an afairesoume 5	
int score[] = {0,0,1,2,4,8}; 

//παιρνει αντιγραφο του vector problem_state το τροποποιει και το επιστρεφει. (αφαιρει υψος 1 απο τις στηλες του χρωματος που επιλεγουμε)
vector<int> new_problem_state(vector<int> problem_state, vector<int> colour_positions){
	for(int i : colour_positions){
		problem_state[i]--;
		assert(problem_state[i] >= 0);
	}
	return problem_state;
}
//μετρα τις μη μηδενικες στηλες
bool zero_state(vector<int>& problem_state){
	int counter=0; //ποσες μη μηδενικες στηλες
	for(unsigned int i=0; i<problem_state.size(); i++){
		if(problem_state[i]>0)
			counter++;
	}
	return (counter <= 1) ? true : false; 
}

/*σε καθε αναδρομη μονο το υψος των στηλων αλλαζει.*/
long recursion(vector<int> problem_state){
//    print_vector(problem_state);
//    cout << endl;
	int n = colour.size();  //isodynama  = problem_state.size()
	if(zero_state(problem_state)){ //αν υπαρχουν 0 ή το πολυ 1 στηλες μη μηδενικες τοτε σκορ μηδεν.
//cout << "\nzero state: when problem_state= ";
//print_vector(problem_state);
//cout << "\n\n\n";
		return 0;
	}

	unordered_map<int,vector<int> > colours_found_and_positions;
	for(int i=0; i<n; i++){ //κοιταω τα χρωματα στις κορυφες και τα αποθηκευω καθως και τις θεσεις τους
		int col_height = problem_state[i]-1; //τη θεση οπου βρισκεται το top στοιχειο της στηλης πχ θεση 2 αν 3 στοιχεια στη στηλη
		//ο πινακας problem state leei το πληθος των chips στη καθε στηλη. η θεση τους ομως ειναι -1
		if(col_height < 0)	//η στηλη αυτη ειναι αδεια, προχωρα στην επομενη
			continue;
		int current_colour = colour[i][col_height];
		//if(colours_found_and_positions.count(current_colour)>0){ //to εχουμε ξανασυναντησει το χρωμα
		colours_found_and_positions[current_colour].push_back(i); //ή δημιουργει καινουργιο vector αν το βρισκουμε πρωτη φορα
		//το χρωμα ή επεκτεινει το ηδη υπαρχον με το current position
		//}
	}	
//	cout << "\ncolours_found_and_positions.size()= " << colours_found_and_positions.size() << endl;
	long best_score = -1;
	//int best_index = -1;
	//int index=0;
	for(auto& colour_map : colours_found_and_positions){ // για καθε χρωμα που εντοπισαμε κοιταμε τι σκορ μας δινει αν το αφαιρεσουμε
		//το βελτιστο σκορ απο το προβλημα που προκυπτει αν αφαιρεσουμε το συγκεκριμενο χρωμα
//	   cout << "\ncolour_map.second = ";
//	   print_vector(colour_map.second);
//	   cout << endl;
//	   cout << "\n new_problem_state = ";
//	   vector<int> sbhsto = new_problem_state(problem_state, colour_map.second) ;
//	   print_vector(sbhsto);
//	   cout << "\n new problem state size()= " << sbhsto.size() << endl;
		long rec_score = stored( new_problem_state(problem_state, colour_map.second) ); 
		long extra_score = score[colour_map.second.size()];  //σκορ αναλογο με το πληθος chips που αφαιρουμε
/*	   cout << "\nfor loop colour_map: current prob_st: ";
	   print_vector(problem_state);
	   cout << "  new_prob_st: ";
	   print_vector(new_problem_state(problem_state, colour_map.second));	   	
	   cout << endl << "rec_score= " << rec_score << "  extra_score= " << extra_score;
*/		if(best_score < rec_score+extra_score){
	//		best_index = index;
			best_score = rec_score+extra_score;
	//		cout << "egine ana8esh!";
		}			
	//	index++;
	}
	
//        print_vector(problem_state);
//	cout << "  rec best score= " << best_score << endl;
	return best_score;
}

int main ( void ) {
	std::ios_base::sync_with_stdio(false);
	long cases;		//plhthos peiramatwn
	cin >> cases ;
	

	/*key is the colour*//*value is a vector of indices (where- in which positions we found the specific colour)*/

	for (long c =0; c < cases ; c ++) {
		int n;  //n<=5 
		cin >> n;		//number of columns
		
		vector<int> initial_state(n);  //το υψος της καθε στηλης οταν ξεκινα το προβλημα. ο πινακας dimensions χρησιμοποιειται μονο
						//για δεικτοδοτηση του πινακα
		for(int i=0; i<n; i++){	//size of each column
			cin >> initial_state[i]; 
		}

		colour.clear();
		colour.resize(n);

		for(int i=0; i<n; i++){	//for every column
			for(int j=0; j<initial_state[i]; j++){	//we read all the colours of the chips
				int temp;
				cin >> temp;
				colour[i].push_back(temp); 
			}
		}

		//o πινακας store_table θα εχει μεγεθος οσο το γινομενο των υψων της καθε στηλης+1. διοτι η καθε στηλη μπορει να ειναι και
		//αδεια. πχ αν 1η στηλη=5 και 2η στηλη=3. μεγεθος= (5+1)*(3+1)= 24
		dimensions.clear();
		dimensions.resize(n);
		std::transform(initial_state.begin(),initial_state.end(), dimensions.begin(), [](int x){ return x+1; });
		store_table.clear();
		resize_vector(store_table, dimensions);

		//the algorithm begins
		cout << recursion(initial_state) << endl;
//		cout << "store_table = \n";
//		print_vector(store_table);
	}
	return 0;
}




/**first argument is the vector that we want to resize
the second is the size of each dimension that we want the new array to have*/
void resize_vector(vector<long>& myvector, vector<int>& size_of_dimensions){
	long mult=1;
	for(unsigned int i=0; i<size_of_dimensions.size(); i++){
		mult *= size_of_dimensions[i]; 
	}
	myvector.resize(mult,-1);	//τιμη αρχικοποιησης
}

/**arg1: the vector that we want to access,
   arg2: a vector with the size of each dimension e.g. N[1] the size of 1st dimension (how many elements it has)
   arg3: access_coord: wich element to access e.g to access myvector[2][5][1] you should pass a vector with {2,5,1} as elements
   assumes Row-Major order of elements*/
long access_vector(vector<long>& myvector, vector<int>& N, vector<int>& access_coord){
	int n = N.size(); //πληθος στηλων
	long offset = access_coord[0];
	for(int i=1; i<n; i++){
		offset = offset*N[i] + access_coord[i];
	}	
/*cout << "\naccess_vector (";
print_vector(access_coord);
cout << ") = " << myvector[offset] << "     offset= "<<offset;	
*/	return myvector[offset];
}
void store_vector(vector<long>& myvector, vector<int>& N, vector<int>& access_coord, long value){
	int n = N.size(); //πληθος στηλων
	long offset = access_coord[0];
	for(int i=1; i<n; i++){
		offset = offset*N[i] + access_coord[i];
	}
/*cout << "\nstore_vector (";
print_vector(access_coord);
cout << ") = " << value << "     offset= "<<offset;	
*/	myvector[offset] = value;
}

long stored(vector<int> problem_state){
	long result = access_vector(store_table, dimensions, problem_state);
	if(result != -1){ //-1 η ακυρη τιμη αρχικοποιησης. αν διαφορετικη τοτε το εχουμε υπολογισει το υποπροβλημα
/*	  cout << "\n!!! stored table usefull! ";
print_vector(problem_state);
cout << ") = " << result << "     offset= ";	
*/
		return result;
	}
	result = recursion(problem_state); //diaforetika ypologise anadromika to paron problhma
	store_vector(store_table, dimensions, problem_state, result);  // αποθηκευσε το στον πινακα για μελλοντικη χρηση
	return result;
}



void print_vector(vector<int> vector){
        for(auto v : vector)
                cout << v << ", ";
}
void print_vector(vector<long> vector){
        for(auto v : vector)
                cout << v << ", ";
}




















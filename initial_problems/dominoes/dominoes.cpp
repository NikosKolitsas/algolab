#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main ( void ) {
	std::ios_base::sync_with_stdio(false);
	int cases;		//plhthos peiramatwn
	cin >> cases ;
	
	for ( int c =0; c < cases ; c ++) {
		unsigned long n;			//posa domino to kathe peirama
		cin >> n;
		unsigned long counter =0; //counter posa exei riksei mexri twra
		unsigned long heightLeft=0; //posa mporei na riksei akoma poso υψος εχει μεινει. οταν διαβαζουμε 1 τοτε το υψος που του μενει για την συνεχεια ειναι 1-1=0 δηλαδη ουτε το επωμενο του δεν μπορει να ριξει. αν διαβασουμε πχ 3 τοτε  heightLeft = 3-1=2 μπορει να ριξει τα δυο επομενα. οποτε συγκρινουμε αυτο που διαβαζουμε με αυτο που εχουμε απο το προηγουμενα και κραταμε το max
//		string line;
//		getline(cin, line);
//		istringstream ss(line);
//		ss >> myint;
		do{
			unsigned long currentHeight;
			cin >> currentHeight;
			if( currentHeight > heightLeft ) 
				heightLeft = currentHeight;
			counter++;
			heightLeft--; //wste sto epwmeno
		} while(heightLeft>=1 && counter<n);

		cout << counter << endl ; 	// write result on stdout
		
		//throw the rest of the line
		/*αν δεν εχω διαβασει ολη την εισοδο της γραμμης και δεν ειναι το τελευταιο πειραμα δηλαδη η τελευταια γραμμη εισοδου του αρχειου (γιατι τοτε μπορει να λειπει ο χαρακτηρας \n)*/
		string restLine;
		if( (counter<n) && (c!=cases-1) ) 
			getline(cin, restLine);
	}
	return 0;
}

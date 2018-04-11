#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

struct Alien{
	unsigned long start;
	unsigned long end;
	bool proud;
	bool operator<(const Alien& rhs) const{
		return start < rhs.start;
	}
	Alien(int s,int e){ start = s; end=e; proud=true; }
	Alien(){};
};


int main ( void ) {
	std::ios_base::sync_with_stdio(false);
	int cases;		//plhthos peiramatwn
	cin >> cases ;
	
	vector<Alien> alienVector;		//τα αρχικοποιω ολα στο 1.  μηκος n+1 γιατι η θεση 0 δεν χρησιμοποιειται.

	for ( int c =0; c < cases ; c ++) {
		unsigned long n;		//aliens
		cin >> n;
		unsigned long m; 	//humans
		cin >> m;
		
		alienVector.clear();		
		//alienVector.resize(n);	

		for(unsigned long i=0; i<n; i++){ 
			int begin, end;
			cin >> begin >> end;
			if(begin==0 && end==0)	//den bazw mesa tous losers
				continue;
			alienVector.push_back(Alien(begin,end));
		}
//cout << "initial n= " << n << endl;
//cout << "alienVector.size()= " << alienVector.size() << endl;
		n=alienVector.size();  		//ws n θεωρω το μεγεθος χωρις τους losers
		std::sort(alienVector.begin(),alienVector.end());
		
		unsigned long current=0;
		unsigned long next=0;
		bool noone=false;
		
		while(current<n && next<n){
			if(current==n-1)
				break;
			Alien currentA = alienVector[current];
			next = current+1;
			do{
				assert(next<n);
				assert(current<n);
				Alien nextA = alienVector[next];
				if( (currentA.start==nextA.start) && (currentA.end==nextA.end) ){//kanena apo ta dyo
					currentA.proud = false;
					nextA.proud = false;
					next++;	
				}else if( (currentA.start==nextA.start) && (currentA.end<nextA.end) ){ //den eimai proud
					currentA.proud = false;
					current=next;
					break;
				}else if( (currentA.start<nextA.start) && (currentA.end<nextA.end) ){ //eimai proud kai paw ston epwmeno
					if(currentA.end+1<nextA.start){ //tote den nikountai oloi oi anthrwpoi ara 0 to apotelesma
						noone=true;	//prepei to ena na καλυπτεται με το επωμενο ή τουλαχιστον να ειναι συνεχομενα
						break;		//επισης πρεπει το τελευταιο να φτανει μεχρι τον τελευταιο ανθρωπο δηλαδη m
					}
					current=next;
					break;
				}else{
					nextA.proud = false;
					next++;
				}
			}while(next<n);
			if(noone==true)
				break;
		}
		if(noone==true || (alienVector[current].end<m && alienVector[next].end<m) ){
//			cout << "current= " << current << "   next= "<< next << endl;
//			cout << "noone flag= " << noone << endl << "alienVector[current].end= " << alienVector[current].end << endl;
			cout << 0 << endl;
		}else{
			unsigned long resultCnt=0;
			for(Alien a : alienVector){
				if(a.proud==true) 
					resultCnt++;
			}
			cout << resultCnt << endl;
		}
//			cout << "\nprint alienVector.proud \n";
//			for(unsigned long int i=0;i<n;i++){
//				cout << i <<"=  " << alienVector[i].proud << endl; 
//			}
	}

	return 0;
}

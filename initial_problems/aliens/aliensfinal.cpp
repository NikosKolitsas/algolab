#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <tuple>

using namespace std;

struct Alien{
	long begin;
	long end;
	bool take;
	bool operator<(const Alien& rhs) const{
		return begin <= rhs.begin;		//return true αν ηδη σωστη η διαταξη. δηλαδη το struct ayto πρεπει να ειναι
	}						//αριστερα του struct της παραμετρου
	Alien(long begin, long end): begin(begin), end(end) {take=true;}  //{ begin=pbegin; end = pend;  take=true; }
};

//περνουμε αποφασεις για το ποιο ειναι εγκυρο απο τις επομενες καταχωρησεις βασει αυτων των δεδομενων. επισης υπολογιζουμε συναρτηση κοστους
//με βαση αυτα τα δεδομενα.
struct BaseInfo{
	long begin; 
	long end; 
	vector<Alien>::iterator pIt;

	//BaseInfo(){ previousEnd = std::numeric_limits<long>::min();} //h prwth barka kai oi ypoloipes sth arxh μπορει να ξεκινησει οσο αριστερα θελει
		//edw oso mikrotero score toso kalytero! opote arxikopoiw me maxi
};


int main ( void ) {
	std::ios_base::sync_with_stdio(false);
	 long cases;		//plhthos peiramatwn
	cin >> cases ;
	
	vector<Alien> myvector;

	for (long c =0; c < cases ; c ++) {
		long n,m;		//aliens, humans
		cin >> n >> m;
		
		myvector.clear();		

		for(long i=0; i<n; i++){ 		//eisodos gia καθε εξωγηινο
			long begin, end;
			cin >> begin >> end;
			if(begin==0 && end==0)
				continue; 	//μη τα βαζει μεσα
			myvector.push_back(Alien(begin, end)); //by default true to take
		}
		std::sort(myvector.begin(), myvector.end());

		
		//βρισκουμε το people cover
		if(myvector[0].begin != 1){
			cout << 0 << endl;
			continue;  //se allo test case
		}
		long currentEnd = myvector[0].end;
		bool peopleCover = true;

		for(vector<Alien>::iterator it = myvector.begin()+1; //apo edw ksekinw thn anazhthsh
		it != myvector.end(); ++it){
			
			if(it->begin > currentEnd+1){
				peopleCover = false;
				break;
			}
			else if(it->end <= currentEnd)
				continue;
			else{
				assert(it->begin <= currentEnd+1   &&  it->end > currentEnd);
				currentEnd = it->end;
			}
		}
		//elegxos αν καλυπτεται και ο τελευταιος ανθρωπος
		if(currentEnd < m) peopleCover = false;

		if(peopleCover==false){
			cout << 0 << endl;
			continue; //to next test case
		}


		//βρισκουμε ποια νικανε εμμεσα ολους τους aliens
		BaseInfo baseInfo;
		baseInfo.begin = myvector[0].begin; //logika iso me 1
		baseInfo.end = myvector[0].end; 
		baseInfo.pIt = myvector.begin(); //to prwto stoixeio 
		
		for(vector<Alien>::iterator it = myvector.begin()+1; //apo edw ksekinw thn anazhthsh
		it != myvector.end(); ++it){
			
			if(it->begin == baseInfo.begin && it->end == baseInfo.end){
				it->take = false;
				baseInfo.pIt->take = false;
			}else if(it->begin == baseInfo.begin &&   it->end > baseInfo.end){
				baseInfo.pIt->take = false;
				baseInfo.end = it->end;
				baseInfo.pIt = it; 	//παιρνουμε τον currentIterator gia na elegxoyme tous epomenous
			}else if(baseInfo.begin <= it->begin   &&   it->end <= baseInfo.end){
				it->take = false;
				//baseInfo.begin = it->begin;  //περιττο αλλα δεν βλαπτει
			}else{
				assert(baseInfo.begin < it->begin   && baseInfo.end < it->end);
				baseInfo.begin = it->begin;
				baseInfo.end = it->end;
				baseInfo.pIt = it; 	//παιρνουμε τον currentIterator gia na elegxoyme tous epomenous
			}
		}

		long resultCnt=0;
		for(Alien a : myvector){
			if(a.take==true) resultCnt++;
		}
		//cout << counter << endl;
		cout << resultCnt << endl;

		/*	cout << "\nprint boatVector.take \n";
			for(unsigned long i=0;i<boatVector.size();i++){
				cout << i <<"=  " << boatVector[i].take << endl; 
			}
		*/
	}


	return 0;
}

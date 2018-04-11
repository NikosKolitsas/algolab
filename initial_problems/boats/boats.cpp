#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <tuple>

using namespace std;

struct Boat{
	long length;
	long position;
	bool take;
	bool operator<(const Boat& rhs) const{
		return position < rhs.position;
	}
	Boat(long plength, long pposition){ length=plength; position = pposition;  take=false; }
//	Boat(){};
};

//περνουμε αποφασεις για το ποιο ειναι εγκυρο απο τις επομενες καταχωρησεις βασει αυτων των δεδομενων. επισης υπολογιζουμε συναρτηση κοστους
//με βαση αυτα τα δεδομενα.
struct BaseInfo{
	long previousEnd; 

	BaseInfo(){ previousEnd = std::numeric_limits<long>::min();} //h prwth barka kai oi ypoloipes sth arxh μπορει να ξεκινησει οσο αριστερα θελει
		//edw oso mikrotero score toso kalytero! opote arxikopoiw me maxi
};

struct BestScore{
	long int bestScore;
	vector<Boat>::iterator bestIt;

	BaseInfo& baseInfo; //me bash αυτο αξιολογουμε το σκορ

	void compare(vector<Boat>::iterator currentIt){
		//υπολογισμος του σκορ του νεου iterator. αν καλυτερο τοτε το κραταμε
		long newScore;
		if(currentIt->position - currentIt->length < baseInfo.previousEnd){ //an dhladh den mporoume na to paroume terma aristera
			//tote ξεκιναμε απο το τελος του προηγουμενου kai προσθετουμε το μηκος του
			newScore = baseInfo.previousEnd + currentIt->length;
		}else{newScore = currentIt->position; } //diaforetika to pame terma aristera kai ftanoume mexri to passalo toy
//cout << "compare() about position= " << currentIt->position << "  newScore=  " << newScore << endl;
		//συγκριση με προηγουμενο
		if(newScore < bestScore){ //βρηκαμε καλυτερο!
//cout << "newScore < bestScore   previous position= " << bestIt->position << "  previous bestScore= " << bestScore << endl;
//cout << "nextBest Position= " << currentIt->position << "  newScore= " << newScore << endl;
			bestScore = newScore;
			bestIt = currentIt;
		}
	}

	BestScore(BaseInfo& pbaseInfo): baseInfo(pbaseInfo){/*baseInfo = pbaseInfo;*/}
	void reInitialize(vector<Boat>::iterator nullValue){
		bestScore = std::numeric_limits<long>::max(); //::min()  antistoixa
		bestIt = nullValue;
	}
};


int main ( void ) {
	std::ios_base::sync_with_stdio(false);
	 long cases;		//plhthos peiramatwn
	cin >> cases ;
	
	vector<Boat> boatVector;		//τα αρχικοποιω ολα στο 0. oti dhladh den ta pairnw

	for (long c =0; c < cases ; c ++) {
		long n;		//wizards
		cin >> n;
		
		boatVector.clear();		

		for(long i=0; i<n; i++){ 
			long length, position;
			cin >> length >> position;
			boatVector.push_back(Boat(length, position));
		}
		std::sort(boatVector.begin(), boatVector.end());
		

		BaseInfo baseInfo;
		
		BestScore bestScore(baseInfo);
		bestScore.bestScore = boatVector[0].position;
		bestScore.bestIt = boatVector.begin();
	
		long counter=0;

		for(vector<Boat>::iterator currentIt = boatVector.begin()+1; //apo edw ksekinw thn anazhthsh
		currentIt != boatVector.end(); ++currentIt){
			//filter out τα ακυρα. οσο δεν συμβαδιζουν με το baseInfo
			if(currentIt->position < baseInfo.previousEnd){ //πρεπει να ξεκινα πριν το τελος της αλλης-αδυνατο
//cout << "filter out pos=  " << currentIt->position << " because baseInfo.previousEnd=  " << baseInfo.previousEnd<<endl;
 				//make it false alla einai apo default
				continue; //προχωρα στο επωμενο.
			}		
			//ελεγχος αν πρεπει να διαλεξουμε το μεχρι τωρα βελτιστο. Τελος του τρεχοντος optimization
			if(bestScore.bestScore <= currentIt->position){ //αυτο καθως και ολα τα επωμενα σιγουρα χειροτερα αφου ολα 
				counter++;				//θα φτανουν σιγουρα μεχρι το position τους
				bestScore.bestIt->take = true; //telika den xrhsimopoiw ta boolean
//cout << "position=  " << bestScore.bestIt->position << "   ends at:   " << bestScore.bestScore << endl;
				//αλλαζω τα baseInfo kai ta bestScore για την συνεχεια
				baseInfo.previousEnd = bestScore.bestScore;
				bestScore.reInitialize(boatVector.end()); //edw to .end() to bazw anti gia nullptr pou απαγορευεται
			}
			//εδω παμε οσο ψαχνουμε σε εγκυρα για βελτιστο. ή αν ξαναρχιζουμε καινουργιο ψαξιμο βελτιστου
			bestScore.compare(currentIt);
		}
		//to teleytaio bestScore to temporary den to exoyme metrhsei. μπορει ομως να εχει τιμη null==myvector.end() ή ακυρη γενικα.δηλαδη
		//δηλαδη να πηραμε το τελευταιο και μετα τα επωμενα να εγιναν filter out ή να μην υπηρχαν επομενα
		if(bestScore.bestIt != boatVector.end()){
			counter++;
			bestScore.bestIt->take = true;
		}				

		long resultCnt=0;
		for(Boat a : boatVector){
			if(a.take==true) 
			resultCnt++;
		}
		cout << counter << endl;
		//cout << resultCnt << endl;

			//cout << "\nprint boatVector.take \n";
			for(unsigned long i=0;i<boatVector.size();i++){
				//cout << i <<"=  " << boatVector[i].take << endl; 
			}
	}


	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct Word{
	int id; 
	int x; //position of word

	Word(int id, int x): id(id), x(x) {}
	
	bool operator<(const Word& rhs) const{
		return x <= rhs.x;
	}
};

void testcase(){
	int n;	//number of words
	cin >> n;
	
	vector<int> freq(n); //frequency of each word
	for(int i=0; i<n; i++){
		cin >> freq[i];
	}

	int freq_sum = std::accumulate(freq.begin(), freq.end(), 0);	
	vector<Word> words;
	words.reserve(freq_sum);
	for(int i=0; i<n; i++){
		for(int j=0; j<freq[i]; j++){
			int position;
			cin >> position;
			words.push_back(  Word( i , position)  );		
		}
	}
	std::sort(words.begin(), words.end());


	/*main algorithm*/
	int uncovered = n; //ola akalypta
	int start = 0; 	//h prwth leksh tou words
	int best = std::numeric_limits<int>::max();
	vector<int> covered(n,0);	//oles oi lekseis exoun cover 0 arxika

	for(int i=0; i<words.size(); i++){
		//poia leksh kalyptei to kainourgio Word
		Word curr = words[i];
		if( covered[curr.id]==0 )	//prin akalypth
			uncovered--;
		
		covered[curr.id]++;

		//koitaw an beltiwsa to best kai proxwrw to start oso mporw
		while(uncovered==0){	//exw plhrh kalypsh
			if( best >   words[i].x - words[start].x +1 )
				best =   words[i].x - words[start].x +1;

			//o start proxwra mia 8esh ara ksekalyptw- meiwnw to cover stis 
			//lekseis pou kalyptei
			covered[  words[start].id  ]--;
			if(covered[  words[start].id  ] ==0)//elegxw an ksekalyfthke meta thn meiwsh
				uncovered++;

			start++;
		}//bgainei apo to while loop otan pleon exei proxwrhsei toso pou den kalyptontai ola
	}
	cout << best << endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}





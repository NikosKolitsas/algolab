#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
//#include <boost/functional/hash.hpp>


using namespace std;

class str{
  public:
	int x,y;
	int vx, vy;
	
	int depth;

	str(int x, int y, int vx, int vy, int depth): x(x), y(y), vx(vx), vy(vy), depth(depth){}
	
	bool operator==(str const& other) const{
        	return x == other.x && y == other.y
		    && vx == other.vx && vy == other.vy;
    	}

	static int finish_x, finish_y;
	static void set_finish_point(int x, int y){
		str::finish_x = x;
		str::finish_y = y;
	}
	bool is_finish(){
		return x==finish_x && y==finish_y;
	}
};
//akyro initialization pou prepei omws na ginei!!!!!
int str::finish_x = -1;
int str::finish_y = -1;


	template <class T>
	inline void hash_combine(std::size_t & seed, const T & v){
    		std::hash<T> hasher;
    		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
struct my_hash{
	std::size_t operator()(str const& p) const{
	      	std::size_t seed = 0;
  		//boost::hash_combine(seed, p.x);
  		hash_combine(seed, p.x);
        	hash_combine(seed, p.y);
        	hash_combine(seed, p.vx);
        	hash_combine(seed, p.vy);
        	return seed;
 	} 
};

void testcase(){
	int X, Y;
	cin >> X >> Y;
	int sx,sy; //start point
	cin >> sx >> sy;
	int fx,fy; //finish point
	cin >> fx >> fy;
	int P; //number of obstacles
	cin >> P;


	//diabazw ola ta obstacles
	//gia ka8e obstacle kanw true ton boolean matrix
	//ena obstacle periexei polla shmeia
	vector<vector<bool> > is_obstacle(X, vector<bool>(Y,false));
	for(int i=0; i<P; i++){
		int x1,y1, x2,y2;
		cin >> x1 >> y1 >> x2 >> y2;
		for(int xi=x1; xi<=x2; xi++){
			for(int yi=y1; yi<=y2; yi++){
				is_obstacle[xi][yi] = true;
			}
		}
	}

	str::set_finish_point(fx,fy);
	if(is_obstacle[fx][fy]){ 
		cout << "No solution.\n";
		return;
	}

	//kanw bfs
	//prin balw ta paidia mesa sthn queue elegxw an peftw se empodio
	//kai an exw ksanapaei ston idia katastash

	//check if start is destination
	str start(sx,sy,0,0, 0);  //starting point
	if(start.is_finish()){
		cout << "Optimal solution takes 0 hops.\n";
		return;		
	}

	std::unordered_set<str, my_hash> visited;
	queue<str> Q;
	Q.push(start);
	visited.insert(start);
	while(!Q.empty()){
		str v = Q.front();
		Q.pop();
		//for all children
		for(int new_vx=v.vx-1; new_vx<=v.vx+1; new_vx++){
			for(int new_vy=v.vy-1; new_vy<=v.vy+1; new_vy++){
				int new_x = v.x + new_vx;
				int new_y = v.y + new_vy;
				//check if new position is legit
				if(new_x<0 || new_x>=X || new_y<0 || new_y>=Y) continue;
				if(new_vx<-3 || new_vx>3 || new_vy<-3 || new_vy>3) continue;
				//check if child is destination
				if(new_x==fx && new_y==fy){
					cout << "Optimal solution takes " << v.depth+1 << " hops.\n";
					return;		
				}
				//elegxos gia empodio
				if(is_obstacle[new_x][new_y])	 continue; 

				str new_position(new_x, new_y, new_vx, new_vy, v.depth+1);
				//test if already visited this state. tets if inside set
				auto search = visited.find(new_position);
    				if(search != visited.end()) {
        				continue; //exoume ksanapaei. proxwra sto epomeno
    				}					

				Q.push(new_position);
				visited.insert(new_position);
			}	
		}		
	}
	//an eftasa edw shmainei oti destination mh prosbasimos
	cout << "No solution.\n";
	return;	
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




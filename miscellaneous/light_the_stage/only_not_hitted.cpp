#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>	    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

typedef K::Point_2 P;

void testcase(){
	int m,n;
	cin >> m >> n;

	vector<P> players(m);
	vector<long> p_radius(m); //player radius

	for(int i=0; i<m; i++){
		int x,y,r;
		cin >> x >> y >> r;
		players[i] = P(x,y);
		p_radius[i] = r;
	}
	//diabazw tis lampes
	long h;
	cin >> h;
	//vector<P> lamps(n);
	vector< std::pair<P,int> > lamps;
	lamps.reserve(n);
	for(int i=0; i<n; i++){
		int x,y;
		cin >> x >> y;
		lamps.push_back(  std::make_pair(  P(x,y) , i)  );
	}

	// construct triangulation
	Triangulation t;
	t.insert(lamps.begin(), lamps.end());

	vector<int> hit_by_lamp(m, INT_MAX); //apo poia lampa xtypietai o ka8e paixths
	//-1 akyrh timh. opoios parameinei -1 shmainei oti den ton xtypaei kamia ara nikhths
	//for every player calculate his distance from the nearest lamp
	for(int i=0; i<m; i++){
		Triangulation::Vertex_handle v = t.nearest_vertex( players[i] );		

		K::FT nearest_dist = CGAL::squared_distance(v->point(), players[i] );

		if( p_radius[i]+h >  CGAL::sqrt(nearest_dist) ) 	
			hit_by_lamp[i] =  v->info();
	}

	int max_lamp_num = *( std::max_element(  hit_by_lamp.begin()  ,  hit_by_lamp.end()  ) );
	vector<int> winners;
	for(int i=0; i<m; i++){
		if( hit_by_lamp[i] == max_lamp_num )
			winners.push_back(i);
	}
//	std::sort(winners.begin(), winners.end());
	
	for(int x : winners){
		cout << x << " ";
	}
	cout << endl;
	
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}

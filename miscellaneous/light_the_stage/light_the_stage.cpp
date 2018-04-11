#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>	    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Point_set_2<K, Tds>				    Point_set; //subclass of Delaunay
typedef CGAL::Point_set_2<K, Tds>::Vertex_handle		    Vertex_handle;

typedef K::Point_2 P;
typedef K::Circle_2 Circle;

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
	//  https://judge.inf.ethz.ch/doc/cgal/doc_html/Point_set_2/classCGAL_1_1Point__set__2.html
	//  https://judge.inf.ethz.ch/doc/cgal/doc_html/Point_set_2/index.html#Point_set_2Example
	Point_set pset;
	pset.insert(lamps.begin(), lamps.end());



	vector<int> hit_by_lamp(m, INT_MAX); //apo poia lampa xtypietai o ka8e paixths
	// opoios parameinei sto INT_MAX shmainei oti den ton xtypaei kamia ara nikhths
	//for every player calculate his distance from the nearest lamp
	for(int i=0; i<m; i++){
		Vertex_handle v = pset.nearest_vertex( players[i] );		

		K::FT nearest_dist = CGAL::squared_distance(v->point(), players[i] );

		if( p_radius[i]+h >  CGAL::sqrt(nearest_dist) ) 	
			hit_by_lamp[i] =  v->info();
	}

	int max_lamp_num = *( std::max_element(  hit_by_lamp.begin()  ,  hit_by_lamp.end()  ) );

    if(max_lamp_num != INT_MAX){
  
  	
     	std::list<Vertex_handle> LV;
   	for(int i=0; i<m; i++){
   		LV.clear();
   		//range circle, perilambanei kai to orio. ton idio ton kyklo
   		//emeis omws 8eloyme mono sto eswteriko tou kyklou.
   		Circle rc(  players[i]  , pow( p_radius[i]+h , 2)  ); 
     		pset.range_search(rc, std::back_inserter(LV));
   
   
     		std::list<Vertex_handle>::const_iterator it;
   		int min_lamp = INT_MAX; //h mikroterh lampa pou petyxainei ton paixth
   		//iterate over all vertex_handle that are incide or on the circle
     		for(it=LV.begin();  it != LV.end(); it++){
   			Vertex_handle temp = *it;
   			//check if on boundary.
   			// http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Circle__2.html#a9ebac5d44d9dbce6f3acceb3e6eff7a0
   			if( rc.bounded_side( temp->point() ) == CGAL::ON_BOUNDARY  )
   				continue;
   		
   			if( temp->info() < min_lamp  )
   				min_lamp = temp->info();
   		}
   		hit_by_lamp[i] = min_lamp;	
   	}
   
   	max_lamp_num = *( std::max_element(  hit_by_lamp.begin()  ,  hit_by_lamp.end()  ) );
  
    }


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

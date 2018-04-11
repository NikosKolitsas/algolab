#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <typeinfo>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;


using namespace std;
int main(){
	int n,m;
	ios_base::sync_with_stdio(false);
	while(true){
		cin >> n;
		//cout<<"n: "<<n<<endl;
		if(n==0) break;
		
		vector<K::Point_2> points(0);
		for(int i=0; i<n; i++){
			K::Point_2 p;
			cin>> p;
			points.push_back(p);		
		}
		
		Triangulation t;
		t.insert(points.begin(), points.end());
		cin >> m;
		
		for(int i=0; i<m; i++){
			K::Point_2 p;
			cin >> p;
			K::Point_2 rest = t.nearest_vertex(p)->point();
			double d = CGAL::to_double(CGAL::squared_distance(p,rest));
			cout<<std::fixed<<setprecision(0)<<d<<endl;
		}
	}
}

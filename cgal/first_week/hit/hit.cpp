#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <limits>
//#include <CGAL/Ray_2.h>
//#include <CGAL/intersections.h>


using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

int main(){
//	std::cout << "max(int) = " << std::numeric_limits<int>::max() << endl;
//	std::cout << "max(long) = "<< std::numeric_limits<long>::max() << endl;
//	std::cout << "max(long long) = "<< std::numeric_limits<long long>::max() << endl;
//std::cout << "max(double) = "<< std::numeric_limits<double>::max() << endl;    
//std::cout << "max(long double) = "<< std::numeric_limits<long double>::max() << endl;    
//std::cout << "max(long long) = "<< std::numeric_limits<long long>::max() << endl;    

  while(true){
	int n; 	//number of obstacles
  	cin >> n;
	if(n==0)  break;	//telos
  	long long x, y;
  	std::cin >> x >> y;
  	K::Point_2 ray_point1(x, y);
  	std::cin >> x >> y;
  	K::Point_2 ray_point2(x, y);
	K::Ray_2 ray(ray_point1, ray_point2);	
 
 /* 	long long x, y;
	long long z,w;
  	std::cin >> x >> y;
  	std::cin >> z >> w;
	K::Ray_2 ray(  *(new P(x,y)), *(new P(z,w)) );	
 */
	bool intersection_found = false; // an brw tomh tote apla diabazw thn eisodo xwris na kanw tipota
	for(int i=0; i<n; i++){ //diabazw ola the obstacle segments
		long long r,s,t,u;
		std::cin >> r >> s >> t >> u;
		if(intersection_found){
//			cout << "throw    " << r << "  " << s << "  " << t<< "  "  << u << endl;
		 	continue; // peta thn eisodo
		}
//	std::cout << "r << s << t << u   " << r<< "  "  << s<< "  "  << t<< "  "  << u << endl;
		P p1(r,s);
		P p2(t,u); //arxh kai telos gia segment
		S segment = S(p1,p2); 
		if(CGAL::do_intersect(ray, segment)){
			intersection_found = true;
		}		
	}
	if(intersection_found == true) 
		cout << "yes" << endl;
	else
		cout << "no" << endl;	
  }
}





































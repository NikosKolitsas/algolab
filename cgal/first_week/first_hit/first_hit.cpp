#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

double floor_to_double(const K::FT&);

int main(){
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  while(true){
	int n; 	//number of obstacles
  	cin >> n;
	if(n==0)  break;	//telos
  	long long x, y, a, b;
  	std::cin >> x >> y >> a >> b;
	P start_point(x,y);
	K::Ray_2 ray( start_point,  *(new P(a,b))  );	
 
	P first_hit;
	K::FT min_dist;
	bool initialization_done = false; 
	for(int i=0; i<n; i++){ //diabazw ola the obstacle segments
		long long r,s,t,u;
		std::cin >> r >> s >> t >> u;
		S segment( *(new P(r,s))  , *(new P(t,u))  );

		if(CGAL::do_intersect(ray, segment)){ 	//bres pou temnontai kai an ayto einai shmeio h segment
			auto o = CGAL::intersection(ray, segment); 	//construction here!!!!
			P intersection_point;
			if (const P* op = boost::get<P>(&*o)){		//if intersection is point
				intersection_point = *op;
			}
			else if (const S* os = boost::get<S>(&*o)){  	//if it is a segment
				K::FT source_distance = CGAL::squared_distance(start_point, os->source());	
				K::FT target_distance = CGAL::squared_distance(start_point, os->target());
				if(target_distance < source_distance){
				//	cout << "!!! target_distance < source_distance\n";
					intersection_point = os->target();
				}else{
				//	cout << "source_distance < target_distance\n";
					intersection_point = os->source();
				}
			}
			//elegxos an exei arxikopioh8ei to first_hit (kontinotero shmeio) an oxi tote parto oti kai na einai
			//an pali h tomh me to current intersection pio konta apo to apo8hkeymeno tote krata to
			if(  (initialization_done == false) 
			    	|| (CGAL::squared_distance(start_point, intersection_point) < min_dist) ){
				first_hit = intersection_point;
				min_dist = CGAL::squared_distance(start_point, intersection_point);
				initialization_done = true;
			} 			
		}
	}
	if(initialization_done == false) 
		cout << "no" << endl;	//den yparxei kamia tomh
	else
		cout << floor_to_double(first_hit.x()) << " " << floor_to_double(first_hit.y()) << endl; 		
  }
}


double floor_to_double(const K::FT& x){
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}


































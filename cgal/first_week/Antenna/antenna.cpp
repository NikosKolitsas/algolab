#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>
#include <limits>


using namespace std;

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  K::Point_2                      Point;

double ceil_to_double(const K::FT& x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}


int main(){
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  while(true){
        int n;  //number of citizens
        std::cin >> n;
        if(n==0)  break;        //telos

	vector<Point> v_points(n);
	for(int i=0; i<n; i++){  //diabase olous tous an8roupous gia ayto to peirama
        	double x, y;
        	std::cin >> x >> y;
        	K::Point_2 p(x, y);
		v_points[i] = p;
	}
	Min_circle mc(v_points.begin(), v_points.end(), true);	
	Traits::Circle c = mc.circle();
	double result = ceil_to_double(sqrt(c.squared_radius()));
	std::cout << result  << std::endl;
  }
  return 0;
}


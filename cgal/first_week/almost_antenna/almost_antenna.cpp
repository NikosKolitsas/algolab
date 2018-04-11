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

double ceil_to_double(const K::FT&);
void remove_support_points(Min_circle& mc, vector<Point>&);
K::FT minus_one_support_point(Min_circle&);

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
	K::FT min_radius = minus_one_support_point(mc);
	
	double result = ceil_to_double(sqrt(min_radius));
	std::cout << result  << std::endl;
  }
  return 0;
}


K::FT minus_one_support_point(Min_circle& mc){
	Traits::Circle c = mc.circle();	//!!constructs the circle (construction!)
//cout << "number of support points:  " << mc.number_of_support_points() << endl;
	K::FT min_radius = c.squared_radius(); 

	vector<Point> base_points;
	remove_support_points(mc, base_points); //vector me ola ta point ektos apo ta support points
	//se ayto to vector  8a pros8etw ka8e fora ola ta support points ektos apo ena.
	for(int i=0; i<mc.number_of_support_points(); i++){ //mh baleis to i support point
		for(int j=0; j<mc.number_of_support_points(); j++){ //pros8ese ola ta support points ektos apo to it
			if(i==j) continue;  //pareleipse ayto to support point
			base_points.push_back(mc.support_point(j));
		}
		Min_circle mc_partial(base_points.begin(), base_points.end(), true);
		Traits::Circle c = mc_partial.circle();	//!!constructs the circle (construction!)
		K::FT new_radius = c.squared_radius();
		if(new_radius < min_radius){
			min_radius = new_radius;
		}
		//remove the added support points
		for(int i=0; i<mc.number_of_support_points()-1; i++){
			base_points.pop_back();
		}
	}		
	return min_radius;
}

/**base_points is out parameter*/
void remove_support_points(Min_circle& mc, vector<Point>& base_points){
	for(auto pit=mc.points_begin(); pit!=mc.points_end(); ++pit){
		bool remove_flag = false;
		for(auto sit=mc.support_points_begin(); sit!=mc.support_points_end(); ++sit){
			if( *pit == *sit ){
//				cout << "support point removed:  " << *pit << endl;
				remove_flag = true; 
			}
		}
		if(remove_flag == false) base_points.push_back(*pit);
	}
//	cout << "full circle number =  " << mc.number_of_points() << endl;
//	cout << "base points number =  " << base_points.size() << endl;
}


double ceil_to_double(const K::FT& x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

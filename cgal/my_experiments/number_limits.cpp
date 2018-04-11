#include <iostream>
#include <limits>

using namespace std;

int main(){
	std::cout << "max(int) = " << std::numeric_limits<int>::max() << endl;
	std::cout << "max(long) = "<< std::numeric_limits<long>::max() << endl;
	std::cout << "max(long long) = "<< std::numeric_limits<long long>::max() << endl;
	std::cout << "max(double) = "<< std::numeric_limits<double>::max() << endl;    
	std::cout << "max(long double) = "<< std::numeric_limits<long double>::max() << endl;    
	std::cout << "max(long long) = "<< std::numeric_limits<long long>::max() << endl;    
	return 0;
}








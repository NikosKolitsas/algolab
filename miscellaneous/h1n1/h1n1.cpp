#include <vector>
#include <algorithm>
#include <set>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Edge Edge;

typedef K::Point_2 P;
typedef K::Segment_2 S;



void testcase(int n){
    vector<P> points(n);
    for(int i=0;i<n;i++) {
        int x,y;
        cin >> x >> y;
        points[i] = P(x,y);
    }
    Triangulation t;
    t.insert(points.begin(), points.end());

    //initialize f->info() to zero
    for(Triangulation::All_faces_iterator  f = t.all_faces_begin();  f != t.all_faces_end(); ++f){
	f->info() = 0;
    }

    int m;
    cin >> m;

    for(int i=1;i<=m;i++) {
        int x,y;
        long r;
        cin >> x >> y;
        cin >> r;
        P s(x,y);	//query point. according to slides
	try {
		if (r <= 0) throw true;

 		Face_handle f = t.locate(s);
		if(f==NULL) //s outside of affine hull.
			throw true;
 		if (CGAL::squared_distance(s, t.nearest_vertex(s, f)->point()) < r)
			throw false;	//already too close to get sick

 		 // DFS
		std::vector<Face_handle> stack;
		stack.push_back(f);
		f->info() = i;
		while (!stack.empty()) {
			f = stack.back();
			stack.pop_back();
			if (t.is_infinite(f)) throw true;
			for (int j = 0; j < 3; ++j)
				if (f->neighbor(j)->info() < i &&
				    t.segment(Triangulation::Edge(f,j)).squared_length() >= 4 * r) {
					stack.push_back(f->neighbor(j));
					f->neighbor(j)->info() = i;
				}
 		}
 		throw false;
	} catch (bool solvable) { std::cout << (solvable ? "y" : "n"); }
    }//telos tou loop gia olous tous m xrhstes
    cout << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    do {
        cin >> n;
        if(n!=0) testcase(n);
    } while(n != 0);
}

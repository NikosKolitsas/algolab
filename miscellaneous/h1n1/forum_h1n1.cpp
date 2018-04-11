#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>


using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Edge Edge;

typedef K::Point_2 P;
typedef K::Segment_2 S;

void solve(const int n) {
    vector<P> points(n);
    for(int i=0;i<n;i++) {
        long x,y;
        cin >> x >> y;
        points[i] = P(x,y);
    }
    Triangulation t;
    t.insert(points.begin(), points.end());

    int m;
    cin >> m;

    for(int i=1;i<=m;i++) {
        int x,y;
        long d;
        cin >> x >> y;
        cin >> d;

        P q(x,y);
        bool exit_found = false;

        // locate face where query point q is containted
        Face_handle fh = t.locate(q);
        if(d <= 0) exit_found = true;

        // if there is enough space in current face
        if(!exit_found && CGAL::squared_distance(q, t.nearest_vertex(q, fh)->point()) >= d) {
            //dfs to exit
            vector<Face_handle> stack;
            stack.push_back(fh);
            fh->info() = i;
            while(!stack.empty() && !exit_found) {
                
                Face_handle f = stack.back();
                stack.pop_back();
                //check if exit node
                if(t.is_infinite(f)) {
                    exit_found = true;
                } else {
                    // add neighbours
                    for(int j=0;j<3;j++) {
                        // get neighbouring face
                        Face_handle neighbor = f->neighbor(j);
                        // edge that separated current face with neighbor
                        Edge e(f, j);                        
                        // get segment of edge
                        Triangulation::Segment s = t.segment(e);
                        // check of this edge is unvisited and wide enough
                        if(neighbor->info() < i && s.squared_length() >= 4*d) {
                            stack.push_back(neighbor); //push on stack
                            neighbor->info() = i; // set as visited
                        }
                    } 
                }
            }
        } 

        if(exit_found) {
            cout << "y";
        } else {
            cout << "n";
        }
    }
    cout << endl;

}


int main()
{
    ios_base::sync_with_stdio(false);
    int n;
    do {
        cin >> n;
        if(n!=0) solve(n);
    } while(n != 0);
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Edge Edge;
typedef Triangulation::Edge_iterator Edge_iterator;


typedef K::Point_2 P;
typedef K::Segment_2 S;

struct my_edge { 
	long to, width;
	my_edge(long to, long width): to(to), width(width){}
};

template class vector<vector<my_edge> >;
template class vector<vector<long> >;

void dijkstra_max_min(const vector< vector<my_edge> >& graph, long source, vector<long>& width, vector<long>& previous) {
    for(long i=0; i<graph.size(); i++){
	width[i] = LONG_MIN;  //INT_MIN
	previous[i] = -1;	//akyrh timh
    }

    width[ source ] = LONG_MAX;
    previous[ source ] = source;
    //to set einai taksinomhmeno. key=width   value=vertex_id 
    set< pair<long,long> > active_vertices;
    active_vertices.insert( {LONG_MAX,source} );
        
    while (!active_vertices.empty()) {
	//pairnw to max element tou set. dhladh to teleytaio. (in ascending order)
	//kai ta value aytoy tou pair dhladh to vertex id.
	//to u mpainei pleon sta oloklhrwmena.
        long u = active_vertices.rbegin()->second;
	//to afairw apo to set
        active_vertices.erase( --active_vertices.end() );
	//eimai o u kai paw se ola mou ta paidia: v kai tous ananewnw to bottleneck
        for (my_edge e : graph[u]){ 
	    long v = e.to;
	    long alt = max(  width[v] ,  min( width[u] , e.width)  );
            if ( alt > width[v]  ){
                active_vertices.erase( { width[v], v } );
                width[v] =   alt;
                active_vertices.insert( { alt, v } );

		previous[v] = u;
            }
	}
    }
}

void print_f_info(Face_handle f){
	cout << "\nf->info()=  " << f->info() << endl;
}

void testcase(long n){
    vector<P> points(n);
    for(long i=0;i<n;i++) {
        long x,y;
        cin >> x >> y;
        points[i] = P(x,y);
    }
    Triangulation t;
    t.insert(points.begin(), points.end());


    //initialize f->info() to unique id. every face will become a vertice in the new graph
    //infinite vertice will be the 0. so the finite vertices will be 1..t.number_of_faces()
    long infinite_vertex = 0;
    long unique_id = 1;

   //initialize f->info() .   all infinite faces go to zero
    for(Triangulation::All_faces_iterator  f = t.all_faces_begin();  f != t.all_faces_end(); ++f){
	if(t.is_infinite(f))	f->info() = 0;
	else			f->info() = unique_id++;
    }

    //construct the graph
    //number of graph nodes = number_of_faces() +1   (+1 for the infinite vertice)
    vector<vector<my_edge> > graph(  t.number_of_faces()+1  );
  
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
		Face_handle f1 = e->first;
		Face_handle f2 = f1->neighbor(e->second);

		long edge_width = t.segment(e).squared_length();
		//apokleietai na einai kai ta dyo faces infinite ara self loop sto neo grafo
		graph[f1->info()].push_back(    my_edge(f2->info(), edge_width)   );
		graph[f2->info()].push_back(    my_edge(f1->info(), edge_width)   );
    }	//twra exei kataskeyastei olos o grafos
    

    vector<long> width(graph.size());
    vector<long> previous(graph.size());
    dijkstra_max_min(graph, infinite_vertex, width, previous);

    //Number of users asking for escape route
    long m;
    cin >> m;
    for(long i=0;i<m;i++) {
        long x,y;
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

		if(width[f->info()] >= 4*r) 	throw true;
		else				throw false;
	} catch (bool solvable) { std::cout << (solvable ? "y" : "n"); }

    }//telos tou loop gia olous tous m xrhstes
    cout << endl;
    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    long n;
    do {
        cin >> n;
        if(n!=0) testcase(n);
    } while(n != 0);
    return 0;
}

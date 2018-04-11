#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>
#include <cmath>
#include <stack>


#include <boost/graph/adjacency_list.hpp>

class myInt {
    public:
	int global_counter = 1;
	static const int INFINITE_VERTEX = 0;
	int vertex_id;
	myInt() {
	    vertex_id = -1;
	}
	int get_global_counter() {
	    return global_counter++;
	}
};

// For CGAL Triangulation
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<myInt,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_Iterator;
typedef Triangulation::Finite_faces_iterator Face_Iterator;
typedef Triangulation::Face_handle Face_handle;

using namespace std;

bool safe_start_point(K::Point_2 start_p, Face_handle f, double min_dist){
	for(int i=0; i<3; i++){
		double dist = CGAL::squared_distance(start_p, f->vertex(i));
		if(dist<min_dist) return false; //ksekiname me sygkroush
	}
	return true;
}


void testcase(size_t n) {
	//Store points, so that they can be inserted more efficiently later
	vector<K::Point_2> infected;
	pts.reserve(n);

	for (size_t i=0; i<n; i++) {
		int x, y;
		cin >> x >> y;
		infected.push_back(K::Point_2(x, y));
	}

	//Construct the triangulation
	Triangulation t;
	t.insert(infected.begin(), infected.end());
	//Construct our graph
	Graph g(t.number_of_faces()+1);

	for (Edge_iterator e = t.finite_edges_begin(); e != finite_edges_end(); e++) {
		CGAL::Face_handle f1 = e.first;
		if (is_infinite(f1))
			f1->info().vertex_id = myInt::INFINITE_VERTEX;
		CGAL::Face_handle f2 = f1.neighbor(e.second);
		if (is_infinite(f2))
			f2->info().vertex_id = myInt::INFINITE_VERTEX;

		Triangulation::Vertex_handle v1 = e.first->vertex((e.second + 1) % 3);
		Triangulation::Vertex_handle v2 = e.first->vertex((e.second + 2) % 3);
		double sq_distance = CGAL::squared_distance(v1->point(), v2->point());
		if (f1->info().vertex_id == -1) {
			f1->info().vertex_id = g.get_global_counter();
		}			
		if (f2->info().vertex_id == -1) {
			f2->info().vertex_id = g.get_global_counter();
		}		

		g.add_edge(f1->info().vertex_id, f2->info().vertex_id, sq_distance);
		g.add_edge(f2->info().vertex_id, f1->info().vertex_id, sq_distance);
	}	//twra exei kataskeyastei olos o grafos
	//Number of users asking for escape route
	int m;
	cin >> m;
	
	for (int i=0; i<m; i++) {
		//Motion planning for each user
		int x, y;	
		double d;	//to min dist pou 8elei aytos o an8rwpos. h aktina tou diskou dhladh	
		cin >> x >> y;
		cin >> d;
		K::Point_2 start_p(x,y);
		Face_handle start_face = locate(start_p);

		Vertex bfs_source = start_face->info().vertex_id;

		if(safe_start_point(start_p, start_face, d)  &&  
				g.bfs(bfs_source, myInt::INFINITE_VERTEX, d)) 	
			cout << "y";
		else	
			cout << "n";		
	}
	cout << endl;
}


int main() {
  while(true){
	int n;
	cin >> n;
	if(n==0) break; //telos programmatos
        testcase(n);	
  }
  return 0;
}


class BFS_Graph{
  enum Color {WHITE,GREY,BLACK};
  const int DUMMY_VERTEX=-1;

  struct Edge{
    int target;   //destination_vertex; //to id toy (noumero) (8esh apo8hkeyshs)
    //put extra fields here
    double capacity;

    Edge(int target, double capacity){
        this->target = target;
	this->capacity = capacity;
    }
  };

  struct Vertex {
    int id;	//se poia 8esh toy pinaka

    vector<Edge> next;  //ta paidia toy, children;
    // BFS properties
    Color color;
    //extra fields here
    Vertex(int id){
	this->id = id;
	color = WHITE;
    }
  };

  public:
    vector<Vertex> vertices;
    BFS_Graph(int n){  	//posous kombous 8a exei o neos grafos
	for(int i=0; i<n; i++)
		vertices.push_back(Vertex(i));
    }

//krataei th domh dhladh ta verticds kai edges alla //arxikopoiei ta ypoloipa: parent, disc time color etc
  void reinitialize(){
	for(auto it=vertices.begin(); it!=vertices.end(); ++it){
		it->color = WHITE;
	}
  }

  void add_edge(int source, int target,double capacity){
	vertices[source].next.push_back(Edge(target, capacity));
  }


void print_graph(){
	cout << "\n\n\nprint Graph";
	for(unsigned int i=0; i<vertices.size(); i++){
		for(unsigned int j=0; j<vertices[i].next.size(); j++){
			cout << vertices[i].next[j].target << ", "; 
		}
		cout << endl;
	}
}

bool bfs(int start_vertex, int finish_vertex, double min_capacity){
    Vertex* u = &vertices[start_vertex];
    u->color = GREY;	//source vertex discovered

    queue<int> Q;
    Q.push(u->id);
    while(!Q.empty()){
    	int current = Q.front();
    	Q.pop();
    	u = &vertices[current];
	if(u->id == finish_vertex) {return true;} //mh synexiseis th bfs
	//print_graph();
    	for(Edge e: u->next) {	//paw sta paidia tou
	    //den xwraei na perasei o diskos opote 
	    if(e.capacity < min_capacity) continue;
	    Vertex* v = &vertices[e.target];
            if(v->color == WHITE){//Classify (u,v) as tree edge (never seen it before)
	    	v->color = GREY;
	    	Q.push(v->id); //kanei push sto telos kai pop apo thn arxh
            }
	} //pros8esa ola ta paidia toy sthn queue.
        u->color = BLACK;
//	cout << u->id <<".color= BLACK\n";
    }// telos tou while loop. queue is empty
    return false;
}

}; //end of class Graph

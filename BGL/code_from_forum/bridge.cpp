#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>	// tuples::ignore
#include <boost/graph/connected_components.hpp>
#include <boost/graph/biconnected_components.hpp>


using namespace std;
using namespace boost;

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}


typedef adjacency_list < vecS, vecS, undirectedS, no_property, property < edge_component_t, std::size_t > >		Graph;
typedef	graph_traits<Graph>::edge_descriptor	Edge;	// Edge type
typedef graph_traits <Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator	EdgeIt;	// Iterator



void solveBiconnect();

bool sortRes(const pair<int,int > &args, const pair<int, int> &arge){

	if(args.first == arge.first){
		return (args.second < arge.second);
	}
	else{
		return (args.first <= arge.first);
	}
}

int main(){
	ios_base::sync_with_stdio(false); 

	int testCases;
	cin >> testCases;
	while(testCases--){
		solveBiconnect();
	}

	return 0;
}


void solveBiconnect(){

	int n, m;

	cin >> n >> m;
	Graph G(n);
	for(int i=0; i<m; i++){
		int start, end;
		cin >> start >> end;
		Edge e;
		tie(e, tuples::ignore) = add_edge(end, start, G);
		
	}


	property_map < Graph, edge_component_t >::type component = get(edge_component, G);

	std::size_t num_comps = biconnected_components(G, component);

	int importantC[num_comps];
	for(int k=0; k<num_comps; k++){
		importantC[k] = -1;
	}
	

	EdgeIt ei, ei_end;
	vector<Edge> vedge;
	// find which edges are critical
	for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei){
		vedge.push_back(*ei);
		// cout << "edge: " << *ei << " comp: " << component[*ei] << endl; 
		if(importantC[component[*ei]] == -1){
			importantC[component[*ei]] = 1;
		}
		else if(importantC[component[*ei]] == 1){
			importantC[component[*ei]] = 0;
		} 
	}



	std::vector<Vertex> art_points;

	articulation_points(G, std::back_inserter(art_points));
	int artP;
	vector<pair<int, int> > res;
	pair<int, int> tempP;

	for (std::size_t i = 0; i < art_points.size(); ++i) {
   		// std::cout << art_points[i] << std::endl;
   		artP = art_points[i];
	   for(int k=0; k<vedge.size(); k++){
	   	// cout << artP << " edge: " << vedge[k] << " component: " << component[vedge[k]] << " allow: "<< importantC[component[vedge[k]]] << endl;
	   		if(importantC[component[vedge[k]]] == 1){
	   			if(artP == source(vedge[k], G) || artP == target(vedge[k], G)){
	   				tempP.first = source(vedge[k], G);
	   				tempP.second = target(vedge[k], G);
	   				res.push_back(tempP);
	   				vedge.erase(vedge.begin() + k);
	   				k--;
	   				// cout << "erase visited" << endl;
	   			}
	   		}
	   		else{
	   			// cout << "erase couse of not 1" << endl;
	   			vedge.erase(vedge.begin() + k);
	   			k--;
	   		}
	   }
  }

  if(res.size() > 0){
		std::sort(res.begin(), res.end(), sortRes);
	}
	cout << res.size() << endl;

	for(int k=0; k<res.size(); k++){
		if(res[k].first > res[k].second){
			cout << res[k].second << " " << res[k].first << endl;
		}
		else{
			cout << res[k].first << " " << res[k].second << endl;
		}
	}


}


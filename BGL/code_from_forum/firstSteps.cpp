#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,
                        no_property,
                        property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator Edgeit;
typedef property_map<Graph, edge_weight_t>::type WeightMap;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIterator;


void test_case();

// bfs that computes a vector of vertices which have longest number of edges from vertex 0
vector<int> bfs(Graph& G, int V) {
  vector<int> color(V,0);
  std::queue<int> bfs_queue;
  bfs_queue.push(0);
  color[0] = 1;
  vector<int>furthest(V,0);
  while(!bfs_queue.empty()) {
    int u = bfs_queue.front();
    bfs_queue.pop();
    OutEdgeIterator eit, eend;
    vector<int> latest;
    bool new_neighbour = 0;
    for (tie(eit, eend) = out_edges(u, G); eit != eend; ++eit) {
      int v = target(*eit, G);
      if(color[v] == 0){
        new_neighbour = 1;
        bfs_queue.push(v);
        color[v] = 1;
        latest.push_back(v);
      }
    }
    if(new_neighbour == 1)
      furthest = latest;
  }
  return furthest;
}

int main()
{
  ios_base::sync_with_stdio(false);
  int T;
  cin>>T;
  while(T--) {
    test_case();
  }
  return 0;
}

void test_case() {
  int V,E;
  cin>>V>>E;
  Graph G(V);
  WeightMap wm = get(edge_weight, G);
  for(int i = 0; i < E; ++i) {
    int u,v,c;
    Edge e;
    cin>>u>>v>>c;
    tie(e,tuples::ignore) = add_edge(u,v,G);
    wm[e] = c;
  }

//Calculation of MST using Kruskal and finding the sum of weights of all edges.
  vector<Edge> mst;
  kruskal_minimum_spanning_tree(G, back_inserter(mst));
  vector<Edge>::iterator ebeg, eend = mst.end();
  int sum_weights = 0;
  for (ebeg = mst.begin(); ebeg != eend; ++ebeg) {
    sum_weights = sum_weights + wm[*ebeg];
  }
//Calculation of the set of possible furthest nodes from 0 (in terms of number of edges)
  vector<int> furthest = bfs(G, V);
  int furthest_dist = 0;
  vector<int> furthest_map(V,0); //value of i-th index is 1 if i is one of the furthest vertices, else 0
  for(size_t i = 0; i < furthest.size(); ++i) {
    furthest_map[furthest[i]] = 1;
  }
//Calculation of shortest paths from 0 to all vertices of G using Dijkstra
  vector<int> dist(V);
  vector<int> pred(V);
  dijkstra_shortest_paths(G, 0,
    predecessor_map(make_iterator_property_map(pred.begin(),
      get(vertex_index, G))).
    distance_map(make_iterator_property_map(dist.begin(),
      get(vertex_index, G))));

//pick the longest distance from the distances of possible furthest vertices
  for(int u = 0; u < V; ++u) {
    if(furthest_map[u]==1) {
      if(dist[u]>furthest_dist)
        furthest_dist = dist[u];
    }
  }

  cout<<sum_weights<<" "<<furthest_dist<<endl;

}

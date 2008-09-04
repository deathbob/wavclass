#include <vector>

using namespace std;

template <class T>
struct vertex{
  vertex(){
    ID = 0;
  };
  vertex(const vertex& copy){
    ID = copy.ID;
    miniwaves.clear();
    miniwaves = copy.miniwaves;
    edges.clear();
    edges = copy.edges;
  }
  void operator=(const vertex& right){
    ID = copy.ID;
    miniwaves.clear();
    miniwaves = right.miniwaves;
    edges.clear();
    edges = right.edges;
  }

  void addEdge(long L ){
    bobEdge newEdge(L, 1);
    vector<bobEdge>::iterator it = find(edges.begin(), edges.end(), newEdge);
    if(it == edges.end()){
      edges.push_back(newEdge);
    }
    else{
      it->weight++;
    }
  }
public:
  vector<miniwave> miniwaves;
  long ID;
  vector<bobEdge> edges;
};

struct bobEdge{
  bobEdge(){
    to = 0;
    weight = 0;
  };
  bobEdge(long L, int W){
    to = L;
    weight = W;
  };
  bobEdge(const bobEdge& copy){
    vertices.clear();
    
  bool operator==(const bobEdge& right){
    if(to = right.to)return true;
    else return false;
  };
public:
  long to;
  int weight;
};


class bobGraph{
  
 public:
  map<long, vertex> vertices;
};

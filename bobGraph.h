#include <vector>

using namespace std;

class bobEdge{
 public:
  bobEdge(){
    to = 0;
    weight = 0;
  };
  bobEdge(long L, int W){
    to = L;
    weight = W;
  };
  bobEdge(const bobEdge& copy){
    to = copy.to;
    weight = copy.weight;
  };

  bool operator==(const bobEdge& right){
    if(to == right.to)return true;
    else return false;
  };
  long to;
  int weight;
};

template <class T>
class vertex{
 public:
  vertex(){
    ID = 0;
  };
  vertex(const vertex& copy){
    ID = copy.ID;
    miniwaves.clear();
    miniwaves = copy.miniwaves;
    edges.clear();
    edges = copy.edges;
  };
  void operator=(const vertex& right){
    ID = right.ID;
    miniwaves.clear();
    miniwaves = right.miniwaves;
    edges.clear();
    edges = right.edges;
  };
  void addEdge(long L ){
    bobEdge newEdge(L, 1);
    vector<bobEdge>::iterator it = find(edges.begin(), edges.end(), newEdge);
    if(it == edges.end()){
      edges.push_back(newEdge);
    }
    else{
      it->weight++;
    }
  };
  void addMW(miniwave<T>& mw){
    if(ID == 0){
      ID = mw.identify();
    }
    miniwaves.push_back(mw);
  };
  vector<miniwave<T> > miniwaves;
  long ID;
  vector<bobEdge> edges;
};



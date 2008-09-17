#include <vector>

using namespace std;

union handy{
  unsigned char Chars[4];
  short int samples[2];
  long int ID;
  void print(){
    cout<<"sampleZero "<<samples[0]<<endl
      <<"sampleOne "<<samples[1]<<endl
	<<"marID "<<ID<<endl
	<<"marChars "
	<<(int)Chars[0]<<" "
	<<(int)Chars[1]<<" "
	<<(int)Chars[2]<<" "
	<<(int)Chars[3]<<endl;
  }
};


class bobEdge{
 public:
  bobEdge(){
    to = 0;
    weight = 0;
  };
  bobEdge(signed short L, int W){
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
  signed short to;
  int weight;
};

template <class T>
class vertex{
 public:
  vertex(){
    ID = 0;
    vertexUnion.ID = 0;
  };
  vertex(const vertex& copy){
    ID = copy.ID;
    edges.clear();
    edges = copy.edges;
    vertexUnion.ID = copy.vertexUnion.ID;
  };
  vertex(const handy mu){
    ID = mu.ID;
    vertexUnion.ID = mu.ID;
  };
  void operator=(const vertex& right){
    ID = right.ID;
    edges.clear();
    edges = right.edges;
    vertexUnion.ID = right.vertexUnion.ID;
  };
  void setID(long int xID){
    ID = xID;
    vertexUnion.ID = xID;
  }
  void addEdge(signed short L ){
    bobEdge newEdge(L, 1);
    //    vector<bobEdge>::iterator it = find(edges.begin(), edges.end(), newEdge);
    //    if(it == edges.end()){
      edges.push_back(newEdge);
      //    }
      //    else{
      //      it->weight++;
      //    }
  };

  handy vertexUnion;
  long int ID;
  vector<bobEdge> edges;
};



#include <vector>

using namespace std;

union marU{
  unsigned char marChars[4];
  signed short int samples[2];
  unsigned long int marID;
  void printMarU(){
    cout<<"sampleOne "<<samples[0]<<endl
      <<"sampleTwo "<<samples[1]<<endl
	<<"marID "<<marID<<endl
	<<"marChars "
	<<(int)marChars[0]<<" "
	<<(int)marChars[1]<<" "
	<<(int)marChars[2]<<" "
	<<(int)marChars[3]<<endl;
  }
};


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
    vertexUnion.marID = 0;
  };
  vertex(const vertex& copy){
    ID = copy.ID;
    edges.clear();
    edges = copy.edges;
    vertexUnion.marID = copy.vertexUnion.marID;
  };
  vertex(const marU mu){
    ID = mu.marID;
    vertexUnion.marID = mu.marID;
  };
  void operator=(const vertex& right){
    ID = right.ID;
    edges.clear();
    edges = right.edges;
    vertexUnion.marID = right.vertexUnion.marID;
  };
  void setID(unsigned long int xID){
    ID = xID;
    vertexUnion.marID = xID;
  }
  void addEdge(long L ){
    bobEdge newEdge(L, 1);
    //    vector<bobEdge>::iterator it = find(edges.begin(), edges.end(), newEdge);
    //    if(it == edges.end()){
      edges.push_back(newEdge);
      //    }
      //    else{
      //      it->weight++;
      //    }
  };

  marU vertexUnion;
  unsigned long int ID;
  vector<bobEdge> edges;
};



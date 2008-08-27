#ifndef BOB_MINIWAV_H
#define BOB_MINIWAV_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

template<class T>
class miniwave
{
 public:
  miniwave(){
    identifier = 0;
    predecessor = "";
};
  miniwave(miniwave const& copy ){
    nameStream << copy.nameStream.str();
    samples = copy.samples;
    identifier = copy.identifier;
    predecessor = copy.predecessor;
  }
  miniwave operator=(miniwave const& right){
    samples = right.samples;
    nameStream.clear();
    nameStream << right.nameStream.str();
    predecessor = right.predecessor;
  }
  void addSample(const T& sampleToAdd){
    samples.push_back(sampleToAdd);
    nameStream << (sampleToAdd % 255);
    identifier += (sampleToAdd % 64);
  };
  string name(){
    return nameStream.str();
  }; 
  long identify(){
    return identifier;
  };
  string getPredecessor(){
    return predecessor;
  };
  string setPredecessor(const string& s){
    predecessor = s;
  };
 private:
  vector<T> samples;
  ostringstream nameStream; 
  long identifier;
  string predecessor;
};


#endif

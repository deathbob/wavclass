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
  miniwave(){};
  miniwave(miniwave const& copy ){
    nameStream << copy.nameStream.str();
    samples = copy.samples;
  }
  miniwave operator=(miniwave const& right){
    samples = right.samples;
    nameStream.clear();
    nameStream << right.nameStream.str();
  }
    
  void addSample(const T& sampleToAdd){
    samples.push_back(sampleToAdd);
    nameStream << sampleToAdd;
  };
  string name(){
    return nameStream.str();
  }; 

 private:
  vector<T> samples;
  ostringstream nameStream; 
};


#endif

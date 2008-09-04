#ifndef BOB_MINIWAV_H
#define BOB_MINIWAV_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

template<class T>
class miniwave
{
 public:
  miniwave(){
    identifier = 0;
  };
  miniwave(const miniwave& copy ){
    samples.clear();
    for(unsigned int i = 0; i < copy.samples.size(); i++){
      samples.push_back(copy.samples[i]);
    }
    identifier = copy.identifier;
  };
  void operator=(const miniwave& right){
    samples.clear();
    for(unsigned int i = 0; i < right.samples.size(); i++){
      samples.push_back(right.samples[i]);
    }
    identifier = right.identifier;
  };
  bool operator==(const miniwave& right){
    if (identifier == right.identifier) return true;
    else return false;
  };
  ~miniwave(){
    samples.clear();
  };
  void addSample(const T sampleToAdd){
    samples.push_back(sampleToAdd);
    identifier += (sampleToAdd % 100);
  };
  long identify(){
    return identifier;
  };

 private:
  vector<T> samples;
  long identifier;
};

#endif

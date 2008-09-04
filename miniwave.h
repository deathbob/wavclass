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
    predecessor = 0;
    followedBy = 0;
  };
  miniwave(const miniwave& copy ){
    samples.clear();
    for(unsigned int i = 0; i < copy.samples.size(); i++){
      samples.push_back(copy.samples[i]);
    }
    identifier = copy.identifier;
    predecessor = copy.predecessor;
    followedBy = copy.followedBy;
  };
  void operator=(const miniwave& right){
    samples.clear();
    for(unsigned int i = 0; i < right.samples.size(); i++){
      samples.push_back(right.samples[i]);
    }
    predecessor = right.predecessor;
    identifier = right.identifier;
    followedBy = right.followedBy;
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
  long getPredecessor(){
    return predecessor;
  };
  void setPredecessor(const long s){
    predecessor = s;
  };
  long getFollowedBy(){
    return followedBy;
  };
  void setFollowedBy(const long l){
    followedBy  = l;
  };

  };
 private:
  bobGraph bob;
  vector<T> samples;
  long identifier;
  long predecessor;
  long followedBy;
};

#endif

#ifndef BOB_MINIWAV_H
#define BOB_MINIWAV_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

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
  miniwave(miniwave const& copy ){
    //    nameStream << copy.nameStream.str();
    samples = copy.samples;
    identifier = copy.identifier;
    predecessor = copy.predecessor;
    followedBy = copy.followedBy;
  };
  miniwave operator=(miniwave const& right){
    samples = right.samples;
    //    nameStream.clear();
    //    nameStream << right.nameStream.str();
    predecessor = right.predecessor;
    identifier = right.identifier;
    followedBy = right.followedBy;
  };
  bool operator==(miniwave const& right){
    if (identifier == right.identifier) return true;
    else return false;
  };
  void addSample(const T& sampleToAdd){
    samples.push_back(sampleToAdd);
    //    nameStream << (sampleToAdd % 2);
    identifier += (sampleToAdd % 100);
  };
/********
  string name(){
    string temp = nameStream.str(); 
    return temp;
  }; 
*/
  long identify(){
    return identifier;
  };
  long getPredecessor(){
    return predecessor;
  };
  long setPredecessor(const long& s){
    predecessor = s;
  };
  long getFollowedBy(){
    return followedBy;
  }
  long setFollowedBy(const long& l){
    followedBy  = l;
  }
 private:
  vector<T> samples;
  //  stringstream nameStream; 
  long identifier;
  long predecessor;
  long followedBy;
};


#endif

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
  miniwave(){identifier = 0;};
  miniwave(miniwave const& copy ){
    nameStream << copy.nameStream.str();
    samples = copy.samples;
    identifier = copy.identifier;
    // the internet has 1000 dark corners, niches.
    // 5 or 6 wide open places.
    // Confederate flag buyers.  
  }
  miniwave operator=(miniwave const& right){
    samples = right.samples;
    nameStream.clear();
    nameStream << right.nameStream.str();
  }

  void addSample(const T& sampleToAdd){
    samples.push_back(sampleToAdd);
    nameStream << sampleToAdd % 255;
    identifier += sampleToAdd % 64;
  };
  string name(){
    return nameStream.str();
  }; 
  long identify(){
    return identifier;
  };

 private:
  vector<T> samples;
  ostringstream nameStream; 
  long identifier;
};


#endif

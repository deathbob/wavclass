#ifndef BOBWAVE_H
#define BOBWAVE_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <map>
#include "miniwave.h"
#include "bobGraph.h"

using namespace std;

class wave
{
 public:
  wave();
  wave(string fileName);
  void setDataLength(float length);
  void setDataLength();
  void writeW(string fileName);
  void print();
  void markovAte();
  void scramble();
  void operator+=(const wave& right);
  wave operator=(const wave& right);

 private:
  vector<char> header;
  vector<char> dataHead;
  vector<char> infoBlock;
  vector<unsigned char> buffer;
  map<long long, vector<short> > markov;

  unsigned long int dataLength;
  unsigned long int totalSize ;
  unsigned int fmtSize;
  unsigned int compressionType;
  unsigned int channels;
  unsigned int sampleRate ;
  unsigned int bytesPerSec;
  unsigned int byteDepth;
  unsigned int bitDepth;
};

#endif

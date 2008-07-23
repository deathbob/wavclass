#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <deque>

using namespace std;

class wave
{
 public:
  wave();
  wave(int channels, int samplerate, int bitspersample);
  wave(string fileName);

  void generateSine();
  void setDesiredFreq(float freq);
  void setDataLength(float length);
  void writeW(string fileName);
  void print();
  void generateSquare();
  void generateTriangle();
  void generateTriSine();			
  wave operator+(wave&);
  wave operator-(wave&);
  void reverseWav();
  //shiftPhase - shift the starting point of the wav by a percentage of the 
  //full period.  Take 1 - 100 input.  
  void shiftPhase(int percentToShift);
  void play();

  float LFO; //  should be less than 1, greater than 0
  // Modifying desiredFreq by the addition of the LFO makes cool ring 
  // modulated type noises.  
  deque<char> header;
  deque<char> dataHead;
  deque<char> infoBlock;
  deque<char> buffer;

 private:
  int counter;
  bool tickOver;
  int maxValue, minValue;
  unsigned int dataLength;
  float fullPeriod;
  unsigned int totalSize ;
  unsigned int fmtSize;
  unsigned int compressionType;
  unsigned int channels;
  unsigned int sampleRate ;
  unsigned int bytesPerSec;
  unsigned int byteDepth;
  unsigned int bitDepth;
  float desiredFreq;

};


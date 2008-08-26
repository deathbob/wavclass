/*
Copyright 2007, 2008 Bob Larrick
larrick@gmail.com
    This file is part of wavclass.

    wavclass is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    wavclass is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with wavclass.  If not, see <http://www.gnu.org/licenses/>.

********/

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
#include <deque>
#include <map>
#include "miniwave.h"


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
  void shiftPhase(int percentToShift);
  void play();
  void markovAte();
  void scramble();
  float LFO; //  should be less than 1, greater than 0
  deque<char> header;
  deque<char> dataHead;
  deque<char> infoBlock;
  deque<unsigned char> buffer;
    //  deque<short> buffer;
  // list pros and cons of using a string or a long as the key
  //
  map<string, vector<miniwave<short> > > markov;
  
 private:
  int counter;
  bool tickOver;
  short maxValue, minValue;
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

#endif

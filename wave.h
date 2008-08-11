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


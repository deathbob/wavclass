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
#include "wave.h"

using namespace std;

int main()
{
  /*
	wave sub1, sub2, sub3;
	sub1.setDesiredFreq(200);
	sub2.setDesiredFreq(400);
	sub1.setDataLength(4);
	sub2.setDataLength(5);
	sub1.generateSine();
	sub2.generateSine();
	sub3 = sub1 - sub2;
	//	sub3.write("sub3.wav");
	wave sub4;
	sub4 = sub2 - sub1;
	//	sub4.write("sub4.wav");
	//	sub1.write("sub1.wav");
	//	sub2.write("sub2.wav");
	sub3.reverseWav();
	sub2.write("exp2.wav");

	wave tester;
	tester.setDataLength(1);
	tester.setDesiredFreq(600);
	tester.generateSine();
	tester.write("tester1.wav");
	tester.shiftPhase(33);
	tester.write("tester2.wav");
  */

  /*
  int nuke = 44100;
  char bang[4] = {'0','0','0','0'}
  *((unsigned int*)(&bang[0])) = nuke;
  cout<< (int)bang[0]<<  " "<<(int)bang[1]<< " "<<(int)bang[2]<< " "<<(int)bang[3]<<endl;
  */

  /*
  char thom[4] = {0,0,0,0};
  int ally = 352800;
  *((int*)(&thom[0])) = ally;
  for(int i = 0; i < sizeof(thom); i++)
    {
      cout<<(int)thom[i]<<endl;
    }
  */

  wave bob1(1, 44100, 16);
  bob1.setDataLength(1);
bob1.setDesiredFreq(200);
  bob1.generateSine();

  wave bob2(1, 44100, 16);
  bob2.setDataLength(1);
  bob2.setDesiredFreq(10);
  bob2.generateTriangle();
  bob2.writeW("BobAug13-1.wav");


  //  bob1.LFO = .001;
  //  bob1.generateTriSine();



  //  bob1.play();
  string  fileName = "BobAug_12-1.wav";
  bob1.writeW(fileName);


}
	

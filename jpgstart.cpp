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
	wave bob, gretchen, baby;
	// setDesiredFreq in hertz
	bob.setDesiredFreq(400);
	gretchen.setDesiredFreq(1200);
	// setDataLength as the length you want the sound to be, in seconds.  
	// float data type, so if you want a 3.751 second sound, that is ok.  
	bob.setDataLength(3);
	gretchen.setDataLength(2);
	bob.generateSine();
	gretchen.generateSine();
	baby = bob + gretchen;
	//	baby.write("baby.wav");
	baby.generateSine();
	//	baby.write("baby2.wav");

	wave ted("baby2.wav");
	ted.write("baby2Redux.wav");
	wave crow = baby + ted;
	//	crow.write("Crow.wav");


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
	sub2 = sub3 - baby;
	sub2.write("exp.wav");
	sub3.reverseWav();
	sub2 = sub3 + baby;
	sub2.write("exp2.wav");

	wave tester;
	tester.setDataLength(1);
	tester.setDesiredFreq(600);
	tester.generateSine();
	tester.write("tester1.wav");

	tester.shiftPhase(33);
	tester.write("tester2.wav");




  bob2.shiftPhase(25);
  bob2.write("1bob2.wav");
  bob1.write("1bob1.wav");
  bob3 = bob1 - bob2;
  wave bob4;
  bob4 = bob1 + bob2;
  bob3.write("1bob3.wav");
  bob4.write("1bob4.wav");
  */


  //  int nuke = 44100;

  /*
  int nuke = 44100;
  char bang[4];
  bang[0] = 0;
  bang[1] = 0;
  bang[2] = 0;
  bang[3] = 0;

  *((unsigned int*)(&bang[0])) = nuke;

  cout<< (int)bang[0]<<  " "<<(int)bang[1]<< " "<<(int)bang[2]<< " "<<(int)bang[3]<<endl;

  */


  //    wave outsider("pinkmask.wav");
  //wave webwav("testsnd.wav");

  // webwav.print();
  /*
  char thom[4] = {0,0,0,0};
  int ally = 352800;
  *((int*)(&thom[0])) = ally;
  
  for(int i = 0; i < sizeof(thom); i++)
    {
      cout<<(int)thom[i]<<endl;
    }
  */


  wave bob1(1,44100,16);
  bob1.setDataLength(.5);

  bob1.setDesiredFreq(100);

  //    bob1.LFO = .001;
  //  bob1.generateTriSine();

  bob1.generateSine();

  //  bob1.play();
  string  fileName = "BobJuly_03.wav";
  bob1.writeW(fileName);


}
	

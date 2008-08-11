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
#include "wave.h"
#include <algorithm>
#include <sstream>
//#include "cmath"
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

// default constructor, sets values for things that need to be in 
// the .wav file when it is finally written that are not changed.
// Need to write support for higher sampleRate (44100), stereo
// files, and greater bit depth (16 or 32 bits per sample).
wave::wave()
{
  char tempHeader[36] =
    {
      'R','I','F','F', // always RIFF
      0,0,0,0,	       // size of the entire file (determined later)
      'W','A','V','E', // always WAVE
      'f','m','t',' ', // always "fmt "
      16,0,0,0,        // 16 more bytes, not including the ones used for this field
      1,0,	       // 1 for PCM encoding

      1,0,	       // 1 for mono, 2 for stereo

      // 68,-84,0,0,   // sample rate (-128, 62, 0 ,0) for 16000 samples
      68, -84, 0, 0,   // per second, 68, -84,0,0 for 44100
                       // 34, 86, 0 ,0 for 22050
                       // 64, 31, 0, 0 for 8000

      32, 98, 5, 0,    // -120,88,1,0, 
                       // Bytes per Second, 0,125,0,0 for 32000
                       // -120, 88, 1 ,0 for 88200
                       // 44100 * 1 * 16 / 2 = 352800
                       // 352800 = 32, 98, 5, 0
                       // SampleRate * NumChannels * BitsPerSample/8

      2,0,	       // BlockAlign == NumChannels * BitsPerSample/8
      16,0	       // oBitsPerSample, usually 8 or 16
    };



  // Moves the data in the array tempHeader into the vector header.
  // I did it this way because (i think) you can't initialize a 
  // vector?  I don't remember now why i wanted it to be a vector
  // instead of an array :)  Oh now i do, because i would have to
  // overload a bunch of operators if I leave it as an array, but
  // as a vector it is taken care of for me.
  for(int i = 0; i < 36; i++)
  {
    header.push_back(tempHeader[i]);
  }
  
  char tempDataHead[8] = { 'd','a','t','a',0,0,0,0 };
  for(int i = 0; i < 8; i++)
    {
      dataHead.push_back(tempDataHead[i]);
    }

  // I have no idea if this is a good way to access the data.
  // I don't know anything about casts and if i'm doing it the 
  // right way or not, but i found some info on how to cast things
  // online, and it's getting the job done.  It seems confusing
  // and feels a little dangerous.  
  dataLength = *((unsigned int*)(&dataHead[4]));
  totalSize = *((unsigned int *)(&header[4]));
  fmtSize = *((unsigned int *)(&header[16]));
  // Not sure if adding chars together is the right way to get this
  // data either.  Again, found some info online and it seems to 
  // be working, but unsure if this is the right way / best way
  // to do it.  
  compressionType = header[20] + header[21];
  channels = header[22] + header[23];
  sampleRate =  *((unsigned int *)(&header[24]));
  bytesPerSec = *((unsigned int *)(&header[28]));
  byteDepth = header[32] + header[33];
  bitDepth = header[34] + header[35];

  desiredFreq = 434;
  fullPeriod = (int)(0.5 + (sampleRate/desiredFreq));
  tickOver= false;
  counter = 0;
  //  maxValue = 255;
  //  minValue = 0;
  LFO = 0;

  char tempInfoBlock[12] = {'i','n','f','o',8,0,0,0,'b','l','a','h'};
  for (int i = 0; i < 12; i++)
    {
      infoBlock.push_back(tempInfoBlock[i]);
    }

  double tempValue = pow(2.0,(double)bitDepth);
  //  cout<<"tempValue "<<tempValue<<endl;


  if(tempValue > 256)
    {
      maxValue = (int)((tempValue/2)-1);
      minValue = -maxValue;
    }
  else
    {
      maxValue = (int)tempValue;
      minValue = 0;
    }
};  // End of wave::wave(){};


wave::wave(int chan, int sampler, int bitspers){
  char tempHeader[36] =
    {
      'R','I','F','F', // always RIFF // 0 - 3
      0,0,0,0,	       // size of the entire file (determined later) 4- 7
      'W','A','V','E', // always WAVE  // 8 - 11
      'f','m','t',' ', // always "fmt " // 12 - 15
      16,0,0,0,        // 16 more bytes, not including the ones used for 
                       // this field  //  16 - 19

      1,0,	       // 1 for PCM encoding  // 20 - 21

      0,0,	       // overwrite this with channels // 22 - 23


      0,0,0,0,         //overwrite this with samplerate  // 24 - 27


      0,0,0,0,         // Bytes per Second  //  28 - 31
                       // Needs to be calculated and overwrite here
                       // channels * samplerate * bitspersample / 8

      0,0,	       // BlockAlign, needs to be calculated  // 32 - 33
                       // AKA bytesPerSec
                       // Overwrite with channels * bitspersample / 8

      0,0	       // bitspersample, needs to be overwritten.  // 34 - 35
                       // AKA bitDepth

    };

  *((int*)(&tempHeader[22])) = chan;
  *((int*)(&tempHeader[24])) = sampler;
  *((int*)(&tempHeader[28])) = chan*sampler*bitspers / 8;
  *((int*)(&tempHeader[32])) = chan * bitspers / 8;
  *((int*)(&tempHeader[34])) = bitspers;
  LFO = 0;
  totalSize = 0;
  dataLength = 0;
  fullPeriod = 0;
  desiredFreq = 0;

  fmtSize = 16;
  compressionType = 1;
  channels = chan;
  sampleRate = sampler;
  bytesPerSec = chan * sampler * bitspers / 8;
  byteDepth = bitspers / 8;
  bitDepth = bitspers;

  counter = 0;
  tickOver = 0;

  double tempValue = pow(2.0,(double)bitDepth);
  if(tempValue > 256)
    {
      maxValue = (int)((tempValue/2)-1);
      minValue = - maxValue;
    }
  else
    {
      maxValue = (int)tempValue;
      minValue = 0;
    }

  for(int i = 0; i < 36; i++)
  {
    header.push_back(tempHeader[i]);
  }
  char tempDataHead[8] = { 'd','a','t','a',0,0,0,0 };
  for(int i = 0; i < 8; i++)
    {
      dataHead.push_back(tempDataHead[i]);
    }
  char tempInfoBlock[12] = {'i','n','f','o',8,0,0,0,'b','l','a','h'};
  for (int i = 0; i < 12; i++)
    {
      infoBlock.push_back(tempInfoBlock[i]);
    }



};// end of wave(int channels, int samplerate, int bitspersample)




wave::wave(string fileName)
{
  ifstream fileIn;
  fileIn.open(fileName.c_str(), ios::binary);

  for(int i = 0; i < 36; ++i)
    {
      char temp = fileIn.get();
      //      cout<<i<< " "<<(int)temp<<endl; 
      header.push_back(temp);
    }
  //  cout<<endl;

  totalSize = *((unsigned int*)(&header[4]));
  compressionType = header[20] + header[21];
  channels = header[22] + header[23];
  sampleRate = *((unsigned int *)(&header[24]));
  bytesPerSec = *((unsigned int*)(&header[28]));
  byteDepth = header[32] + header[33];
  bitDepth = header[34] + header[35];

  for(int i = 0; i < 8; ++i)
    {
      char temp = fileIn.get();
      //      cout<<temp<< " "<<(int)temp<<endl;
      dataHead.push_back(temp);
    }


  dataLength = *((unsigned int*)(&dataHead[4]));

  double tempValue = pow(2.0,(double)bitDepth);
  
cout<<"tempValue "<<tempValue;

  if(tempValue <= 256)
    {
      maxValue = (int)((tempValue / 2)-1);
      minValue = -maxValue;
    }
  else
    {
      maxValue = (int)tempValue;
      minValue = 0;
    }

  for(int i = 0; i < dataLength; ++i)
    {
      char temp = fileIn.get();
      buffer.push_back(temp);
    }
  //  cout<<endl;
  char temp;

  while((temp = fileIn.get()) != char_traits<char>::eof())
    {
      infoBlock.push_back(temp);
      //     cout<<temp<<" "<<(int)temp<<endl;
    }
  fileIn.close();
  LFO = 0;
  
  desiredFreq = 1;
  fullPeriod = (int)(0.5 + (sampleRate/desiredFreq));
  tickOver= false;
  counter = 0;
}


  
void wave::generateSine()
{
  // clear the buffer or the data will get appended w/o 
  // updating the length of the data (dataLength).
  buffer.clear(); 
  //  vector<float> sinLUT;
  float PI = 3.1415926535;
  float rad = 2 * PI;
  float temp = 0.0;
  //  float pTemp = 0.0;

  //  int radMax = (int)(rad * 10000);
  //  sinLUT.reserve(radMax);
  /*
  for(int i = 0; i < radMax ; ++i)
{
  //  temp = (i * PI / 180);
  pTemp = sin(i);
  temp = pTemp;
  sinLUT.push_back(temp);
} 
  */

  tickOver = false;
  counter = 0;
  float  tempLFO = LFO;
  float tempDesiredFreq = 0;
  cout<<maxValue<<" maxValue "<<endl;
  cout<<minValue<<" minValue "<<endl;
  for (unsigned int i = 0; i < dataLength / byteDepth; i++)
    {

      /*
      cout<<"*******"<<endl;
      temp = (rad * desiredFreq / sampleRate * counter);
      cout<<temp<<" temp "<<endl;
      temp = temp * 10000;
      pTemp = sinLUT[(int)temp];
      cout<<pTemp<<" pTemp "<<endl;
      */

      temp = sin(rad * desiredFreq / sampleRate * counter);

      //      cout<<temp<<" temp "<<endl;
      //      cout<<"*******"<<endl;


      temp = temp * maxValue/2 ;      
      if (temp > maxValue)
	temp = maxValue;
      if(temp < minValue)
	temp = minValue;

      //      cout<<temp<<" temp "<<endl;

      char tempArr[2];
      *((int*)(&tempArr[0])) = (int)temp;

      buffer.push_back(tempArr[0]);
      buffer.push_back(tempArr[1]);     


      // int tempCheck = *((unsigned int*)(&tempArr[0]));

      //    desiredFreq += LFO;
       //       LFO+= .000001;
       //       if(LFO > .5)
       //       LFO = tempLFO;

      if(counter >= fullPeriod)
	{
	  counter = 0;
	}
      counter++;
  fullPeriod = (int)(0.5 + (sampleRate/desiredFreq));
    }
  //  cout<<"LFO at end of Generate Sine "<<LFO<<endl;
  //  cout<<"Desired Freq at end "<<desiredFreq<<endl;
  LFO = tempLFO;
  desiredFreq = tempDesiredFreq;

};

//  changes the frequency of the wave.  
void wave::setDesiredFreq(float freq)
{
  desiredFreq = freq;
  // add 0.5 so that when cast to int it is rounded up.  
  fullPeriod = (int)(0.5 + (sampleRate/desiredFreq));
};

// change the value of dataLength, and write that value
// into the appropriate headers.  The value of "length" is the
// length in seconds the noise generated should last.  
void wave::setDataLength(float length)
{
  dataLength = (int)(length * bytesPerSec);
  *((unsigned int*)(&header[4])) = dataLength + 36 + 12;
  *((unsigned int*)(&dataHead[4])) = dataLength;

};


// Writes the wave to disk, into a file specified by fileName.    
void wave::writeW(string fileName)
{
  ofstream fileOut;
  // spent hours looking for a way to be able to specify at the command
  // prompt or terminal what file to write to.  Not using that functionality 
  // now, but .c_str() eluded me for a long time.  
  fileOut.open(fileName.c_str(), ios::out | ios::binary);

  copy(header.begin(), header.end(), ostream_iterator<unsigned char>(fileOut));
  copy(dataHead.begin(), dataHead.end(), ostream_iterator<unsigned char>(fileOut));
  copy(buffer.begin(), buffer.end(), ostream_iterator<unsigned char>(fileOut));
  copy(infoBlock.begin(),infoBlock.end(), ostream_iterator<unsigned char>(fileOut));	 
  fileOut.close();
};

// mainly used for debugging.  Prints various info about the wave.  
void wave::print()
{
  cout<<"desiredFreq & dataLength ";
  cout<<desiredFreq<<"  "<<dataLength<<endl;
  cout<<"buffer.size() ";
  cout<<buffer.size()<<endl;
  //  cout<< " sizeof(char) ";
  //  cout<<sizeof(char)<<endl;
  //  cout<<"size of a / b ";
  //  cout<< sizeof(buffer) / (sizeof(char))<<endl;
  cout<<"contents of header \n";
  cout<<left;
  for(int i = 0; i < 36; i++)
    {
      cout<<left<<setw(7)<<header[i]<<(int)header[i]<<endl;
    }
  cout<<endl<<"contents of dataHead \n";
  for(int i = 0; i< 8; i++)
    {
      cout<<left<<setw(7)<<dataHead[i]<<setw(7) <<(int)dataHead[i]<<endl;
    }
  cout<<endl<<"contents of infoBlock \n";
  for (int i = 0; i < 12; i++)
    {
      cout<<setw(7)<<infoBlock[i]<<setw(7)<<(int)infoBlock[i]<<endl;
    }
  cout<<endl;
}

// Not much to see here, easiest wave form to generate.  
// Looking at it now, there's probably an even easier way, 
// I'll think about it.  
void wave::generateSquare()
{
  buffer.clear();
  tickOver = false;
  for (unsigned int i = 0; i < dataLength; i++)
    {
      int flopper = (i % (int)fullPeriod);
      if(flopper == 0)tickOver = !tickOver;
      if(tickOver)
	buffer.push_back(maxValue) ;
      else 
	buffer.push_back(minValue) ;
    }
}

// Pretty straight forward as far as what this function does. 
// Maybe it could be improved, but I don't know how.  
void wave::generateTriangle()
{
  buffer.clear();
  tickOver = false;
  float temp = 0;
  counter = 0;
  for(unsigned int i = 0; i < dataLength; i++)
    {		
      temp = (float)counter/(float)fullPeriod;
      temp = ((temp * maxValue));
      // fullPeriod += LFO;

      if (temp > maxValue)
	temp = maxValue;
      if(temp < minValue)
	temp = minValue;

      buffer.push_back((char)temp);	
      int flopper = (i % (int)fullPeriod);
      if (flopper == 0)
	tickOver = !tickOver;
      if (tickOver)
	counter++;
      else 
	counter--;
    }
}

//  I forget what exactly this function does.  Judging by the name 
// alone, it's my attempt to generate a wave that is half triangle, 
// half sine wave.  
void wave::generateTriSine()
{
  buffer.clear();
  float temp = 0.0;
  tickOver = false;
  counter = 0;
  float PI = 3.1415926535;
  float rad = 2 * PI;
  for(unsigned int i = 0; i < dataLength; i++)
    {		
      temp = (float)counter/(float)fullPeriod;
      float temp1 = abs(sin(rad * desiredFreq / sampleRate * counter));
      
      temp = temp * (maxValue);
      temp1 = temp1 * (maxValue/2);
      if (tickOver)
	temp = temp1;
      
      if (temp > maxValue)
	temp = maxValue;
      if(temp < minValue)
	temp = minValue;

      buffer.push_back((char)temp);	
      
      int flopper = (counter % (int)fullPeriod);
      if (flopper == 0)
	tickOver = !tickOver;
      
      if (tickOver)
	counter++;
      else 
	counter--;
    }
}

// Gotta overload the old addition operator.
// Not sure if this is a good way, or even the right way to add
// waves to get a half and half mix, but it does do something.
// I need to read a lot more about sound synthesis, this just 
// makes things sound horrible!!!
wave wave::operator+( wave& right) 
{
  wave result;

  int i = 0;
  if (buffer.size() <= right.buffer.size())
    {
      // this is probably why i wanted the header as a vector instead
      // of an array, so i could use = instead of writing a loop here.  
      result.header = right.header;
      result.dataHead = right.dataHead;
      result.dataLength = right.dataLength;		
      
      for(i = 0; i < buffer.size(); i++)
	{
	  result.buffer.push_back((buffer[i] + right.buffer[i]) / 2);
	}
      for(i; i < right.buffer.size(); i++)
	{
	  result.buffer.push_back(right.buffer[i]);
	}
    }
  else
    {
      result.dataLength = dataLength;
      result.header = header;
      result.dataHead = dataHead;
      //      result.buffer.reserve(dataLength);
      for (i = 0; i < right.buffer.size(); i++)
	{
	  result.buffer.push_back((buffer[i] + right.buffer[i])/2);
	}
      for (i; i < buffer.size();i++)
	{
	  result.buffer.push_back(buffer[i]);
	}
    }
  int tempFreq =(int)(desiredFreq + right.desiredFreq) / 2;
  result.setDesiredFreq(tempFreq);
  return result;
}	

wave wave::operator-( wave& right)
{
  wave result;
  int i = 0;
  if(buffer.size() <= right.buffer.size())
    {
      result.header = right.header;
      result.dataHead = right.dataHead;
      result.dataLength = right.dataLength;

      for(i = 0; i < buffer.size(); ++i)
	{
	  result.buffer.push_back(abs(buffer[i] - right.buffer[i]) );
	}
      for(i; i < right.buffer.size(); ++i)
	{
	  result.buffer.push_back(right.buffer[i]);
	}
    }
  else
    {
      result.dataLength = dataLength;
      result.header = header;
      result.dataHead = dataHead;
      //      result.buffer.reserve(dataLength);
      for (i = 0; i < right.buffer.size(); ++i)
	{
	  result.buffer.push_back(abs(buffer[i] - right.buffer[i])  );
	}
      for(i; i < buffer.size();++i)
	{
	  result.buffer.push_back(buffer[i]);
	}
    }
  int tempFreq = (int)(desiredFreq + right.desiredFreq) / 2;
  result.setDesiredFreq(tempFreq);
  return result;
}

void wave::reverseWav()
{
  reverse(buffer.begin(), buffer.end());
}


void wave::shiftPhase(int percentToShift)
{
  vector<int> tempV;
  for(int i = 0; i < fullPeriod; ++i)
    {
      tempV.push_back(buffer[i]);
    }
  float modBy = ((float)percentToShift / 100.00) * fullPeriod;
  int modifyBy = (int)(modBy + .5);

  for(int i = 0; i < modifyBy; ++i)
    {
      buffer.push_front(tempV.back());
      tempV.pop_back();
    }

  dataLength += modifyBy;

  *((unsigned int*)(&header[4])) = dataLength + 36 + 12;
  *((unsigned int*)(&dataHead[4])) = dataLength;
}

void wave::play()
{


  int fd = open("/dev/dsp", O_RDWR);

  //  int status = ioctl(fd, SOUND_PCM_READ_RATE);

  int tempCow = 0;

  int status = 0;

  status = ioctl(fd, SOUND_PCM_READ_RATE, tempCow);
  status = ioctl(fd, SOUND_PCM_READ_BITS, tempCow);
  status = ioctl(fd, SOUND_PCM_READ_CHANNELS, tempCow);
  status = ioctl(fd, SOUND_PCM_SYNC, 0);
  status = ioctl(fd, SOUND_PCM_WRITE_RATE, sampleRate);
  status = ioctl(fd, SOUND_PCM_WRITE_BITS, bitDepth);
  status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, channels);

  signed int buf[buffer.size()];

  for (int i = 0; i < buffer.size(); i++)
    {
      buf[i] = buffer[i];
    }

  status = write(fd, buf, buffer.size());
  
  status = close(fd);

}

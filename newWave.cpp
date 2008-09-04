#include "newWave.h"
#include <algorithm>

using namespace std;

wave::wave(){
	char tempHeader[36] = {
		'R','I','F','F', // always RIFF
		0,0,0,0,	 // size of the entire file (determined later)
		'W','A','V','E', // always WAVE
		'f','m','t',' ', // always "fmt "
		16,0,0,0,        // 16 more bytes, not including these
		1,0,	         // 1 for PCM encoding
		1,0,	         // 1 for mono, 2 for stereo
		68, -84, 0, 0,   // per second, 68, -84,0,0 for 44100
		-120, 88, 1, 0,  // Bytes per Second -120, 88, 1 ,0 for 88200
		                 // SampleRate * NumChannels * BitsPerSample/8
		2,0,	         // BlockAlign == NumChannels * BitsPerSample/8
		16,0	         // BitsPerSample, usually 8 or 16
	};
	for(int i = 0; i < 36; i++){
		header.push_back(tempHeader[i]);
	}
	char tempDataHead[8] = { 'd','a','t','a',0,0,0,0 };
	for(int i = 0; i < 8; i++){
		dataHead.push_back(tempDataHead[i]);
	}
	dataLength = *((unsigned int*)(&dataHead[4]));
	totalSize = *((unsigned int *)(&header[4]));
	fmtSize = *((unsigned int *)(&header[16]));
	compressionType = header[20] + header[21];
	channels = header[22] + header[23];
	sampleRate =  *((unsigned int *)(&header[24]));
	bytesPerSec = *((unsigned int *)(&header[28]));
	byteDepth = header[32] + header[33];
	bitDepth = header[34] + header[35];
	char tempInfoBlock[12] = {'i','n','f','o',8,0,0,0,'b','l','a','h'};
	for (int i = 0; i < 12; i++){
		infoBlock.push_back(tempInfoBlock[i]);
	}

};  // End of wave::wave(){};



wave::wave(string fileName){
	ifstream fileIn;
	fileIn.open(fileName.c_str(), ios::binary);
	//get the standard header
	for(int i = 0; i < 36; ++i){
		char temp = fileIn.get();
		header.push_back(temp);
	}
	totalSize = *((unsigned int*)(&header[4]));
	compressionType = header[20] + header[21];
	channels = header[22] + header[23];
	sampleRate = *((unsigned int *)(&header[24]));
	bytesPerSec = *((unsigned int*)(&header[28]));
	byteDepth = header[32] + header[33];
	bitDepth = header[34] + header[35];
	//find datablock, ignore all else
	bool dataBlockFound = false;
	while(!dataBlockFound){
		dataHead.clear();
		for(int i = 0; i < 8; ++i){
			char temp = fileIn.get();
			dataHead.push_back(temp);
		}
		if(dataHead[0] == 'd' && dataHead[1] == 'a' && dataHead[2] == 't' && dataHead[3] == 'a')dataBlockFound = true;
	}
	dataLength = *((unsigned int*)(&dataHead[4]));
	for(unsigned int i = 0; i < dataLength; ++i){
		unsigned char temp = fileIn.get();
		buffer.push_back(temp);
	}
	//round up any other crap
	char temp;
	while((temp = fileIn.get()) != char_traits<char>::eof()){
		infoBlock.push_back(temp);
	}
	fileIn.close();
}

void wave::setDataLength(float length){
	dataLength = (int)(length * bytesPerSec);
	*((unsigned int*)(&header[4])) = dataLength + 36 + 12;
	*((unsigned int*)(&dataHead[4])) = dataLength;
};


void wave::writeW(string fileName){
	ofstream fileOut;
	fileOut.open(fileName.c_str(), ios::out | ios::binary);
	copy(header.begin(), header.end(), ostream_iterator<unsigned char>(fileOut));
	copy(dataHead.begin(), dataHead.end(), ostream_iterator<unsigned char>(fileOut));
	copy(buffer.begin(), buffer.end(), ostream_iterator<unsigned char>(fileOut));
	copy(infoBlock.begin(),infoBlock.end(), ostream_iterator<unsigned char>(fileOut));
	fileOut.close();
};


void wave::print(){
	cout<<"dataLength "<<dataLength<<endl;
	cout<<"buffer.size() "<<buffer.size()<<endl;

	cout<<"contents of header \n"<<left;
	for(unsigned int i = 0; i < header.size(); i++)    {
	     cout<<setw(7)<<header[i]<<setw(7)<<(int)header[i]<<endl;
	}
	//	copy(header.begin(), header.end(), ostream_iterator<unsigned char>(cout));

	cout<<endl<<"contents of dataHead \n";
	for(unsigned int i = 0; i < dataHead.size(); i++)    {
		cout<<setw(7)<<dataHead[i]<<setw(7)<<(int)dataHead[i]<<endl;
	}
	cout<<endl<<"contents of infoBlock \n";
	for (unsigned int i = 0; i < infoBlock.size(); i++)    {
		cout<<setw(7)<<infoBlock[i]<<setw(7)<<(int)infoBlock[i]<<endl;
	}
	/*
	cout<<"contents of markov"<<endl;
	map<long, vector<miniwave<short> > >::iterator it = markov.begin();
	for(it;it!= markov.end();it++){
		if(it->second.size() > 3){
			cout<<"it->first "<<it->first<<endl;
			cout<<"it->second.size() "<<it->second.size()<<endl;
		}
	}
	cout<<"total number of unique slices |"<<markov.size()<<"|";
	cout<<endl;
	*/
}


void wave::markovAte(){
     bool negative = false;
     int i = 0;
     short tempTogether = *((short*)(&buffer[i]));
     long prevMW = 0;
     int bufferSize = buffer.size();
     vector<short> temp;
     while(i < bufferSize){
	  miniwave<short>* mw = new miniwave<short>;
	  if(negative){
	       while((tempTogether <= 0)&&(i<bufferSize)){
		    mw->addSample(tempTogether);
		    i+=2;
		    tempTogether = *((short*)(&buffer[i]));
	       }
	       negative = false;
	  }
	  else{
	       while((tempTogether >= 0) && (i < bufferSize)){
		    mw->addSample(tempTogether);
		    i+=2;
		    tempTogether = *((short*)(&buffer[i]));
	       }
	       negative = true;
	  }
	  vertices[mw->identify()].addMW(*mw);
	  vertices[prevMW].addEdge(mw->identify());
	  prevMW = mw->identify();
	  delete mw;
     }
}//end of markovAte

// note to self
// change it so that the numbers are stored in teh buffer
// in their natural state, change them to chars
// when writing out.  Save a lot of this dickery.


void wave::scramble(){
	buffer.clear();

	for (unsigned int i = 0; i < dataLength/byteDepth; i++){
		// code to build new buffer from markov chain goes here.  
	}		
}// end of scramble

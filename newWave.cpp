#include "newWave.h"
#include <algorithm>
#include <map>
#include <cassert>
#include <ctime>
using namespace std;

typedef long long bobLong;

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
    //round up any other data
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

// .wav must have correct length to play.  
// This checks the size of everything that will be written out 
// and sets .wav length accordingly.
void wave::setDataLength(){
    if(dataLength != buffer.size()){
	cout<<"dataLength was wrong!"<<endl;
	cout<<"expected "<<dataLength<< " got "<<buffer.size()<<endl;
	dataLength = buffer.size(); 
	*((unsigned int*)(&header[4])) = dataLength + 24 + 8 + infoBlock.size();
	*((unsigned int*)(&dataHead[4])) = dataLength + infoBlock.size();
    }
};
	  
void wave::writeW(string fileName){
    if(dataLength != buffer.size()){
	setDataLength();
    }
    ofstream fileOut;
    fileOut.open(fileName.c_str(), ios::out | ios::binary);
    copy(header.begin(), header.end(), ostream_iterator<unsigned char>(fileOut));
    copy(dataHead.begin(), dataHead.end(), ostream_iterator<unsigned char>(fileOut));
    copy(buffer.begin(), buffer.end(), ostream_iterator<unsigned char>(fileOut));
    copy(infoBlock.begin(),infoBlock.end(), ostream_iterator<unsigned char>(fileOut));
    fileOut.close();
};

// for debugging.  
void wave::print(){
    cout<<"dataLength "<<dataLength<<endl;
    cout<<"buffer.size() "<<buffer.size()<<endl;

    cout<<"contents of header \n"<<left;
    for(unsigned int i = 0; i < header.size(); i++)    {
	cout<<setw(7)<<header[i]<<setw(7)<<(int)header[i]<<endl;
    }
    cout<<endl<<"contents of dataHead \n";
    for(unsigned int i = 0; i < dataHead.size(); i++)    {
	cout<<setw(7)<<dataHead[i]<<setw(7)<<(int)dataHead[i]<<endl;
    }
    cout<<endl<<"contents of infoBlock \n";
    for (unsigned int i = 0; i < infoBlock.size(); i++)    {
	cout<<setw(7)<<infoBlock[i]<<setw(7)<<(int)infoBlock[i]<<endl;
    }
}

// create a markov chain from .wav data block.
// memory intensive naive implementation.  
void wave::markovAte(){
    int i = 0;
    bobLong tempLong;
    int subSize = sizeof(tempLong);
    int bufferSize = buffer.size();
    short tempShort = 0;
    while(i < bufferSize - subSize){	 
	tempLong = *((bobLong*)(&buffer[i]));
	tempShort = *((short*)(&buffer[i+subSize]));
	markov[tempLong].push_back(tempShort);
	i+=2;
    }

    // little bit of housekeeping to link the end of the .wav data
    // with the beginning.  
    char zap[16];
    long j = 0;
    while(i < bufferSize){
	zap[j] = buffer[i];
	j++;
	i++;
    }
    i = 0;
    while(i < 8 ){
	zap[j] = buffer[i];
	j++;
	i++;
    }
    i = 0;
    int counterB = 0;
    while(i < 8 ){
	counterB++;
	tempLong = *((bobLong*)(&zap[i]));
	tempShort = *((short*)(&zap[i+subSize]));
	markov[tempLong].push_back(tempShort);
	i+=2;
    }
}//end of markovAte


// select the first link in the markov chain
// and build new .wav from it.  
void wave::scramble(){
    //uncomment to seed rand with the current time
    srand(time(NULL));
    buffer.clear();
    unsigned long bufferPlace = 0;
    handy tempHandy;
    map<long long, vector<short> >::iterator it = markov.begin();
    long long current = it->first;
    tempHandy.ID = current;
    for(int i = 0; i < 8; i++){
	buffer.push_back(tempHandy.Chars[i]);
    }
    int edgeSize;
    int randomEdge;
    short nextSample;
    unsigned char tempArr[2];
    unsigned long amountToMake = dataLength - sizeof(current);
    amountToMake = amountToMake * 6;
     
    while(bufferPlace < amountToMake){ 
	current = *((long long*)(&buffer[bufferPlace]));
	tempHandy.ID = current;
	edgeSize = markov[current].size();
	if(edgeSize < 1){
	    cout<<bufferPlace<<"  bufferplace"<<endl;
	    break;
	}
	randomEdge = rand() % edgeSize;
	nextSample = markov[current][randomEdge];
	*((short*)(&tempArr[0])) = nextSample;
	buffer.push_back(tempArr[0]);
	buffer.push_back(tempArr[1]);
	bufferPlace += 2;
    }
    setDataLength();
}// end of scramble


void wave::operator+=(const wave& right){
    bool chanEq = channels == right.channels;
    bool bytePS = bytesPerSec == right.bytesPerSec;

    if (chanEq && bytePS){
	unsigned long rightSize = right.buffer.size();
	for(unsigned long i = 0; i < rightSize; i++){
	    buffer.push_back(right.buffer[i]);
	}
    }
    else{cout<<"fuck"<<endl;}
    setDataLength();
}

wave wave::operator=(const wave& right){
    wave retWave;
    retWave.header = right.header;
    retWave.dataHead = right.dataHead;
    retWave.infoBlock = right.infoBlock;
    retWave.buffer = right.buffer;
    retWave.markov = right.markov;
    
    retWave.dataLength = right.dataLength;
    retWave.totalSize = right.totalSize;
    retWave.fmtSize = right.fmtSize;
    retWave.compressionType = right.compressionType;
    retWave.channels = right.channels;
    retWave.sampleRate = right.sampleRate;
    retWave.bytesPerSec = right.bytesPerSec;
    retWave.byteDepth = right.byteDepth;
    retWave.bitDepth = right.bitDepth;
    return retWave;
}
    

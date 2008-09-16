#include "newWave.h"
#include <algorithm>
#include <map>
#include <cassert>
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

void wave::setDataLength(){
     if(dataLength != buffer.size()){
	  cout<<"dataLength was wrong!"<<endl;
	  dataLength = buffer.size(); 
	  *((unsigned int*)(&header[4])) = dataLength + header.size() + infoBlock.size();
	  *((unsigned int*)(&dataHead[4])) = dataLength + header.size() + infoBlock.size();
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
     cout<<"size of vertices "<<vertices.size()<<endl;
     cout<<"vertices with no edges"<<endl;
     map<long int, vertex<short> >::iterator it;
     for(it = vertices.begin();it != vertices.end();it++){
	  //	  cout<<" ID "<<it->second.ID<<endl;
	  //	  cout<<"  edges   "<<it->second.edges.size()<<endl;
	  if(it->second.edges.size() == 0){
	       cout<<"Problem!  "<<it->first<<endl;
	       //		  it->second.addEdge(0);

	  }
     }
}


void wave::markovAte(){
     int i = 0;
     int bufferSize = buffer.size();
     marU marUnion;
     while(i < bufferSize - 4){	 
	  marUnion.marID = *(( long int*)(&buffer[i]));
	  vertices[marUnion.marID].setID(marUnion.marID);
	   short tempS = *(( short*)(&buffer[i+4]));
	  vertices[marUnion.marID].addEdge(tempS);
	  secretMap[marUnion.samples[1]].push_back(tempS);
	  i+=2;
     }
     //     marUnion.marID = *(( long int*)(&buffer[i]));
     //     vertices[marUnion.marID].setID(marUnion.marID);
     //     vertices[marUnion.marID].addEdge(0);

}//end of markovAte

// note to self
// change it so that the numbers are stored in teh buffer
// in their natural state, change them to chars
// when writing out.  Save a lot of this dickery.

void wave::scramble(){
     buffer.clear();
     unsigned int bufferPlace = 0;
	  marU twerp;
     map< long int, vertex<short> >::iterator it = vertices.begin();
      long int currentVertex = it->second.ID;
     assert(it->second.ID == it->first);
     	  twerp.marID = currentVertex;
     buffer.push_back(vertices[currentVertex].vertexUnion.marChars[0]);
     buffer.push_back(vertices[currentVertex].vertexUnion.marChars[1]);
     buffer.push_back(vertices[currentVertex].vertexUnion.marChars[2]);
     buffer.push_back(vertices[currentVertex].vertexUnion.marChars[3]);
     int edgeSize = vertices[currentVertex].edges.size();
     int randomEdge = rand() % edgeSize;
      short tempSample = vertices[currentVertex].edges[randomEdge].to;
     unsigned char tempArr[2];
     *((short*)(&tempArr[0])) = tempSample;
     buffer.push_back(tempArr[0]);
     buffer.push_back(tempArr[1]);
     bufferPlace += 2;
     //     short prevSampleOne = twerp.samples[0];
     short prevSampleTwo = twerp.samples[1];
     short prevTempSample = tempSample;
     while(bufferPlace < (dataLength - 4)){
	  currentVertex = *(( long int*)(&buffer[bufferPlace]));

	  twerp.marID = currentVertex;
	  //	  cout<<"----------------"<<endl;
	  //	  	  twerp.printMarU();
	  
	  if(!prevSampleTwo == twerp.samples[0]){
	       cout<<"big fucking problem "<<endl;
	       cout<<"prevSampleTwo   twerp.samples[0]"<<endl;
	       cout<<prevSampleTwo<<" != "<<twerp.samples[0]<<endl;
	       cout<<"bufferPlace "<<bufferPlace<<endl;
	       break;
	  }

	  if(!prevTempSample == twerp.samples[1]){
	       	       cout<<"big fucking problem "<<endl;
	       cout<<"prevTempSample   twerp.samples[1]"<<endl;
	       cout<<prevTempSample<<" != "<<twerp.samples[1]<<endl;
	       cout<<"bufferPlace "<<bufferPlace<<endl;
	       break;
	  }

	  it = vertices.find(currentVertex);
	  if(it == vertices.end()){
	       edgeSize = secretMap[twerp.samples[1]].size();
	       randomEdge = rand() % edgeSize;
	       tempSample = secretMap[twerp.samples[1]][randomEdge];
	  }
	  else{
	       edgeSize = vertices[currentVertex].edges.size();
	       randomEdge = rand() % edgeSize;
	       tempSample = vertices[currentVertex].edges[randomEdge].to;
	  }


	  //	  cout<<tempSample<<" TempSample"<<endl;
	  *((short*)(&tempArr[0])) = tempSample;
	  //	  cout<<(int)tempArr[0]<<" "<<(int)tempArr[1]<<endl;
	  buffer.push_back(tempArr[0]);
	  buffer.push_back(tempArr[1]);
	  bufferPlace += 2;
     prevSampleTwo = twerp.samples[1];
     prevTempSample = tempSample;
     }
     setDataLength();
}// end of scramble

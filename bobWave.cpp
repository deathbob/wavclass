#include "newWave.h"

using namespace std;

int main()
{
  //     cout<<sizeof(long long)<<endl;
  cout<<sizeof(unsigned int)<<endl;

     //     	wave mono1("guitMono.wav");
     wave mono1("amendeeptouch.wav");
	mono1.markovAte();
	mono1.scramble();
	mono1.writeW("amen1scramble9-27.wav");

}

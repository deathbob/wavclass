#include "newWave.h"

using namespace std;

int main()
{
     cout<<sizeof(long long)<<endl;

     //     	wave mono1("guitMono.wav");
          wave mono1("beasties.wav");
	mono1.markovAte();
	mono1.scramble();
	mono1.writeW("beastiesScramble2.wav");

}

#include "newWave.h"

using namespace std;

int main()
{
  //     cout<<sizeof(long long)<<endl;
  cout<<sizeof(unsigned int)<<endl;

     //     	wave mono1("guitMono.wav");
  //     wave mono1("amenbreaking.wav");
       wave mono1("amendeeptouch.wav");
  //  wave mono1("Abstract05.wav");
  //  wave mono1("chemical_135.wav");
  //  wave mono1("electroclash_klantz_133.wav"); 
       //  wave mono1("genesis.wav");
	mono1.markovAte();
	mono1.scramble();
	mono1.writeW("amenDP-1scramble9-27.wav");

}

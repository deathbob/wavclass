#include "newWave.h"

using namespace std;

int main()
{
     	wave mono1("guitMono.wav");
	//     wave mono2("snicker.wav");
	mono1.markovAte();
	mono1.print();
	mono1.scramble();
	mono1.print();
	mono1.writeW("drumLoopScramble9_15.wav");
}

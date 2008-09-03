#include "newWave.h"

using namespace std;

int main()
{
	wave mono1("guitMono.wav");
	//	mono1.writeW("Mono1.wav");
	mono1.markovAte();
	mono1.scramble();
	mono1.print();
}

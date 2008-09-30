#include "newWave.h"

using namespace std;

int main()
{
    //    wave mono1("guitMono.wav");
    //        wave stereoB("amenbreaking.wav");
    //        wave monoB("amenbreaking_mono.wav");
        wave mono1("amenupdown.wav");
	//        wave stereoDT("amendeeptouch.wav");
	//        wave stereoAB05("Abstract05.wav");
	//        wave stereoChem("chemical_135.wav");
	//        wave stereoDance("dance.wav");
	//    wave stereoElectro("electroclash_klantz_133_1.wav"); 
    //    wave stereoNord("nordic_1.wav");
    //    wave stereoMid("midnight_1.wav");
	//	mono1 += monoB;
    mono1.markovAte();
    mono1.scramble();
    mono1.writeW("UDlong_scramble9-29.wav");
}

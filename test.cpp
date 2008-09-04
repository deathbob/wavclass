#include <map>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "miniwave.h"
using namespace std;

int main(){
     map<long, miniwave<short> > bob;
     miniwave<short>* problem = new miniwave<short>;
     problem->addSample(100);
     bob[problem->identify()] = *problem;
     delete problem;
}

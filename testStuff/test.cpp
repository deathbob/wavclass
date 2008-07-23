#include <vector>
#include <iostream>

using namespace std;

int main()
{
  /* 
 char temp[] = {0,128,0,0};
  vector<char> notTemp;

  for(int i = 0; i < 4; ++i)
    {
      notTemp.push_back(temp[i]);
    }

  unsigned int desiredValue = *(unsigned int*)&notTemp[0];
  cout<<desiredValue<<endl;
  */

  int bob= 128;
  char ted[4];
  cout<<sizeof(int)<<" int"<<endl;
  cout<<sizeof(char)<<" char"<<endl;
  for(int i = 0; i < 4; ++i)
    {
      ted[i] = bob<<i * 8;
      cout<<(int)ted[i]<<endl;
      for(int j = 0; j < 8; ++j)
	{
	  int bit = ((ted[i] >> j) & 1);
	  cout<<bit;
	}
      cout<<endl;
    }


}

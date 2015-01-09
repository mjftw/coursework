#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    float k = 1;

    for(int i=0; i<16; i++)
    {
        k *= cos(atan(pow(2,-i)));
    }

    cout << "k = " << k << endl;
}

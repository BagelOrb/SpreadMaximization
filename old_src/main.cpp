
#include <stdio.h>
#include <iostream> // std::cerr

#include "Mat3Df.h"
#include "debug.h"

int main ( int argc, char** argv )
{
    typedef Mat3Df::Pos Pos;
    int w = 5;
    int h = 4;
    int d = 3;
    Mat3Df m(w,h,d);
    float i = 0;
    for (int z = 0; z < d; z++)
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
        m.set(x,y,z, i);
        i++;
    }

    Mat3Df k(2,2,2);
    for (int z = 0; z < k.d; z++)
    for (int y = 0; y < k.h; y++)
    for (int x = 0; x < k.w; x++)
    {
        k.set(x,y,z, 0);
        if (x == y && y == 1 && z == 0)
            k.set(x,y,z, 1);
    }

    Mat3Df r = m.convolute(k);
    r = r.apply([](float in) {return in -1; } );
    
    for (Mat3Df::iterator it = r.begin(); it != r.end(); ++it)
        std::cerr << ((it.x == 0)? "\n" : std::string("\t")) << *it;
    
    
    return 0;   
}
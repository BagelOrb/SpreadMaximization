
//test convolution
#include <stdio.h>
#include <iostream> // std::cerr

#include "Mat3Df.h"
#include "Convolution.h"
#include "Pooling.h"
#include "Pos.h"


int main ( int argc, char** argv )
{
    int w = 5;
    int h = 4;
    int d = 3;
    Mat3Df m(Dims(w,h,d));
    float i = 0;
    for (int z = 0; z < d; z++)
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
        m.set(x,y,z, i);
        i++;
    }

    Convolution conv(Dims(2, 2, 2));
    Mat3Df& k = conv.kernel;
    for (int z = 0; z < k.d; z++)
    for (int y = 0; y < k.h; y++)
    for (int x = 0; x < k.w; x++)
    {
        k.set(x,y,z, 0);
        if (x == 0 && y == 0 && z == 0)
            k.set(x,y,z, 1);
    }

    auto func = [](float in) 
    { 
//         std::cerr << "sdg " <<std::endl; 
        return in * -1; 
    };
    
    Mat3Df r = conv.convolute(m).apply(func );
//     r = r.apply([](float in) {return in -1; } );
    
    for (int z = 0; z < r.d; z++)
    {
        for (int y = 0; y < r.h; y++)
        {
            for (int x = 0; x < r.w; x++)
            {
                std::cerr << r.get(x,y,z) << "\t"; 
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
    return 0;   
}
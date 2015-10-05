#ifndef NEURON_H
#define NEURON_H


#include "opencv2/imgproc/imgproc.hpp"

#include "basics.h"

using namespace cv;

class Neuron 
{
    std::vector<Mat_<val>> weights;
    val bias;

    int size;
    
    Neuron(int size) 
    : size(size)
    , weights(size, size)
    {   
    }

};

#endif // NEURON_H
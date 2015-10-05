#ifndef NEURON_H
#define NEURON_H


#include "opencv2/imgproc/imgproc.hpp"

#include "basics.h"

using namespace cv;

class Neuron 
{
public:
    std::vector<Mat_<val>> weights;
    val bias;

    int size;
    
    Neuron(int size, int n_input_channels) 
    : size(size)
    {   
        while (weights.size() < n_input_channels)
        {
            weights.emplace_back(size, size);
        }
    }

};

#endif // NEURON_H
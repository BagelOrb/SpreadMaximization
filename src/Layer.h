#ifndef LAYER_H
#define LAYER_H

#include "opencv2/imgproc/imgproc.hpp"
#include "Neuron.h"

using namespace cv;

class Layer 
{
public:
    std::vector<Neuron> neurons;
    
    
    Layer(int depth, int input_patch_size, int n_input_channels)
    {
        while (neurons.size() < depth) 
        {
            neurons.emplace_back(input_patch_size, n_input_channels);
        }
    }
    
    


};

#endif // LAYER_H
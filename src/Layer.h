#ifndef LAYER_H
#define LAYER_H

#include "Neuron.h"

using namespace cv;

class Layer 
{
    std::vector<Neuron> neurons;
    
    
    Layer(int depth, int input_patch_size)
    {
        while (neurons.size() < depth) 
        {
            neurons.emplace_back(input_patch_size);
        }
    }


};

#endif // LAYER_H
#ifndef LAYER_LAYER_H
#define LAYER_LAYER_H

#include "SubLayer.h"
// #include "SignalLayer.h"
// #include "PoolingFunction.h"
// #include "PoolingLayer.h"

class ComboLayer : public SubLayer
{
    std::vector<SubLayer> layers;
    
    void initializeParams(std::function<float(float)> func)
    {
        for (SubLayer& layer : layers)
        {
            layer.initializeParams(func);
        }
    }
    
    Dims3 getOutputDims(Dims3 input_dims)
    {
        for (SubLayer& layer : layers)
        {
            input_dims = layer.getOutputDims(input_dims); // output of prev layer is input to next
        }
        return input_dims;
    }
    
    void forward(Mat3Df& in, Mat3Df& out)
    {
        Mat3Df* _in = &in;
        Mat3Df* _out = &out;
        for (SubLayer& layer : layers)
        {
            layer.forward();
        }
        
    }
    
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives = nullptr) = 0;
    
};

#endif // LAYER_LAYER_H
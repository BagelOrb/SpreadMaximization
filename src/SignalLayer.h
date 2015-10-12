#ifndef SIGNAL_LAYER_H
#define SIGNAL_LAYER_H

#include <vector>
#include <functional>

#include "SubLayer.h"
#include "Mat3Df.h"
#include "Convolution.h"
#include "Pos.h"

class SignalLayer : public SubLayer 
{
    struct Neuron 
    {
        Mat3Df weights;
        Convolution conv;
        float bias;
        
        Neuron(Dims conv_field)
        : weights(conv_field)
        , conv(weights)
        {
        }
        
        void initializeParams(std::function<float(float)> func)
        {
            weights.apply(func);
            bias = func(bias);
        }
    };
    
    std::vector<Neuron> neurons;
    
public:
    SignalLayer(Dims conv_field, unsigned int n_neurons)
    {
        while (neurons.size() < n_neurons)
        {
            neurons.emplace_back(conv_field);
        }
    }
    
    void forward(Mat3Df& in, Mat3Df& out)
    {
        for (unsigned int z = 0; z < out.d; z++)
        {
            neurons[z].conv.convolute(in, out, z);
        }
    }
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df& in_derivatives) = 0;


    void initializeParams(std::function<float(float)> func)
    {
        for (Neuron& neuron : neurons)
        {
            neuron.initializeParams(func);
        }
    }




};

#endif // SIGNAL_LAYER_H
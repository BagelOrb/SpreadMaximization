#ifndef SIGNAL_LAYER_H
#define SIGNAL_LAYER_H

#include <vector>
#include <functional>

#include <iostream> // debug

#include "SubLayer.h"
#include "../utils/Mat3Df.h"
#include "../utils/Mat4D.h"
#include "../utils/Convolution.h"
#include "../utils/Pos.h"
#include "../utils/Dims.h"

#include "../utils/Mat3D.h"

#include "../learning/GradientDescentFunction.h"

#include "Neuron.h"

class SignalLayer : public SubLayer 
{
public:
    
    Mat4Df* weights;
    Mat4Df* weights_ders;
    Mat4Df* biases; // 1*1*1*m mat
    Mat4Df* biases_ders; // 1*1*1*m mat
    
    std::vector<Neuron> neurons;

    SignalLayer(Dims3 conv_field, unsigned int n_neurons)
    {
        layer_params.reserve(2); // weights and derivatives
        layer_params.emplace_back(Dims4(conv_field.w, conv_field.h, conv_field.d, n_neurons));
        layer_params.emplace_back(Dims4(n_neurons, 1, 1, 1));
        weights = &layer_params[0].params;
        weights_ders = &layer_params[0].params;
        biases = &layer_params[1].params;
        biases_ders = &layer_params[1].params;
    }
    
    Dims3 getOutputDims(Dims3 input_dims);
    
    void initializeParams(std::function<float(float)> func);
    
    void forward(Mat3Df& in, Mat3Df& out);
    
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives);

};


#endif // SIGNAL_LAYER_H
#ifndef SIGNAL_LAYER_H
#define SIGNAL_LAYER_H

#include <vector>
#include <functional>

#include <iostream> // debug

#include "SubLayer.h"
#include "../utils/Mat3Df.h"
#include "../utils/Convolution.h"
#include "../utils/Pos.h"
#include "../utils/Dims.h"

#include "../utils/Mat3D.h"

#include "../learning/GradientDescentFunction.h"

#include "Neuron.h"

template<class UpdateFunctionState>
class SignalLayer : public SubLayer 
{
public:
    
    std::vector<Neuron> neurons;
    
    UpdateFunctionParams* update_function_params; //!< The global parameters to the update function, e.g. weight decay term
    
    struct NeuronUpdateState
    {
        Mat3D<UpdateFunctionState> weight_params;
        UpdateFunctionState bias_params;
        
        NeuronUpdateState(Dims conv_field)
        : weight_params(conv_field)
        { }
        
        void resetDers()
        {
            bias_params.resetDer();
            for (auto weight_param_it = weight_params.begin(); weight_param_it != weight_params.end(); ++weight_param_it)
            {
                weight_param_it->resetDer();
            }
        }
    };
    
    std::vector<NeuronUpdateState> update_function_state; //!< The current state of local parameters for the update function for a single layer parameter
    
    SignalLayer(Dims conv_field, unsigned int n_neurons)
    : update_function_params(nullptr)
    {
        neurons.reserve(n_neurons);
        update_function_state.reserve(n_neurons);
        while (neurons.size() < n_neurons)
        {
            neurons.emplace_back(conv_field);
            update_function_state.emplace_back(conv_field);
        }
    }
    
    Dims getOutputDims(Dims input_dims);
    
    void initializeParams(std::function<float(float)> func);
    
    void forward(Mat3Df& in, Mat3Df& out);
    
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives);

};

#include "SignalLayerImpl.h"

#endif // SIGNAL_LAYER_H
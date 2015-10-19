#ifndef SIGNAL_LAYER_H
#define SIGNAL_LAYER_H

#include <vector>
#include <functional>

#include <iostream> // debug

#include "SubLayer.h"
#include "Mat3Df.h"
#include "Convolution.h"
#include "Pos.h"

#include "Mat3D.h"

#include "UpdateFunction.h"

struct Neuron 
{
    struct State 
    {
        Mat3Df weights;
        float bias;
        
        State(Dims conv_field)
        : weights(conv_field)
        {
        }
        void initializeParams(std::function<float(float)> func)
        {
            weights.apply(func);
            bias = func(bias);
        }
    };
    State state;
    Convolution conv;
    
    Neuron(Dims conv_field)
    : state(conv_field)
    , conv(state.weights)
    {
    }
    
//     Neuron(const Neuron& b)
//     : state(b.state)
//     , conv(b.conv)
//     {
//         std::cerr << "Neruon copy\n";
//     }
    
    void initializeParams(std::function<float(float)> func)
    {
        state.initializeParams(func);
    }
};

template<class UpdateFunctionState>
class SignalLayer : public SubLayer 
{
public:
    
    std::vector<Neuron> neurons;
//     std::vector<Neuron::State> derivatives;
    
    UpdateFunctionParams* update_function_params; //!< The global parameters to the update function, e.g. weight decay term
    
//     template<class UpdateFunctionParams_>
    struct NeuronUpdateState
    {
        Mat3D<UpdateFunctionState> weight_params;
        UpdateFunctionState bias_params;
        
        NeuronUpdateState(Dims conv_field)
        : weight_params(conv_field)
        { }
    };
//     template<class T> NeuronUpdateState<T>::NeuronUpdateState(Dims conv_field)
//     : weight_params(conv_field)
//     { }
    
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
    
    Dims getOutputDims(Dims input_dims)
    {
        assert(neurons.size() > 0);
        Dims2 conv_out_dims = neurons[0].conv.getOutputDims(input_dims);
        return Dims(conv_out_dims.w, conv_out_dims.h, neurons.size());
    }
    
    void initializeParams(std::function<float(float)> func)
    {
        for (Neuron& neuron : neurons)
        {
            neuron.initializeParams(func);
        }
    }
    
    void forward(Mat3Df& in, Mat3Df& out)
    {
        assert(out.d == neurons.size());
        for (unsigned int z = 0; z < out.d; z++)
        {
            neurons[z].conv.convolute(in, out, z);
        }
    }
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives)
    {
        assert(out.getDims() == out_derivatives.getDims());
        assert(!in_derivatives || in.getDims() == in_derivatives->getDims());
        if (in_derivatives) 
        {
            in_derivatives->apply([](float) { return 0; }); // reset derivatives at input mat to zero
        }
        
        for (unsigned int neuron_idx = 0; neuron_idx < neurons.size(); neuron_idx++)
        {
            Neuron& neuron = neurons[neuron_idx];
            for (Mat3Df::iterator out_it = out_derivatives.begin(); out_it != out_derivatives.end(); ++out_it)
            {
//                 update_function_state->update(neuron.state.bias, *out_it, update_function_params);
                float neuron_out_der = *out_it;
                update_function_state[neuron_idx].bias_params.update(neuron.state.bias, neuron_out_der, update_function_params);
                for (Mat3Df::iterator weight_it = neuron.state.weights.begin(); weight_it != neuron.state.weights.end(); ++weight_it)
                {
                    Pos in_pos = out_it.getPos() + weight_it.getPos();
                    float weight_der = neuron_out_der * in.get(in_pos);
                    update_function_state[neuron_idx].bias_params.update(neuron.state.bias, weight_der, update_function_params);
                    if (in_derivatives)
                    {
                        in_derivatives->add(in_pos, neuron_out_der * *weight_it);
                    }
                }
            }
        }
    }






};

#endif // SIGNAL_LAYER_H
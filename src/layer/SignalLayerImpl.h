#ifndef LAYER_SIGNAL_LAYER_IMPL_H
#define LAYER_SIGNAL_LAYER_IMPL_H

#include "SignalLayer.h"

template<class UpdateFunctionState>
Dims SignalLayer<UpdateFunctionState>::getOutputDims(Dims input_dims)
{
    assert(neurons.size() > 0);
    Dims2 conv_out_dims = neurons[0].conv.getOutputDims(input_dims);
    return Dims(conv_out_dims.w, conv_out_dims.h, neurons.size());
}

template<class UpdateFunctionState>
void SignalLayer<UpdateFunctionState>::initializeParams(std::function<float(float)> func)
{
    for (Neuron& neuron : neurons)
    {
        neuron.initializeParams(func);
    }
}

template<class UpdateFunctionState>
void SignalLayer<UpdateFunctionState>::forward(Mat3Df& in, Mat3Df& out)
{
    assert(out.d == neurons.size());
    for (unsigned int z = 0; z < out.d; z++)
    {
        neurons[z].conv.convolute(in, out, z);
    }
}

template<class UpdateFunctionState>
void SignalLayer<UpdateFunctionState>::backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives)
{
    assert(out.getDims() == out_derivatives.getDims());
    assert(!in_derivatives || in.getDims() == in_derivatives->getDims());
    if (in_derivatives) 
    {
        in_derivatives->apply([](float) { return 0; }); // reset derivatives at input mat to zero
    }
    
    for (NeuronUpdateState& param_ders : update_function_state)
    {
        param_ders.resetDers();
    }
    
    for (unsigned int neuron_idx = 0; neuron_idx < neurons.size(); neuron_idx++)
    {
        Neuron& neuron = neurons[neuron_idx];
        for (Mat3Df::iterator out_it = out_derivatives.begin(); out_it != out_derivatives.end(); ++out_it)
        {
//                 update_function_state->update(neuron.state.bias, *out_it, update_function_params);
            float neuron_out_der = *out_it;
            update_function_state[neuron_idx].bias_params.registerDer(neuron_out_der);
            for (Mat3Df::iterator weight_it = neuron.state.weights.begin(); weight_it != neuron.state.weights.end(); ++weight_it)
            {
                Pos in_pos = out_it.getPos() + weight_it.getPos();
                float weight_der = neuron_out_der * in.get(in_pos);
                update_function_state[neuron_idx].weight_params.get(weight_it.getPos()).registerDer(weight_der);
                if (in_derivatives)
                {
                    in_derivatives->add(in_pos, neuron_out_der * *weight_it);
                }
            }
        }
    }
    for (unsigned int neuron_idx = 0; neuron_idx < neurons.size(); neuron_idx++)
    {
        Neuron& neuron = neurons[neuron_idx];
        update_function_state[neuron_idx].bias_params.update(neuron.state.bias, update_function_params);
        
        for (Mat3Df::iterator weight_it = neuron.state.weights.begin(); weight_it != neuron.state.weights.end(); ++weight_it)
        {
            update_function_state[neuron_idx].weight_params.get(weight_it.getPos()).update(*weight_it, update_function_params);
        }
        
    }
}


#endif // LAYER_SIGNAL_LAYER_IMPL_H
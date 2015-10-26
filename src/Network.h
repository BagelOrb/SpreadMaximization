#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <functional>

#include "layer/SubLayer.h"
#include "utils/Mat3Df.h"
#include "utils/Mat3D.h"

class LayerState
{
public:
    SubLayer& layer;
    Mat3Df& input;
    Mat3Df output;
    Mat3Df output_ders;
    Mat3Df* input_ders;
    LayerState(SubLayer& layer, Mat3Df& input)
    : layer(layer)
    , input(input)
    , output(layer.getOutputDims(input.getDims()))
    , output_ders(output.getDims())
    , input_ders(nullptr)
    {
    }
};

class NetworkState 
{
public:
    std::vector<LayerState> layer_states;
    
    void initialize(std::vector<SubLayer>& layers, Mat3Df& input)
    {
        if (!layer_states.empty())
        {
            if (layer_states.front().input.getDims() == input.getDims())
            {
                return;
            }
            else 
            {
                layer_states.clear();
            }
        }
        
        layer_states.reserve(layers.size());
        Mat3Df* _input = &input;
        LayerState* last_layer_state = nullptr;
        
        for (unsigned int layer_idx = 0; layer_idx < layers.size(); layer_idx++)
        {
            layer_states.emplace_back(layers[layer_idx], *_input);
            LayerState& layer_state = layer_states.back();
            if (last_layer_state)
            {
                layer_state.input_ders = &last_layer_state->output_ders;
            }
            _input = &layer_state.output;
            last_layer_state = &layer_state;
        }
        
    }
};

class Network 
{
    std::vector<SubLayer*> layers;
    
    NetworkState network_state;
    
    void initializeParams(std::function<float(float)> func)
    {
        for (SubLayer* layer : layers)
        {
            layer->initializeParams(func);
        }
    }
    
    Dims3 getOutputDims(Dims3 input_dims)
    {
        for (SubLayer* layer : layers)
        {
            input_dims = layer->getOutputDims(input_dims); // output of prev layer is input to next
        }
        return input_dims;
    }
    
    void forward()
    {
        for (unsigned int layer_idx = 0; layer_idx < layers.size(); layer_idx++)
        {
            SubLayer& layer = *layers[layer_idx];
            LayerState& state = network_state.layer_states[layer_idx];
            layer.forward(state.input, state.output);
        }
    }
    
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives = nullptr)
    {
        network_state.layer_states[0].input_ders = in_derivatives;
        
        for (unsigned int layer_idx = layers.size() - 1; int(layer_idx) >= 0; layer_idx++)
        {
            SubLayer& layer = *layers[layer_idx];
            LayerState& state = network_state.layer_states[layer_idx];
            layer.backward(state.input, state.output, state.output_ders, state.input_ders);
        }
        
    }
    


};

#endif // NETWORK_H
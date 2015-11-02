#include "Network.h"
#include "layer/SignalLayer.h"
#include "layer/TransferLayer.h"
#include "layer/PoolingLayer.h"
#include "layer/PoolingFunction.h"
#include "utils/Dims.h"

void NetworkState::initialize(std::vector<SubLayer*>& layers, Mat3Df& input)
{
    if (!layer_states.empty())
    {
        if (layer_states.front().input.getDims() == input.getDims())
        { // the last time the network has signalled was on the same size image, so all Mat3D's are already of the correct size
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
        layer_states.emplace_back(*layers[layer_idx], *_input);
        LayerState& layer_state = layer_states.back();
        if (last_layer_state)
        {
            layer_state.input_ders = &last_layer_state->output_ders;
        }
        _input = &layer_state.output;
        last_layer_state = &layer_state;
    }
    
}

Network::~Network()
{
    for (SubLayer* layer : layers)
        delete layer;
}


void Network::addLayer(LayerSettings settings, unsigned int n_input_features)
{
    assert(layers.size() > 0 || n_input_features > 0);
    if (n_input_features == 0)
    {
        n_input_features = getOutputDepth(n_input_features);
    }
    layers.push_back(new SignalLayer(Dims3(settings.conv_field_size.w, settings.conv_field_size.h, n_input_features), settings.n_neurons));
    layers.push_back(new TransferLayer(*settings.transfer_function));
    switch (settings.pool_type)
    {
    case PoolType::Max:
        layers.push_back(new PoolingLayer<MaxPoolingFunction>(settings.pool_field_size, settings.pool_skip));
        break;
    case PoolType::AbsMax:
        layers.push_back(new PoolingLayer<AbsMaxPoolingFunction>(settings.pool_field_size, settings.pool_skip));
        break;
    case PoolType::SoftAbsMax:
        layers.push_back(new PoolingLayer<SoftAbsMaxPoolingFunction>(settings.pool_field_size, settings.pool_skip));
        break;
    case PoolType::SoftSquareMax:
        layers.push_back(new PoolingLayer<SoftSquareMaxPoolingFunction>(settings.pool_field_size, settings.pool_skip));
        break;
    }
}


void Network::initializeParams(std::function<float (float)> func)
{
    for (SubLayer* layer : layers)
    {
        layer->initializeParams(func);
    }
}

Dims3 Network::getOutputDims(Dims3 input_dims)
{
    for (SubLayer* layer : layers)
    {
        input_dims = layer->getOutputDims(input_dims); // output of prev layer is input to next
    }
    return input_dims;
}

void Network::forward()
{
    for (unsigned int layer_idx = 0; layer_idx < layers.size(); layer_idx++)
    {
        SubLayer& layer = *layers[layer_idx];
        LayerState& state = network_state.layer_states[layer_idx];
        layer.forward(state.input, state.output);
    }
}

void Network::backward(Mat3Df* in_derivatives)
{
    network_state.layer_states[0].input_ders = in_derivatives;
    
    for (unsigned int layer_idx = layers.size() - 1; int(layer_idx) >= 0; layer_idx++)
    {
        SubLayer& layer = *layers[layer_idx];
        LayerState& state = network_state.layer_states[layer_idx];
        layer.backward(state.input, state.output, state.output_ders, state.input_ders);
    }
    
}





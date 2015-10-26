#ifndef LAYER_LAYER_H
#define LAYER_LAYER_H

#include "SubLayer.h"
#include "SignalLayer.h"
#include "PoolingFunction.h"
#include "PoolingLayer.h"
#include "TransferLayer.h"
#include "TransferFunction.h"

#include "../utils/Dims.h"
#include "../Settings.h"



class Layer 
{
    SubLayer* pooling_layer;
    SubLayer* transfer_layer;
    SubLayer* signal_layer;
    
    TransferFunction* transfer_function;
    
public:
    Layer(LayerSettings settings, unsigned int n_input_channels)
    {
        switch (settings.pool_type)
        {
            case PoolType::Max:
                pooling_layer = new PoolingLayer<MaxPoolingFunction>(settings.pool_field_size, settings.pool_skip);
                break;
            case PoolType::AbsMax:
                pooling_layer = new PoolingLayer<AbsMaxPoolingFunction>(settings.pool_field_size, settings.pool_skip);
                break;
            case PoolType::SoftAbsMax:
                pooling_layer = new PoolingLayer<SoftAbsMaxPoolingFunction2>(settings.pool_field_size, settings.pool_skip);
                break;
            case PoolType::SoftSquareMax:
            default:
                pooling_layer = new PoolingLayer<SoftSquareMaxPoolingFunction2>(settings.pool_field_size, settings.pool_skip);
                break;
        }
        
        
        switch (settings.transfer_function_type)
        {
            case TransferFunctionType::Sigmoid:
                transfer_function = new SigmoidTransferFunction();
                break;
            case TransferFunctionType::Tanh:
            default:
                transfer_function = new TanhTransferFunction();
        }
        transfer_layer = new TransferLayer(*transfer_function);
        
        
        signal_layer = new SignalLayer(Dims3(settings.conv_field_size.w, settings.conv_field_size.h, n_input_channels), settings.n_neurons);
    }
    
    void addToNetwork(std::vector<SubLayer*>& layers)
    {
        layers.push_back(signal_layer);
        layers.push_back(transfer_layer);
        layers.push_back(pooling_layer);
    }
};

#endif // LAYER_LAYER_H
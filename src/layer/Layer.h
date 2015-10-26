#ifndef LAYER_LAYER_H
#define LAYER_LAYER_H

#include "SubLayer.h"
#include "SignalLayer.h"
#include "PoolingFunction.h"
#include "PoolingLayer.h"
#include "TransferLayer.h"
#include "TransferFunction.h"

#include "../utils/Dims.h"

enum class PoolType
{
    Max, AbsMax, SoftAbsMax, SoftSquareMax
};

enum class TransferFunctionType
{
    Sigmoid, Tanh
};

class LayerSettings
{
public:
    // pool settings
    PoolType pool_type;
    Dims2 field_size; 
    Dims2 skip;
    
    // transfer settings
    TransferFunctionType transfer_function_type;
    
    // convolution settings
    Dims2 conv_field; 
    unsigned int n_neurons;
};

class Layer 
{
    SubLayer* pooling_layer;
    SubLayer* transfer_layer;
    SubLayer* signal_layer;
    
    TransferFunction* transfer_function;
    
    
    Layer(LayerSettings settings, unsigned int n_input_channels)
    {
        switch (settings.pool_type)
        {
            case PoolType::Max:
                pooling_layer = new PoolingLayer<MaxPoolingFunction>(settings.field_size, settings.skip);
                break;
            case PoolType::AbsMax:
                pooling_layer = new PoolingLayer<AbsMaxPoolingFunction>(settings.field_size, settings.skip);
                break;
            case PoolType::SoftAbsMax:
                pooling_layer = new PoolingLayer<SoftAbsMaxPoolingFunction2>(settings.field_size, settings.skip);
                break;
            case PoolType::SoftSquareMax:
            default:
                pooling_layer = new PoolingLayer<SoftSquareMaxPoolingFunction2>(settings.field_size, settings.skip);
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
        
        
        signal_layer = new SignalLayer(Dims3(settings.conv_field.w, settings.conv_field.h, n_input_channels), settings.n_neurons);
    }
    
};

#endif // LAYER_LAYER_H
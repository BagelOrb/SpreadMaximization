#ifndef SETTINGS_H
#define SETTINGS_H

#include "layer/TransferFunction.h"

enum class PoolType
{
    Max, AbsMax, SoftAbsMax, SoftSquareMax
};

enum class TransferFunctionType
{
    Sigmoid, Tanh
};



class NetworkSettings
{
public:
    PoolType pool_type;
    TransferFunctionType transfer_function_type;
};

class LayerSettings 
{
public:
    NetworkSettings network_settings;
    
    // pool settings
    Dims2 pool_field_size; 
    Dims2 pool_skip;
    
    // transfer settings
    TransferFunction* transfer_function;
    
    // convolution settings
    Dims2 conv_field_size; 
    unsigned int n_neurons;
    
    LayerSettings(NetworkSettings network_settings, unsigned int n_neurons, unsigned int w_c, unsigned int w_p, TransferFunction* transfer_function)
    : network_settings(network_settings)
    , pool_field_size(w_p, w_p)
    , pool_skip(w_p, w_p)
    , transfer_function(transfer_function)
    , conv_field_size(w_c, w_c)
    , n_neurons(n_neurons)
    {
    }
    
};



#endif // SETTINGS_H
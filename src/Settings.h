#ifndef SETTINGS_H
#define SETTINGS_H

#include "layer/TransferFunction.h"
#include "utils/Dims.h"

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
    PoolType pool_type;
    TransferFunctionType transfer_function_type;
    
    // pool settings
    Dims2 pool_field_size; 
    Dims2 pool_skip;
    
    // transfer settings
    TransferFunction* transfer_function;
    
    // convolution settings
    Dims2 conv_field_size; 
    unsigned int n_neurons;
    
    LayerSettings(PoolType pool_type, TransferFunctionType transfer_function_type, unsigned int n_neurons, unsigned int w_c, unsigned int w_p, TransferFunction* transfer_function)
    : pool_type(pool_type)
    , transfer_function_type(transfer_function_type)
    , pool_field_size(w_p, w_p)
    , pool_skip(w_p, w_p)
    , transfer_function(transfer_function)
    , conv_field_size(w_c, w_c)
    , n_neurons(n_neurons)
    {
    }
    
};



#endif // SETTINGS_H
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
    Sigmoid, Tanh, Linear
};


class LayerSettings 
{
public:
    PoolType pool_type;
    TransferFunctionType transfer_function_type;
    
    // pool settings
    Dims2 pool_field_size; 
    Dims2 pool_skip;
    
    // convolution settings
    Dims2 conv_field_size; 
    unsigned int n_neurons;
    
    LayerSettings(PoolType pool_type, TransferFunctionType transfer_function_type, unsigned int n_neurons, unsigned int w_c, unsigned int w_p)
    : pool_type(pool_type)
    , transfer_function_type(transfer_function_type)
    , pool_field_size(w_p, w_p)
    , pool_skip(w_p, w_p)
    , conv_field_size(w_c, w_c)
    , n_neurons(n_neurons)
    {
    }
    
};

enum class ObjectiveFunctionType
{
    Test
};

enum class UpdaterType 
{
    GradientDescent 
};

class ProcessorSettings 
{ 
public:
    ObjectiveFunctionType objective_function_type;
    UpdaterType updater_type;
    
    double gradient_descent_delta;
    
    ProcessorSettings(double gradient_descent_delta)
    : objective_function_type(ObjectiveFunctionType::Test)
    , updater_type(UpdaterType::GradientDescent)
    , gradient_descent_delta(gradient_descent_delta)
    {
    }
};

#endif // SETTINGS_H
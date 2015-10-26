#ifndef SETTINGS_H
#define SETTINGS_H


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

class LayerSettings : public NetworkSettings
{
public:
    // pool settings
    Dims2 pool_field_size; 
    Dims2 pool_skip;
    
    // transfer settings
    
    // convolution settings
    Dims2 conv_field_size; 
    unsigned int n_neurons;
    
    LayerSettings(unsigned int n_neurons, unsigned int w_c, unsigned int w_p, NetworkSettings network_settings)
    : NetworkSettings(network_settings)
    , pool_field_size(w_p, w_p)
    , pool_skip(w_p, w_p)
    , conv_field_size(w_c, w_c)
    , n_neurons(n_neurons)
    {
    }
    
};



#endif // SETTINGS_H
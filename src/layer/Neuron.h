#ifndef LAYER_NEURON_H
#define LAYER_NEURON_H




struct Neuron 
{
    struct State 
    {
        Mat3Df weights;
        float bias;
        
        State(Dims3 conv_field)
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
    
    Neuron(Dims3 conv_field)
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

#endif // LAYER_NEURON_H
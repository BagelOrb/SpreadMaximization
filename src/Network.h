#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <functional>

#include "layer/SubLayer.h"
#include "utils/Mat3Df.h"
#include "utils/Mat3D.h"
#include "Settings.h"

class LayerState
{
public:
    SubLayer& layer;
    Mat3Df& input; // the input is the output of the previous layer
    Mat3Df output;
    Mat3Df output_ders;
    Mat3Df* input_ders; // may be nullptr when input derivatives aren't needed (on the very first layer)
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
    
    /*!
     * Initialize all mats and link them over layers.
     * Used for reinitialization when the next image is of a different size.
     */
    void initialize(std::vector<SubLayer*>& layers, Mat3Df& input);
    
    /*!
     * Similar to initialize: copy all mats and link them over layers.
     */
    NetworkState& operator=(const NetworkState& other);
    
    /*!
     * Copy constructor.
     */
    NetworkState(const NetworkState& rhs);
    
    /*!
     * Empty constructor. (Doesn't construct anything)
     */
    NetworkState()
    {
    }
};

class Network 
{
public:
    std::vector<SubLayer*> layers;
    
    NetworkState network_state;
    
    ~Network();
    
    
    unsigned int getOutputDepth(unsigned int input_depth)
    {
        for (SubLayer* layer : layers)
        {
            input_depth = layer->getOutputDepth(input_depth);
        }
        return input_depth;
    }
    
    /*!
     * \param n_input_features The number of input features for the new layer (only needed for the very first layer added)
     */
    void addLayer(LayerSettings settings, unsigned int n_input_features = 0);
    
    void initializeParams(std::function<float(float)> func);
    
    Dims3 getOutputDims(Dims3 input_dims);
    
    void forward();
    
    /*!
     * Take the local derivatives at the output layer and propagates them back through the network.
     */
    void backward(Mat3Df* in_derivatives = nullptr);

};

#endif // NETWORK_H
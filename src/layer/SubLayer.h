#ifndef SUB_LAYER_H
#define SUB_LAYER_H

#include <functional>
#include <vector>

#include "../utils/Mat3Df.h"
#include "../utils/Mat4Df.h"
#include "../utils/Dims.h"

class LayerParams 
{
public:
    Dims4 dims;
    Mat4Df params;
    Mat4Df ders;
    
    LayerParams(Dims4 dims)
    : dims(dims)
    , params(dims)
    , ders(dims)
    {
    }
};

class SubLayer 
{
public:
    std::vector<LayerParams> layer_params;
    
    virtual void initializeParams(std::function<float(float)> func) = 0;
    virtual Dims3 getOutputDims(Dims3 input_dims) = 0;
    virtual unsigned int getOutputDepth(unsigned int input_depth) = 0;
    virtual void forward(Mat3Df& in, Mat3Df& out) = 0;
    /*!
     * When no @p in_derivatives is given, the derivatives are only back propagated to the weights and not the inputs
     */
    virtual void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives = nullptr) = 0;
};

#endif // SUB_LAYER_H
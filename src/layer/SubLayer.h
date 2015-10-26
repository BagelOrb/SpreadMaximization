#ifndef SUB_LAYER_H
#define SUB_LAYER_H

#include <functional>

#include "../utils/Mat3Df.h"
#include "../utils/Dims.h"

class SubLayer 
{
public:
    virtual void initializeParams(std::function<float(float)> func) = 0;
    virtual Dims getOutputDims(Dims input_dims) = 0;
    virtual void forward(Mat3Df& in, Mat3Df& out) = 0;
    /*!
     * When no @p in_derivatives is given, the derivatives are only back propagated to the weights and not the inputs
     */
    virtual void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives) = 0;
};

#endif // SUB_LAYER_H
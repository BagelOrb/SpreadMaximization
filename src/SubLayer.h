#ifndef SUB_LAYER_H
#define SUB_LAYER_H

#include <functional>

#include "Mat3Df.h"

class SubLayer 
{
    virtual void initializeParams(std::function<float(float)> func) = 0;
    virtual void forward(Mat3Df& in, Mat3Df& out) = 0;
    virtual void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df& in_derivatives) = 0;
};

#endif // SUB_LAYER_H
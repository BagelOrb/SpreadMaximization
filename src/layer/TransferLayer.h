#ifndef TRANSFER_LAYER_H
#define TRANSFER_LAYER_H


#include <functional>

#include "SubLayer.h"
#include "TransferFunction.h"

class TransferLayer : public SubLayer
{
    TransferFunction* transfer_function;
public: 
    TransferLayer(TransferFunction& transfer_function)
    : transfer_function(&transfer_function)
    {
    }
    
    void initializeParams(std::function<float(float)>) { } // there are no params
    
    Dims3 getOutputDims(Dims3 input_dims)
    {
        return input_dims;
    }
    void forward(Mat3Df& in, Mat3Df& out)
    {
        assert(in.getDims() == out.getDims());
        for (Mat3Df::iterator in_it = in.begin(); in_it != in.end(); ++in_it)
        {
            out.set(in_it.getPos(), transfer_function->apply(*in_it));
        }
    }
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives)
    {
        assert(in.getDims() == out.getDims());
        assert(out_derivatives.getDims() == out.getDims());
        assert(!in_derivatives || out_derivatives.getDims() == in_derivatives->getDims());
        
        for (Mat3Df::iterator in_it = in.begin(); in_it != in.end(); ++in_it)
        {
            in_derivatives->set(in_it.getPos(), transfer_function->der(*in_it, out.get(in_it.getPos()) * out_derivatives.get(in_it.getPos())));
        }
    }





};

#endif // TRANSFER_LAYER_H
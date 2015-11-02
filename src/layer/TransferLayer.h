#ifndef TRANSFER_LAYER_H
#define TRANSFER_LAYER_H


#include <functional>

#include "SubLayer.h"
#include "TransferFunction.h"
#include "../Settings.h"

class TransferLayer : public SubLayer
{
    TransferFunction* transfer_function;
public: 
    TransferLayer(TransferFunctionType transfer_function_type)
    {
        switch (transfer_function_type)
        {
            case TransferFunctionType::Sigmoid:
                transfer_function = new SigmoidTransferFunction();
                break;
            case TransferFunctionType::Tanh:
                transfer_function = new TanhTransferFunction();
                break;
            case TransferFunctionType::Linear:
                transfer_function = new LinearTransferFunction();
                break;
        }
    }
    
    ~TransferLayer()
    {
        delete transfer_function;
    }
    
    void initializeParams(std::function<float(float)>) { } // there are no params
    
    unsigned int getOutputDepth(unsigned int input_depth)
    {
        return input_depth;
    }
    
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
        
        if (!in_derivatives)
        {
            return;
        }
        
        for (Mat3Df::iterator in_it = in.begin(); in_it != in.end(); ++in_it)
        {
            in_derivatives->set(in_it.getPos(), out_derivatives.get(in_it.getPos()) * transfer_function->der(*in_it, out.get(in_it.getPos()) * out_derivatives.get(in_it.getPos())));
        }
    }





};

#endif // TRANSFER_LAYER_H
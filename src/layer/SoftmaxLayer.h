#ifndef LAYER_SOFTMAX_LAYER_H
#define LAYER_SOFTMAX_LAYER_H

#include <cmath>

#include "SubLayer.h"

class SoftmaxLayer : public SubLayer 
{
public: 
    SoftmaxLayer()
    {
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
        float total_contribution = 0.0;
        for (Mat3Df::iterator in_it = in.begin(); in_it != in.end(); ++in_it)
        {
            float contribution = exp(*in_it);
            total_contribution += contribution;
            out.set(contribution);
        }
        out.applyInPlace([&total_contribution](float in){ return in/total_contribution; });
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
        
        float tot = 0.0;
        for (Mat3Df::iterator out_it = out.begin(); out_it != out.end(); ++out_it)
        {
            tot += *out_it * out_derivatives.get(out_it.getPos());
        }
        for (Mat3Df::iterator out_it = out.begin(); out_it != out.end(); ++out_it)
        {
            float on = *out_it;
            float de_don = out_derivatives.get(out_it.getPos());
            
            in_derivatives->set(out_it.getPos(), on * (de_don - tot));
        }
    }




};

#endif // LAYER_SOFTMAX_LAYER_H
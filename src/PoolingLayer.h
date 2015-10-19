#ifndef POOLING_LAYER_H
#define POOLING_LAYER_H

#include "SubLayer.h"
#include "utils/Pos.h"
#include "utils/Dims.h"
#include "utils/Mat3D.h"

template <class PoolingFunction>
class PoolingLayer : public SubLayer
{
    Dims2 field_size;
    Dims2 skip;
    Mat3D<PoolingFunction>* pool_states;
public:
    
    PoolingLayer(Dims2 field_size, Dims2 skip)
    : skip(skip)
    , field_size(field_size)
    , pool_states(nullptr)
    {
        
    }
    
    ~PoolingLayer()
    {
        if (pool_states)
            delete pool_states;
    }
        
    
    void initializeParams(std::function<float(float)> func)
    {
        // no params
    }
    
    Dims getOutputDims(Dims input_dims)
    {
        return Dims((input_dims.w - field_size.w) / skip.w + 1, (input_dims.h - field_size.h) / skip.h + 1, input_dims.d);
    }
    
    void forward(Mat3Df& in, Mat3Df& out)
    {
        if (pool_states && pool_states->getDims() != out.getDims())
        {
            delete pool_states;
            pool_states = nullptr;
        }
        if (!pool_states)
        {
            pool_states = new Mat3D<PoolingFunction>(out.getDims());
        }
        for (typename Mat3D<PoolingFunction>::iterator pool_it = pool_states->begin(); pool_it != pool_states->end(); pool_it++)
        {
            pool_it->reset();
        }
        
        
        for (Mat3Df::iterator out_it = out.begin(); out_it != out.end(); ++out_it)
        {
            *out_it = pool_states->get(out_it.getPos()).apply(in, out_it.getPos() * skip, field_size);
        }
    }
    
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives)
    {
        assert(pool_states);
        assert(in_derivatives);
        in_derivatives->clear();
        for (Mat3Df::iterator out_it = out.begin(); out_it != out.end(); ++out_it)
        {
            pool_states->get(out_it.getPos()).add_ders(in, out_it.getPos() * skip, field_size, *out_it, out_derivatives.get(out_it.getPos()), *in_derivatives);
        }
    }



};

#endif // POOLING_LAYER_H
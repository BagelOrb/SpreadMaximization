#ifndef POOLING_LAYER_IMPL_H
#define POOLING_LAYER_IMPL_H

#include "PoolingLayer.h"



template <class PoolingFunction>
void PoolingLayer<PoolingFunction>::forward(Mat3Df& in, Mat3Df& out)
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
    
    if (is_total_pooling_layer)
    {
        field_size = Dims2(in.getDims().w, in.getDims().h);
    }
    
    for (Mat3Df::iterator out_it = out.begin(); out_it != out.end(); ++out_it)
    {
        *out_it = pool_states->get(out_it.getPos()).apply(in, out_it.getPos() * skip, field_size);
    }
}


template <class PoolingFunction>
void PoolingLayer<PoolingFunction>::backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives)
{
    assert(pool_states);
    assert(in_derivatives);
    in_derivatives->clear();
    for (Mat3Df::iterator out_it = out.begin(); out_it != out.end(); ++out_it)
    {
        pool_states->get(out_it.getPos()).add_ders(in, out_it.getPos() * skip, field_size, *out_it, out_derivatives.get(out_it.getPos()), *in_derivatives);
    }
}


#endif // POOLING_LAYER_IMPL_H
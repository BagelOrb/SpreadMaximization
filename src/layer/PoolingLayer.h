#ifndef POOLING_LAYER_H
#define POOLING_LAYER_H

#include "SubLayer.h"
#include "../utils/Pos.h"
#include "../utils/Dims.h"
#include "../utils/Mat3D.h"

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
    
    Dims3 getOutputDims(Dims3 input_dims)
    {
        return Dims3((input_dims.w - field_size.w) / skip.w + 1, (input_dims.h - field_size.h) / skip.h + 1, input_dims.d);
    }
    
    void forward(Mat3Df& in, Mat3Df& out); // defined in PoolingLayerImpl.h
    
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives);  // defined in PoolingLayerImpl.h
};

#include "PoolingLayerImpl.h"

#endif // POOLING_LAYER_H
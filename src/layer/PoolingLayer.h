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
    bool is_total_pooling_layer;
public:
    
    PoolingLayer(Dims2 field_size, Dims2 skip, bool is_total_pooling_layer = false)
    : skip(skip)
    , field_size(field_size)
    , pool_states(nullptr)
    , is_total_pooling_layer(is_total_pooling_layer)
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
    
    unsigned int getOutputDepth(unsigned int input_depth)
    {
        return input_depth;
    }
    
    Dims3 getOutputDims(Dims3 input_dims)
    {
        if (is_total_pooling_layer)
        {
            return Dims3(1, 1, input_dims.d);
        }
        else 
        {
            assert(int(input_dims.w - field_size.w) >= 0);
            assert(int(input_dims.h - field_size.h) >= 0);
            return Dims3((input_dims.w - field_size.w) / skip.w + 1, (input_dims.h - field_size.h) / skip.h + 1, input_dims.d);
        }
    }
    
    void forward(Mat3Df& in, Mat3Df& out); // defined in PoolingLayerImpl.h
    
    void backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives);  // defined in PoolingLayerImpl.h
};

#include "PoolingLayerImpl.h"

#endif // POOLING_LAYER_H
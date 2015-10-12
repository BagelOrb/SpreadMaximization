#ifndef POOLING_H
#define POOLING_H

#include <cassert>

#include "Mat3Df.h"

class Pooling 
{
public:
    Dims pool_size;
    Dims2 skip;
    
    Pooling(Dims dims, Dims2 skip)
    : pool_size(dims)
    , skip(skip)
    { 
    }
    
    
    Dims getOutputDims(Dims input_dims)
    {
        return Dims((input_dims.w - pool_size.w) / skip.w + 1, (input_dims.h - pool_size.h) / skip.h + 1, input_dims.d);
    }
    
private:
    
    void _pool(Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(pool_size.w <= input.w && pool_size.h <= input.h && pool_size.d <= input.d);
        for (Mat3Df::iterator res_it = result.begin(); res_it != Mat3Df::iterator(0,0,1,result); ++res_it)
        {
            *res_it = 0;
//             for (Mat3Df::iterator k_it = skip.begin(); k_it != skip.end(); ++k_it)
//             {
//                 Mat3Df::Pos dataPos = res_it + k_it;
//                 result.add(res_it.getPos(), input.get(dataPos) * *k_it);
//             }
        }
    }
    
    void _poolMax()
    {
        
    }
public:
    Mat3Df pool(Mat3Df& input)
    {
        Mat3Df result(getOutputDims(input.getDims()));
        _pool(input, result, 0);
        return result;
    }
    Mat3Df pool(Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(result.getDims() == getOutputDims(input.getDims()));
        _pool(input, result, z);
        return result;
    }
    
    
    
    
    
};















#endif // POOLING_H
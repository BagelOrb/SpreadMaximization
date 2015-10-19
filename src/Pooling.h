#ifndef POOLING_H
#define POOLING_H

#include <cassert>
#include <limits>
#include <iostream> // temp

#include "Pos.h"
#include "utils/Mat3Df.h"

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
    
    void _pool(Mat3Df& input, Mat3Df& result)
    {
        assert(pool_size.w <= input.w && pool_size.h <= input.h && pool_size.d <= input.d);
        for (Mat3Df::iterator res_it = result.begin(); res_it != Mat3Df::iterator(0,0,1,result); ++res_it)
        {
            _poolMax(input, result, res_it);
        }
    }
    
    void _poolMax(Mat3Df& input, Mat3Df& result, Mat3Df::iterator res_it)
    {
        *res_it = std::numeric_limits<float>::lowest();
        for (Dims::iterator k_it = pool_size.begin(); k_it != pool_size.end(); ++k_it)
        {
            Pos dataPos = res_it * skip + k_it;
            *res_it = std::max(*res_it, input.get(dataPos));
        }
    }
public:
    Mat3Df pool(Mat3Df& input)
    {
        Mat3Df result(getOutputDims(input.getDims()));
        _pool(input, result);
        return result;
    }
    Mat3Df pool(Mat3Df& input, Mat3Df& result)
    {
        assert(result.getDims() == getOutputDims(input.getDims()));
        _pool(input, result);
        return result;
    }
    
    
    
    
    
};















#endif // POOLING_H
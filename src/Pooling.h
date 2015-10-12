#ifndef POOLING_H
#define POOLING_H

#include <cassert>

#include "Mat3Df.h"

class Pooling 
{
public:
    Mat3Df kernel;
    Mat3Df::Dims skip;
    Pooling(Mat3Df& kernel)
    : kernel(kernel)
    {  
    }
    
    Pooling(Mat3Df::Dims dims, Mat3Df::Dims skip)
    : kernel(dims)
    , skip(skip)
    { 
    }
    
    
    Mat3Df::Dims getOutputDims(Mat3Df::Dims input_dims)
    {
        return Mat3Df::Dims((input_dims.w() - kernel.w) / skip.w() + 1, (input_dims.h() - kernel.h) / skip.h() + 1, input_dims.d());
    }
    
private:
    
    void _pool(Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(kernel.w <= input.w && kernel.h <= input.h && kernel.d <= input.d);
        for (Mat3Df::iterator res_it = result.begin(); res_it != Mat3Df::iterator(0,0,1,result); ++res_it)
        {
            *res_it = 0;
            for (Mat3Df::iterator k_it = kernel.begin(); k_it != kernel.end(); ++k_it)
            {
                Mat3Df::Pos dataPos = res_it + k_it;
                result.add(res_it.getPos(), input.get(dataPos) * *k_it);
            }
        }
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
        assert(result.w == input.w - kernel.w + 1 
            && result.h == input.h - kernel.h + 1);
        _pool(input, result, z);
        return result;
    }
    
    
    
    
    
};















#endif // POOLING_H
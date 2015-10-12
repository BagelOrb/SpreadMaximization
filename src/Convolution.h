#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <cassert>

#include "Pos.h"
#include "Mat3Df.h"

class Convolution 
{
public:
    Mat3Df kernel;
    Convolution(Mat3Df& kernel)
    : kernel(kernel)
    {  
    }
    
    Convolution(Dims dims)
    : kernel(dims)
    { 
    }
    
    
    Dims2 getOutputDims(Dims input_dims)
    {
        return Dims2(input_dims.w - kernel.w + 1, input_dims.h - kernel.h + 1);
    }
    
private:
    
    void _convolute(Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(kernel.w <= input.w && kernel.h <= input.h && kernel.d <= input.d);
        for (Mat3Df::iterator res_it = result.begin(); res_it != Mat3Df::iterator(0,0,1,result); ++res_it)
        {
            *res_it = 0;
            for (Mat3Df::iterator k_it = kernel.begin(); k_it != kernel.end(); ++k_it)
            {
                Pos dataPos = res_it + k_it;
                result.add(res_it, input.get(dataPos) * *k_it);
            }
        }
    }
public:
    Mat3Df convolute(Mat3Df& input)
    {
        Dims2 out_dims = getOutputDims(input.getDims());
        Mat3Df result(Dims(out_dims.w, out_dims.h, 1));
        _convolute(input, result, 0);
        return result;
    }
    Mat3Df convolute(Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(result.w == input.w - kernel.w + 1 
            && result.h == input.h - kernel.h + 1);
        _convolute(input, result, z);
        return result;
    }
    
    
    
    
    
};















#endif // CONVOLUTION_H
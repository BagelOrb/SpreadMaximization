#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <cassert>

#include "Mat3Df.h"

class Convolution 
{
public:
    Mat3Df kernel;
    Convolution(Mat3Df& kernel)
    : kernel(kernel)
    {  
    }
    
    Convolution(Mat3Df::Dims dims)
    : kernel(dims)
    { 
    }
    
    
    std::pair<unsigned int, unsigned int> getOutputDims(Mat3Df::Dims input_dims)
    {
        return std::make_pair(input_dims.x - kernel.w + 1, input_dims.y - kernel.h + 1);
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
                Mat3Df::Pos dataPos = res_it + k_it;
                result.add(res_it.getPos(), input.get(dataPos) * *k_it);
            }
        }
    }
public:
    Mat3Df convolute(Mat3Df& input)
    {
        Mat3Df result(Mat3Df::Dims(input.w - kernel.w + 1, input.h - kernel.h + 1, 1));
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
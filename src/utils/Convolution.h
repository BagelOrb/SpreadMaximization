#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <cassert>

#include <iostream> // debug

#include "Pos.h"
#include "Dims.h"
#include "Mat3Df.h"
#include "Mat4Df.h"

class Convolution 
{
public:
    Mat3Df& kernel;
    Convolution(Mat3Df& kernel)
    : kernel(kernel)
    {
    }
    
    
    Dims2 getOutputDims(Dims3 input_dims)
    {
        return Dims2(input_dims.w - kernel.w + 1, input_dims.h - kernel.h + 1);
    }
    
private:
    
    void _convolute(Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(kernel.w <= input.w && kernel.h <= input.h && kernel.d <= input.d);
        for (Mat3Df::iterator res_it_xy = result.begin(); res_it_xy != Mat3Df::iterator(0,0,1,result); ++res_it_xy)
        {
            Mat3Df::iterator res_it(res_it_xy);
            res_it.z = z;
            *res_it = 0;
            for (Mat3Df::iterator k_it = kernel.begin(); k_it != kernel.end(); ++k_it)
            {
                Pos3 dataPos = res_it_xy + k_it;
                result.add(res_it, input.get(dataPos) * *k_it);
            }
        }
    }
public:
    Mat3Df convolute(Mat3Df& input)
    {
        Dims2 out_dims = getOutputDims(input.getDims());
        Mat3Df result(Dims3(out_dims.w, out_dims.h, 1));
        _convolute(input, result, 0);
        return result;
    }
    void convolute(Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(result.w == input.w - kernel.w + 1 
            && result.h == input.h - kernel.h + 1);
        _convolute(input, result, z);
    }
    
    
    
    
    
    
    

public:
    static Dims2 getOutputDims(Mat3Df& kernel, Dims3 input_dims)
    {
        return Dims2(input_dims.w - kernel.w + 1, input_dims.h - kernel.h + 1);
    }
    
private:
    
    static void _convolute(Mat3Df& kernel, Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(kernel.w <= input.w && kernel.h <= input.h && kernel.d <= input.d);
        for (Mat3Df::iterator res_it_xy = result.begin(); res_it_xy != Mat3Df::iterator(0,0,1,result); ++res_it_xy)
        {
            Mat3Df::iterator res_it(res_it_xy);
            res_it.z = z;
            *res_it = 0;
            for (Mat3Df::iterator k_it = kernel.begin(); k_it != kernel.end(); ++k_it)
            {
                Pos3 dataPos = res_it_xy + k_it;
                result.add(res_it, input.get(dataPos) * *k_it);
            }
        }
    }
public:
    static void convolute(Mat3Df& kernel, Mat3Df& input, Mat3Df& result, unsigned int z)
    {
        assert(result.w == input.w - kernel.w + 1 
            && result.h == input.h - kernel.h + 1);
        _convolute(kernel, input, result, z);
    }
    
    
    
    
    
    
    
    
    
    
    
    

public:
    static Dims2 getOutputDims(Mat4Df& kernels, Dims3 input_dims)
    {
        return Dims2(input_dims.w - kernels.w + 1, input_dims.h - kernels.h + 1);
    }
    
private:
    
    static void _convolute(Mat4Df& kernels, Mat3Df& input, Mat3Df& result)
    {
        assert(kernels.w <= input.w && kernels.h <= input.h && kernels.d <= input.d);
        for (Mat3Df::iterator res_it = result.begin(); res_it != result.end(); ++res_it)
        {
            *res_it = 0;
            for (Mat4Df::iterator k_it = Mat4Df::iterator(0,0,0,res_it.z, kernels); k_it != Mat4Df::iterator(0,0,0,res_it.z+1, kernels); ++k_it)
            {
                Pos3 dataPos(res_it.x + k_it.x, res_it.y + k_it.y, res_it.z);
                result.add(res_it, input.get(dataPos) * *k_it);
            }
        }
    }
public:
    static void convolute(Mat4Df& kernels, Mat3Df& input, Mat3Df& result)
    {
        assert(result.w == input.w - kernels.w + 1 
            && result.h == input.h - kernels.h + 1);
        _convolute(kernels, input, result);
    }
    
    
    
    
    
};















#endif // CONVOLUTION_H
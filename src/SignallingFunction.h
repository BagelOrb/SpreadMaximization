#ifndef SIGNALLING_FUNCTION_H
#define SIGNALLING_FUNCTION_H


#include "basics.h"



using namespace cv;

class SignallingFunction 
{
    template <class Func> 
    void apply(std::vector<Mat_<val>>& src, std::vector<Mat_<val>>& dst, Func& f)
    {
        assert(src.size() == dst.size());
        for (unsigned int channel_idx = 0; channel_idx < src.size(); channel_idx++)
        {
            Mat_<val>& src_ = src[channel_idx];
            Mat_<val>& dst_ = dst[channel_idx];
            assert(src_.rows == dst_.rows);
            assert(src_.cols == dst_.cols);
            for (int x = 0; x < src_.cols(); x++)
            {
                for (int y = 0; y < src_.rows(); y++)
                {
                    dst_.at<val>(x, y) = f(src_.at<val>(x, y));
                }
            }
        }
    }







};

#endif // SIGNALLING_FUNCTION_H
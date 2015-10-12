#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "opencv2/imgproc/imgproc.hpp"

#include "utils/NoCopy.h"

#include "Neuron.h"
#include "Layer.h"

using namespace cv;

class Convolution : NoCopy
{
    static Convolution instance;
    Convolution() 
    { 
    }
    
    int output_type = -1; // same is input type (CV_32U ?)
    Point anchor = Point(1, 1); // the first convoluted patch results in pixel (0,0) of the output
    int border_type = BORDER_CONSTANT;
    
/* http://docs.opencv.org/modules/imgproc/doc/filtering.html
 Various border types, image boundaries are denoted with '|'

 * BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
 * BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
 * BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
 * BORDER_WRAP:          cdefgh|abcdefgh|abcdefg
 * BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
 */
    
public:
    
    Convolution& getInstance() { return instance; }
    
    void convolute(std::vector<Mat_<val>>& src, Mat_<val>& dst, Neuron& neuron)
    {
        assert(src.size() > 0);
        assert(src.size() == neuron.weights.size());
        assert(dst.channels() == 1); // result is always a single output channel
        
        std::vector<Mat_<val>> channels_dst(neuron.weights.size());
        dst = neuron.bias; // set all values in the result to the bias
        for (int channel_idx = 0; channel_idx < src.size(); channel_idx++)
        {
            Mat_<val> input_channel_out;
            filter2D(src[channel_idx], input_channel_out, output_type , neuron.weights[channel_idx], anchor, 0, border_type );
            
            // add the totals from the current input channel to the output channel
            dst += input_channel_out;
        }
        
    }
    
    void convolute(std::vector<Mat_<val>>& src, std::vector<Mat_<val>>& dst, Layer& layer) 
    {
        assert(dst.size() == layer.neurons.size());
        for (unsigned int neuron_idx = 0; neuron_idx < layer.neurons.size(); neuron_idx++)
        {
            convolute(src, dst[neuron_idx], layer.neurons[neuron_idx]);
        }
        
    }


};


#endif // CONVOLUTION_H
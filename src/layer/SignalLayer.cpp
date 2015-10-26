#include "SignalLayer.h"


Dims3 SignalLayer::getOutputDims(Dims3 input_dims)
{
    Dims2 conv_out_dims = Convolution::getOutputDims(*weights, input_dims);
    return Dims3(conv_out_dims.w, conv_out_dims.h, weights->m);
}


void SignalLayer::initializeParams(std::function<float(float)> func)
{
    weights->apply(func);
    biases->apply(func);
}


void SignalLayer::forward(Mat3Df& in, Mat3Df& out)
{
    Convolution::convolute(*weights, in, out);
}


void SignalLayer::backward(Mat3Df& in, Mat3Df& out, Mat3Df& out_derivatives, Mat3Df* in_derivatives)
{
    assert(out.getDims() == out_derivatives.getDims());
    assert(!in_derivatives || in.getDims() == in_derivatives->getDims());
    if (in_derivatives) 
    {
        in_derivatives->apply([](float) { return 0; }); // reset derivatives at input mat to zero
    }
    
    weights_ders->clear();
    biases_ders->clear();
    
    for (Mat3Df::iterator out_it = out_derivatives.begin(); out_it != out_derivatives.end(); ++out_it)
    {
        float neuron_out_der = *out_it;
        for (Mat4Df::iterator bias_it = biases_ders->begin(); bias_it != biases_ders->end(); ++bias_it)
        {
            *bias_it += neuron_out_der;
        }
        for (Mat4Df::iterator weight_it = Mat4Df::iterator(0,0,0,out_it.z, *weights); weight_it != Mat4Df::iterator(0,0,0,out_it.z + 1, *weights); ++weight_it)
        {
            Pos3 in_pos(out_it.x + weight_it.x, out_it.y + weight_it.y, weight_it.z);
            float weight_der = neuron_out_der * in.get(in_pos);
            weights_ders->add(weight_it.getPos(), weight_der);
            if (in_derivatives)
            {
                in_derivatives->add(in_pos, neuron_out_der * *weight_it);
            }
        }
    }
}


//test convolution
#include <stdio.h>
#include <iostream> // std::cerr

#include "utils/Mat3Df.h"
#include "utils/Convolution.h"
#include "Pooling.h"
#include "utils/Pos.h"
#include "utils/Dims.h"

#include "SignalLayer.h"

#include "learning/GradientDescentFunction.h"

#include "TransferLayer.h"
#include "TransferFunction.h"

#include "PoolingFunction.h"
#include "PoolingLayer.h"

Mat3Df get_test_mat()
{
    int w = 5;
    int h = 5;
    int d = 3;
    Mat3Df m(Dims(w,h,d));
    float i = 0;
    for (int z = 0; z < d; z++)
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
        m.set(x,y,z, i);
        i++;
    }
    return m;
}

void test_convolution()
{
    Mat3Df m = get_test_mat();
    
    Mat3Df k(Dims(2, 2, 2));
    Convolution conv(k);
    for (int z = 0; z < k.d; z++)
    for (int y = 0; y < k.h; y++)
    for (int x = 0; x < k.w; x++)
    {
        k.set(x,y,z, 0);
        if (x == 0 && y == 0 && z == 0)
            k.set(x,y,z, 1);
    }

    auto func = [](float in) 
    { 
//         std::cerr << "sdg " <<std::endl; 
        return in + 1; 
    };
    
    Mat3Df r = conv.convolute(m).apply(func );
//     r = r.apply([](float in) {return in -1; } );
    
    r.debugOut();
    
    std::cerr << " pooled: " << std::endl;
    
    Pooling pool(Dims(2, 2, 1), Dims2(2, 2));
    
    r = pool.pool(r);
    
    r.debugOut();
}

void test_signalLayer()
{
    SignalLayer<GradientDescentFunction> layer(Dims(2, 2, 2), 2);
    layer.update_function_params = new GradientDescentParams();
    {
        layer.neurons[0].state.bias = 0.0;
        Mat3Df& k = layer.neurons[0].state.weights;
        k.apply([](float) { return 0; } );
        k.set(0,0,0, 1.0);
    }
    {
        layer.neurons[1].state.bias = 0.0;
        Mat3Df& k = layer.neurons[1].state.weights;
        k.apply([](float) { return 0; } );
        k.set(1,1,1, 1.0);
    }
    
    Mat3Df input = get_test_mat();
    input.debugOut("input");
    
//     std::cerr << "input dims: " << input.w << ", " << input.h << ", " << input.d << "\n";
    
    Mat3Df output(layer.getOutputDims(input.getDims()));
    
//     std::cerr << "output dims: " << output.w << ", " << output.h << ", " << output.d << "\n";
    
    layer.forward(input, output);
    
    output.debugOut("output");
    
    // =======================================================================================
    
    Mat3Df out_ders(output.getDims());
    out_ders.clear(1.0f);
    
    Mat3Df in_ders(input.getDims());
    
    layer.backward(input, output, out_ders, &in_ders);
    
    
    in_ders.debugOut("input derivatives");
    
    for (Neuron& neuron : layer.neurons)
    {
        neuron.state.weights.debugOut("neuron weights");
    }
}

void test_transferLayer()
{
    Mat3Df input = get_test_mat();
    input.debugOut("input");
    
    SigmoidTransferFunction transfer_function;
    TransferLayer layer(transfer_function);
    
    Mat3Df output(layer.getOutputDims(input.getDims()));
    
    layer.forward(input, output);
    
    output.debugOut("output");
    
    // =======================================================================================
    
    Mat3Df out_ders(output.getDims());
    out_ders.clear(1.0f);
    
    Mat3Df in_ders(input.getDims());
    
    layer.backward(input, output, out_ders, &in_ders);
    
    
    in_ders.debugOut("input derivatives");
}



void test_poolingLayer()
{
    Mat3Df input = get_test_mat();
    input.debugOut("input");
    
    PoolingLayer<MaxPoolingFunction> layer(Dims2(2,2), Dims2(2,2));
    
    Mat3Df output(layer.getOutputDims(input.getDims()));
    
    layer.forward(input, output);
    
    output.debugOut("output");
    
    // =======================================================================================
    
    Mat3Df out_ders(output.getDims());
    out_ders.clear(1.0f);
    
    Mat3Df in_ders(input.getDims());
    
    layer.backward(input, output, out_ders, &in_ders);
    
    
    in_ders.debugOut("input derivatives");
}



int main ( int argc, char** argv )
{
//     test_signalLayer();
//     test_transferLayer();
    test_poolingLayer();
    return 0;
}

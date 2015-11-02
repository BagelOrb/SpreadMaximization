
//test convolution
#include <stdio.h>
#include <iostream> // std::cerr
#include <iomanip> // std cerr precision

#include "utils/Mat3Df.h"
#include "utils/Convolution.h"
#include "layer/Pooling.h"
#include "utils/Pos.h"
#include "utils/Dims.h"

#include "layer/SignalLayer.h"

#include "learning/GradientDescentFunction.h"

#include "layer/TransferLayer.h"
#include "layer/TransferFunction.h"

#include "layer/PoolingFunction.h"
#include "layer/PoolingLayer.h"

#include "Network.h"
#include "layer/Layer.h"

#include "learning/Updater.h"
#include "learning/ObjectiveFunction.h"

Mat3Df get_test_mat()
{
    int w = 5;
    int h = 5;
    int d = 3;
    Mat3Df m(Dims3(w,h,d));
    float i = 0;
    for (int z = 0; z < d; z++)
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
        m.set(Pos3(x,y,z), i);
        i++;
    }
    return m;
}

Mat3Df get_test_mat_rand()
{
    int w = 5;
    int h = 5;
    int d = 3;
    Mat3Df m(Dims3(w,h,d));
    float i = 0;
    for (int z = 0; z < d; z++)
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
        m.set(Pos3(x,y,z), (rand() % 100) / 5.0 - 10.0 );
        i++;
    }
    return m;
}

void test_convolution()
{
    Mat3Df m = get_test_mat();
    
    Mat3Df k(Dims3(2, 2, 2));
    Convolution conv(k);
    for (int z = 0; z < k.d; z++)
    for (int y = 0; y < k.h; y++)
    for (int x = 0; x < k.w; x++)
    {
        k.set(Pos3(x,y,z), 0);
        if (x == 0 && y == 0 && z == 0)
            k.set(Pos3(x,y,z), 1);
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
    
    Pooling pool(Dims3(2, 2, 1), Dims2(2, 2));
    
    r = pool.pool(r);
    
    r.debugOut();
}

void test_signalLayer()
{
    SignalLayer layer(Dims3(2, 2, 2), 2);
    
    layer.biases->clear();
    layer.weights->clear();
    layer.weights->set(Pos4(0,0,0,0), 1.0);
    layer.weights->set(Pos4(1,1,1,1), 1.0);
    
    Mat3Df input = get_test_mat();
    input.debugOut("input");
    
//     std::cerr << "input dims: " << input.w << ", " << input.h << ", " << input.d << "\n";
    
    Mat3Df output(layer.getOutputDims(input.getDims()));
    
//     std::cerr << "output dims: " << output.w << ", " << output.h << ", " << output.d << "\n";
    
    layer.forward(input, output);
    
    output.debugOut("output");
    
    for (Mat3Df::iterator out_it = output.begin(); out_it != output.end(); ++out_it)
    {
        if (out_it.z == 0)
        {
            assert(*out_it == input.get(out_it.getPos()));
        }
        else if (out_it.z == 1)
        {
            assert(*out_it == input.get(out_it.getPos() + Pos3(1,1,0)));
        }
        else 
            assert(*out_it == 0);
    }
    
    // =======================================================================================
    
    Mat3Df out_ders(output.getDims());
    out_ders.clear(1.0f);
    
    Mat3Df in_ders(input.getDims());
    
    layer.backward(input, output, out_ders, &in_ders);
    
    for (Mat3Df::iterator it = in_ders.begin(); it != in_ders.end(); ++it)
    {
        if (it.z == 0)
        {
            if (it.x < in_ders.w - 1 && it.y < in_ders.h - 1)
                assert(*it == 1);
            else 
                assert(*it == 0);
        }
        else if (it.z == 1)
        {
            if (it.x==0 || it.y==0)
                assert(*it == 0);
            else 
                assert(*it == 1);
        }
        else 
            assert(*it == 0);
    }
    
    
    in_ders.debugOut("input derivatives");
    
    layer.weights->debugOut("neuron weights");
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
    Mat3Df input = get_test_mat_rand();
//     Mat3Df input = get_test_mat();
    input.debugOut("input");
    
    
//     PoolingLayer<SoftAbsMaxPoolingFunction> layer(Dims2(2,2), Dims2(2,2));
//     PoolingLayer<SoftSquareMaxPoolingFunction> layer(Dims2(2,2), Dims2(2,2));
//     PoolingLayer<SoftSquareMaxPoolingFunction2> layer(Dims2(2,2), Dims2(2,2));
    PoolingLayer<SoftAbsMaxPoolingFunction2> layer(Dims2(2,2), Dims2(2,2));
    
    Mat3Df output(layer.getOutputDims(input.getDims()));
    
    layer.forward(input, output);
    
    output.debugOut("output");
    
    // =======================================================================================
    
    Mat3Df out_ders(output.getDims());
    out_ders.clear(1.0f);
    for (Mat3Df::iterator it = out_ders.begin(); it != out_ders.end(); ++it)
    { // set every second feature maps derivatives to minus one
        if (it.getPos().z % 2 == 1)
            *it = -1.0;
    }
    
    Mat3Df in_ders(input.getDims());
    
    layer.backward(input, output, out_ders, &in_ders);
    
    
    in_ders.debugOut("input derivatives");
}

void test_network()
{
    Mat3Df input = get_test_mat();
    input.applyInPlace([](float in) { return in/10; });
    input.debugOut("input");
    
//     TanhTransferFunction transfer_function;
    LinearTransferFunction transfer_function;
    
    Network network;
    network.addLayer(LayerSettings(PoolType::Max, TransferFunctionType::Tanh, 3, 2, 2, &transfer_function), input.d);
    
    bool set = false;
    network.initializeParams(
        [&set](float) 
        { 
            if (set)
                return 0.0; 
            set = true;
            return 1.0; // only set the very first param to 1
        });
    
//     network.layers.front()->layer_params.front().params.debugOut("first layer params");
    
    network.network_state.initialize(network.layers, input);
    
    network.forward();
    
    for (LayerState& state : network.network_state.layer_states)
    {
        state.output.debugOut("network output");
    }
    /*
    for (Pos3 p : state.output.getDims())
    {
        if (p == Pos3(0,0,0)) assert(std::abs(state.output.get(p) - 0.54) < 0.01);
        else if (p == Pos3(1,0,0)) assert(std::abs(state.output.get(p) - 0.66) < 0.01);
        else if (p == Pos3(0,1,0)) assert(std::abs(state.output.get(p) - 0.92) < 0.01);
        else if (p == Pos3(1,1,0)) assert(std::abs(state.output.get(p) - 0.95) < 0.01);
        else assert(state.output.get(p) == 0);
    }*/
    
    GradientDescentUpdater updater(0.1);
    
    BSobjectiveFunction objective;
    
    objective.ObjectiveFunction::setOutputDerivatives(network);
    
    Mat3Df input_ders(input.getDims());
    network.backward(&input_ders);
    
    
    input_ders.debugOut("input derivatives");
    for (LayerState& state : network.network_state.layer_states)
    {
        state.output_ders.debugOut("derivatives");
    }
    
    
    network.layers.front()->layer_params.front().ders.debugOut("first layer derivatives");
    
}

int main ( int argc, char** argv )
{
    std::cerr << std::fixed;
    std::cerr << std::setprecision(2);
    
//     test_signalLayer();
//     std::cerr << "\n\n\n==========================================================================\n\n\n";
//     test_transferLayer();
//     std::cerr << "\n\n\n==========================================================================\n\n\n";
//     test_poolingLayer();
    
    test_network();
    return 0;
}

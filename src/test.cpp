
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

#include "NetworkProcessor.h"
#include "learning/ParamInitializer.h"

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
    
    TransferLayer layer(TransferFunctionType::Sigmoid);
    
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
//     input.applyInPlace([](float in) { return in/10; });
    input.debugOut("input");
    
    Network network;
    network.addLayer(LayerSettings(PoolType::Max, TransferFunctionType::Linear, 3, 2, 2), input.d);
    
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
    
    
    GradientDescentUpdater updater(0.0001);
    
    TestObjectiveFunction objective;
    
    updater.initializeMetaParams(network);
    
    
    
    
    network.network_state.initialize(network.layers, input);
    
    network.forward();
    
    for (LayerState& state : network.network_state.layer_states)
    {
        state.output.debugOut("network output");
    }
    
    LayerState& state = network.network_state.layer_states.back();
    for (Pos3 p : state.output.getDims())
    {
        if (p == Pos3(0,0,0)) assert(state.output.get(p) == 6);
        else if (p == Pos3(1,0,0)) assert(state.output.get(p) == 8);
        else if (p == Pos3(0,1,0)) assert(state.output.get(p) == 16);
        else if (p == Pos3(1,1,0)) assert(state.output.get(p) == 18);
        else assert(state.output.get(p) == 0);
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
    
    objective.ObjectiveFunction::setOutputDerivatives(network);
    
    Mat3Df input_ders(input.getDims());
    network.backward(&input_ders);
    
    
    input_ders.debugOut("input derivatives");
    for (LayerState& state : network.network_state.layer_states)
    {
        state.output_ders.debugOut("derivatives");
    }
    for (Pos3 p : input_ders.getDims())
    {
        if (p == Pos3(1,1,0)) assert(input_ders.get(p) == 1.0);
        else assert(input_ders.get(p) == 0.0);
    }
    
    network.layers.front()->layer_params.front().ders.debugOut("first layer weight derivatives");
    network.layers.front()->layer_params.back().ders.debugOut("first layer bias derivatives");
    
    updater.update(network);
    
    
    network.forward();
    
    for (LayerState& state : network.network_state.layer_states)
    {
        state.output.debugOut("network output");
    }
}

void test_input_derivatives(Network& network, Mat3Df& input)
{
    NetworkProcessor processor(ProcessorSettings(0.0)); // don't really update
    
    Mat3Df input_derivatives(input.getDims());
    
    Mat3Df variable_input = input;
    
    processor.process(network, variable_input, &input_derivatives);
    
    input_derivatives.debugOut("input_derivatives");
    
    NetworkState state = network.network_state; 
    
    float output = network.network_state.layer_states.back().output.get(Pos3(0,0,0));
    
    float delta = 0.001;
    
    for (Pos3 pos : input.getDims())
    {
        Mat3Df updated_input(input);
        updated_input.add(pos, delta);
        
        network.network_state.initialize(network.layers, updated_input);
        network.forward();
        
        float updated_output = network.network_state.layer_states.back().output.get(Pos3(0,0,0));
        
        std::cerr << output << "\t" <<updated_output << "\t" << (updated_output - output) << " \t ";
        std::cerr << "computed der = " << input_derivatives.get(pos) << "\tactual der = " << ((updated_output - output) / delta) << std::endl;
    }
    
}


void test_network_derivatives()
{
    Mat3Df input = get_test_mat();
    
    input.debugOut("input");
    
    Network network;
//     network.addLayer(LayerSettings(PoolType::Max, TransferFunctionType::Linear, 3, 2, 2), input.d);
    network.addLayer(LayerSettings(PoolType::SoftSquareMax, TransferFunctionType::Linear, 3, 2, 2), input.d);
//     network.addLayer(LayerSettings(PoolType::SoftAbsMax, TransferFunctionType::Linear, 3, 2, 2), input.d);
    
//     network.layers.push_back(new PoolingLayer<SoftAbsMaxPoolingFunction>(Dims2(2,2), Dims2(2,2)));
    
    bool set = false;
    network.initializeParams(
        [&set](float) 
        { 
            if (set)
                return 0.0; 
            set = true;
            return 1.0; // only set the very first param to 1
        });
    
    
    
    test_input_derivatives(network, input);
    
}


void test_network_processor()
{
    Mat3Df input = get_test_mat();
    
    input.debugOut("input");
    
    Network network;
//     network.addLayer(LayerSettings(PoolType::Max, TransferFunctionType::Linear, 3, 2, 2), input.d);
    network.addLayer(LayerSettings(PoolType::SoftSquareMax, TransferFunctionType::Linear, 3, 2, 2), input.d);
//     network.addLayer(LayerSettings(PoolType::SoftAbsMax, TransferFunctionType::Linear, 3, 2, 2), input.d);
    
    bool set = false;
    network.initializeParams(
        [&set](float) 
        { 
            if (set)
                return 0.0; 
            set = true;
            return 1.0; // only set the very first param to 1
        });
    
    
    NetworkProcessor processor(ProcessorSettings(0.0001));
    
    processor.process(network, input);
    
    LayerState& state = network.network_state.layer_states.back();
    
    
    
    state.output.debugOut("output");
//     for (Pos3 p : state.output.getDims())
//     {
//         if (p == Pos3(0,0,0)) assert(state.output.get(p) == 6);
//         else if (p == Pos3(1,0,0)) assert(state.output.get(p) == 8);
//         else if (p == Pos3(0,1,0)) assert(state.output.get(p) == 16);
//         else if (p == Pos3(1,1,0)) assert(state.output.get(p) == 18);
//         else assert(state.output.get(p) == 0);
//     }

    for (LayerState& state : network.network_state.layer_states)
    {
        state.output.debugOut("network output");
    }

    for (LayerState& state : network.network_state.layer_states)
    {
        state.output_ders.debugOut("derivatives");
    }
    
    for (unsigned int der : network.network_state.layer_states.front().output_ders)
    {
        assert(der < 1.0); // the output derivative is a total of one, so all dereivatives must be smaller than one
    }
    
}


void test_network_init()
{
    Mat3Df input = get_test_mat();
    
    
    Network network;
    network.addLayer(LayerSettings(PoolType::Max, TransferFunctionType::Linear, 3, 2, 2), input.d);
    
    bool set = false;
//     network.initializeParams(ParamInitializer::uniformRandomTanh());
    network.initializeParams(ParamInitializer::nonuniformRandomTanh());
    
    for (SubLayer* layer : network.layers)
    {
        std::cerr << "||||||||||||||||||||||||||||||||||||||||||||||\n";
        for (LayerParams& params : layer->layer_params)
            params.params.debugOut("params");
    }
    

    
}

int main ( int argc, char** argv )
{
    std::cerr << std::fixed;
    std::cerr << std::setprecision(5);
    
//     test_signalLayer();
//     std::cerr << "\n\n\n==========================================================================\n\n\n";
//     test_transferLayer();
//     std::cerr << "\n\n\n==========================================================================\n\n\n";
//     test_poolingLayer();
    
//     test_network();
//     test_network_processor();
    test_network_derivatives();
//     test_network_init();
    return 0;
}

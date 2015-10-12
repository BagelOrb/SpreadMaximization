#ifndef LAYER_H
#define LAYER_H

#include <cassert> // assert
#include <vector>
#include <cmath> // tanh
#include <math.h>       /* exp */

#include "Pos.h"
#include "Mat3Df.h"

class Layer
{
    
    std::vector<Mat3Df> weights;
    std::vector<Mat3Df> weights_derivatives;
    std::vector<float> biases;
    std::vector<float> biases_derivatives;
    
    class State
    {
//         Mat3Df input;
        Mat3Df activation;
        Mat3Df convoluted;
        Mat3Df pooled;
        
        Mat3Df pooled_derivatives;
        Mat3Df convoluted_derivatives;
        
        unsigned int pool_size = 2;
        
        
        float soft_arg_max_abs(Pos conv_pos, unsigned int w, unsigned int h, unsigned int z, float hardness)
        {
            double denom = 0;
            double total = 0;
            for (unsigned int x = 0; x < w; x++)
            for (unsigned int y = 0; y < h; y++)
            {
                double in = convoluted.get(x, y, z);
                double w = std::exp(hardness * std::abs(in));
                total += w * in;
                denom += w;
            }
            return total / denom;
        }
        
        void pool()
        {
            assert(pooled.d == convoluted.d 
                && pooled.w == convoluted.w / pool_size
                && pooled.h == convoluted.h / pool_size);
            for (int x = 0; x < convoluted.x; x++)
            for (int y = 0; y < convoluted.h; y++)
            for (int z = 0; z < convoluted.d; z++)
            {
                pooled.set(x, y, z, soft_arg_max_abs(Pos(x*pool_size, y*pool_size, z), pool_size, pool_size, 1));
            }
        }
        
        void signal(Mat3Df input)
        {
            auto signallingFunction = std::tanh; //[] (float in) { return 0; };
            
            for (int neuron = 0; neuron < weights.size(); neuron++)
            {
                input.convolute(weights[neuron], activation, neuron);
                for (int x = 0; x < convoluted.x; x++)
                for (int y = 0; y < convoluted.h; y++)
                for (int z = 0; z < convoluted.d; z++)
                {
                    activation.add(x, y, z, biases[z]);
                }
                
                activation.apply(signallingFunction, convoluted);
                
                pool();
            }
        }
    };
};


#endif//LAYER_H

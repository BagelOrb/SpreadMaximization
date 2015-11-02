#ifndef PARAM_INITIALIZER_H
#define PARAM_INITIALIZER_H

#include <random>
#include <functional>

#include "../utils/NoCopy.h"

std::random_device _param_initializer_rd;

//
// Engines 
//
std::mt19937 _param_initializer_e2(_param_initializer_rd());
//std::knuth_b e2(rd());
//std::default_random_engine e2(rd()) ;

//
// Distribtuions
//
std::uniform_real_distribution<float> _param_initializer_dist(-1, 1);
//std::normal_distribution<> dist(2, 2);
//std::student_t_distribution<> dist(5);
//std::poisson_distribution<> dist(2);
//std::extreme_value_distribution<> dist(0,2);

class ParamInitializer : public NoCopy
{
    void testVisualize()
    {
        int size = 40;
        int a[size];
        for (int i = 0; i < size; i++) a[i] = 0;
        for (int i = 0; i < 100000; i++)
        {
            float r = ParamInitializer::nonuniformRandomTanh()(0);
            int as = std::min(size-1, std::max(0, static_cast<int>( (r + 1.0) *.5 * size )));
            a[as]++;
        }
        for (int i = 0; i < size; i++)
        {
            for ( ; a[i] > 0 ; a[i] -=500)
                std::cerr << "=";
            std::cerr << "\n";
        }
    }
    
    
public:
    /*!
     * Return a uniform random number between -1 and 1
     */
    static std::function<float(float)> uniformRandomTanh()
    {
        return [&] (float) { return _param_initializer_dist(_param_initializer_e2); };
    }
    
    /*!
     * Return a non-uniform random number between -1 and 1.
     * 
     * Just a funny distribution to try...
     */
    static std::function<float(float)> nonuniformRandomTanh()
    {
        return [&] (float) 
        { 
            float r = _param_initializer_dist(_param_initializer_e2);
            return r * r * r; 
        };
    }
    
};

#endif // PARAM_INITIALIZER_H
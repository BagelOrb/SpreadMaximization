#ifndef POOLING_FUNCTION_H
#define POOLING_FUNCTION_H

#include <limits>
#include <cmath> // exp

#include "../utils/Mat3Df.h"
#include "../utils/Dims.h"
#include "../utils/Pos.h"

#include "../utils/DifferentiableFunction.h"

class PoolingFunction
{
public:
    virtual void reset() = 0;
    virtual float apply(Mat3Df& in, Pos lu_start_pos, Dims2 field_size) = 0;
    virtual float add_ders(Mat3Df& in, Pos lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders) = 0;
    
};

class MaxPoolingFunction : public PoolingFunction
{
public:
    Pos max_pos;
    
    MaxPoolingFunction()
    : max_pos(-1,-1,-1)
    { }
        
    
    void reset() 
    {
    }
    
    float apply(Mat3Df& in, Pos lu_start_pos, Dims2 field_size);
    
    /*!
     * \param in_ders (output param) input derivatives
     */
    float add_ders(Mat3Df& in, Pos lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders);
        
};

class AbsMaxPoolingFunction : public PoolingFunction
{
public:
    Pos max_pos;
    
    AbsMaxPoolingFunction()
    : max_pos(-1,-1,-1)
    { }
        
    
    void reset() 
    {
    }
    
    float apply(Mat3Df& in, Pos lu_start_pos, Dims2 field_size);
    
    /*!
     * \param in_ders (output param) input derivatives
     */
    float add_ders(Mat3Df& in, Pos lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders);

};

class SoftArgMaxPoolingFunction : public PoolingFunction
{
private:
    Mat3Df* weights;
    float total_weight;
    DifferentiableFunction& inner_function;
public:
    float hardness;
    
    SoftArgMaxPoolingFunction(DifferentiableFunction& inner_function)
    : inner_function(inner_function)
    , hardness(1.0)
    { }
        
    
    void reset() 
    {
    }
    
private:
    Mat3Df& getWeights(Dims2 field_size);
    
public:
    float apply(Mat3Df& in, Pos lu_start_pos, Dims2 field_size);
    
    /*!
     * \param in_ders (output param) input derivatives
     */
    float add_ders(Mat3Df& ins, Pos lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders);        
};

class SoftAbsMaxPoolingFunction : public SoftArgMaxPoolingFunction
{
    Abs _inner_function;
public:
    SoftAbsMaxPoolingFunction()
    : SoftArgMaxPoolingFunction(_inner_function)
    {
        hardness = 1.0;
    }
};

class SoftSquareMaxPoolingFunction : public SoftArgMaxPoolingFunction
{
    Square _inner_function;
public:
    SoftSquareMaxPoolingFunction()
    : SoftArgMaxPoolingFunction(_inner_function)
    {
        hardness = 0.1;
    }
};

class SoftSquareMaxPoolingFunction2 : public SoftArgMaxPoolingFunction
{
    class Square2 : public DifferentiableFunction
    {
    public:
        float apply(float in) { return in * in - 10; }
        float der(float in) { return .5* in; }
    };
    Square2 _inner_function;
public:
    SoftSquareMaxPoolingFunction2()
    : SoftArgMaxPoolingFunction(_inner_function)
    {
        hardness = 0.1;
    }
};

class SoftAbsMaxPoolingFunction2 : public SoftArgMaxPoolingFunction
{
    class Square2 : public DifferentiableFunction
    {
    public:
        float apply(float in) { return std::abs(in) - 10; }
        float der(float in) { return (in > 0)? 1 : -1; }
    };
    Square2 _inner_function;
public:
    SoftAbsMaxPoolingFunction2()
    : SoftArgMaxPoolingFunction(_inner_function)
    {
        hardness = 0.1;
    }
};

#endif // POOLING_FUNCTION_H
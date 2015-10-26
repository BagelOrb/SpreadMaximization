#ifndef POOLING_FUNCTION_H
#define POOLING_FUNCTION_H

#include <limits>
#include <cmath> // exp

#include "../utils/Mat3Df.h"
#include "../utils/Dims.h"
#include "../utils/Pos.h"

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


class SoftAbsMaxPoolingFunction : public PoolingFunction
{
private:
    Mat3Df* weights;
    float total_weight;
public:
    float hardness;
    
    SoftAbsMaxPoolingFunction()
    : hardness(1.0)
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


#endif // POOLING_FUNCTION_H
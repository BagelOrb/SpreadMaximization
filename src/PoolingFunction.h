#ifndef POOLING_FUNCTION_H
#define POOLING_FUNCTION_H

#include <limits>
#include <cmath> // exp

#include "utils/Mat3Df.h"
#include "utils/Dims.h"
#include "utils/Pos.h"

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
    
    float apply(Mat3Df& in, Pos lu_start_pos, Dims2 field_size)
    {
        float max = std::numeric_limits<float>::lowest();
        for (Pos pool_pos : field_size)
        {
            Pos in_pos = lu_start_pos + pool_pos;
            if (in.get(in_pos) > max)
            {
                max = in.get(in_pos);
                max_pos = in_pos;
            }
        }
    }
    
    /*!
     * \param in_ders (output param) input derivatives
     */
    float add_ders(Mat3Df& in, Pos lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders)
    {
        in_ders.add(max_pos, out_der);
    }
        
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
    
    float apply(Mat3Df& in, Pos lu_start_pos, Dims2 field_size)
    {
        float max = 0;
        for (Pos pool_pos : field_size)
        {
            Pos in_pos = lu_start_pos + pool_pos;
            if (std::abs(in.get(in_pos)) > max)
            {
                max = std::abs(in.get(in_pos));
                max_pos = in_pos;
            }
        }
    }
    
    /*!
     * \param in_ders (output param) input derivatives
     */
    float add_ders(Mat3Df& in, Pos lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders)
    {
        in_ders.add(max_pos, out_der);
    }
        
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
    
    Mat3Df& getWeights(Dims2 field_size)
    {
        if (weights && (weights->getDims().w != field_size.w || weights->getDims().h != field_size.h))
        {
            delete weights;
            weights = nullptr;
        }
        if (!weights)
        {
            weights = new Mat3Df(Dims(0,0,1) + field_size);
        }
        return *weights;
    }
    
    float apply(Mat3Df& in, Pos lu_start_pos, Dims2 field_size)
    {
        Mat3Df& weights = getWeights(field_size);
        float total = 0.0;
        total_weight = 0.0;
        for (Pos pool_pos : field_size)
        {
            Pos in_pos = lu_start_pos + pool_pos;
            float in_val = in.get(in_pos);
            float weight = exp(std::abs(hardness * in_val));
            total_weight += weight;
            weights.set(pool_pos, weight);
            float contribution = weight * in_val;
            total += contribution;
        }
        return total / total_weight;
    }
    
    /*!
     * \param in_ders (output param) input derivatives
     */
    float add_ders(Mat3Df& ins, Pos lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders)
    {
        assert(weights);
        for (Pos pool_pos : field_size)
        {
            Pos in_pos = lu_start_pos + pool_pos;
            float weighted_weight = weights->get(pool_pos) / total_weight; // c_n
            float in = ins.get(in_pos);
            float df_din = (in > 0)? 1.0 : -1.0;
            in_ders.add(in_pos, out_der * weighted_weight * ( 1.0 + df_din * (in - out)));
        }
    }
        
};


#endif // POOLING_FUNCTION_H
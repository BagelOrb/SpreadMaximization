#ifndef POOLING_FUNCTION_H
#define POOLING_FUNCTION_H

#include <limits>

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

#endif // POOLING_FUNCTION_H
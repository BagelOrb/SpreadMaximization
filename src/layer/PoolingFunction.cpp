#include "PoolingFunction.h"

#include "../utils/DifferentiableFunction.h"

float MaxPoolingFunction::apply(Mat3Df& in, Pos3 lu_start_pos, Dims2 field_size)
{
    float max = std::numeric_limits<float>::lowest();
    for (Pos3 pool_pos : field_size)
    {
        Pos3 in_pos = lu_start_pos + pool_pos;
        if (in.get(in_pos) > max)
        {
            max = in.get(in_pos);
            max_pos = in_pos;
        }
    }
    return max;
}

float MaxPoolingFunction::add_ders(Mat3Df& in, Pos3 lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders)
{
    in_ders.add(max_pos, out_der);
}


float AbsMaxPoolingFunction::apply(Mat3Df& in, Pos3 lu_start_pos, Dims2 field_size)
{
    float max = -1;
    for (Pos3 pool_pos : field_size)
    {
        Pos3 in_pos = lu_start_pos + pool_pos;
        if (std::abs(in.get(in_pos)) > max)
        {
            max = std::abs(in.get(in_pos));
            max_pos = in_pos;
        }
    }
    return max;
}

float AbsMaxPoolingFunction::add_ders(Mat3Df& in, Pos3 lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders)
{
    if (in.get(max_pos) < 0)
    {
        in_ders.add(max_pos, - out_der);
    }
    else 
    {
        in_ders.add(max_pos, out_der);
    }
}

Mat3Df& SoftArgMaxPoolingFunction::getWeights(Dims2 field_size)
{
    if (weights && (weights->getDims().w != field_size.w || weights->getDims().h != field_size.h))
    {
        delete weights;
        weights = nullptr;
    }
    if (!weights)
    {
        weights = new Mat3Df(Dims3(0,0,1) + field_size);
    }
    return *weights;
}

float SoftArgMaxPoolingFunction::apply(Mat3Df& in, Pos3 lu_start_pos, Dims2 field_size)
{
    Mat3Df& weights = getWeights(field_size);
    float total = 0.0;
    total_weight = 0.0;
    for (Pos3 pool_pos : field_size)
    {
        Pos3 in_pos = lu_start_pos + pool_pos;
        float in_val = in.get(in_pos);
        float weight = exp(hardness * inner_function.apply(in_val));
        total_weight += weight;
        weights.set(pool_pos, weight);
        float contribution = weight * in_val;
        total += contribution;
    }
    assert (!isinf(total)); // weights of layer below are too large or hardness is too high
    return total / total_weight;
}

float SoftArgMaxPoolingFunction::add_ders(Mat3Df& ins, Pos3 lu_start_pos, Dims2 field_size, float out, float out_der, Mat3Df& in_ders)
{
    assert(weights);
    for (Pos3 pool_pos : field_size)
    {
        Pos3 in_pos = lu_start_pos + pool_pos;
        float weighted_weight = weights->get(pool_pos) / total_weight; // c_n
        float in = ins.get(in_pos);
        float df_din = hardness * inner_function.der(in);
        in_ders.add(in_pos, out_der * weighted_weight * ( 1.0 + df_din * (in - out)));
    }
}





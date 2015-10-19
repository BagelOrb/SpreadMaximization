#ifndef GRADIENT_DESCENT_FUNCTION_H
#define GRADIENT_DESCENT_FUNCTION_H

#include "UpdateFunction.h"

class GradientDescentParams : public UpdateFunctionParams
{
public:
    float delta = 0.01;
    
};

class GradientDescentFunction : public UpdateFunctionState
{
public:
    void update(float& param, UpdateFunctionParams* params)
    {
        param += der * static_cast<GradientDescentParams*>(params)->delta;
    }
};

#endif // GRADIENT_DESCENT_FUNCTION_H
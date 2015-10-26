#ifndef GRADIENT_DESCENT_FUNCTION_H
#define GRADIENT_DESCENT_FUNCTION_H

#include "UpdateFunction.h"

class GradientDescentParams : public UpdateFunctionParams
{
};

class GradientDescentFunction : public UpdateFunction
{
    float delta = 0.01;
public:
    void update(float& param, float der, UpdateFunctionParams* local_update_params)
    {
        param += der * delta; // static_cast<GradientDescentParams*>(local_update_params)->
    }
};

#endif // GRADIENT_DESCENT_FUNCTION_H
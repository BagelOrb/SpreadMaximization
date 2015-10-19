#ifndef UPDATE_FUNCTION_H
#define UPDATE_FUNCTION_H

class UpdateFunctionParams
{
    
};

class UpdateFunctionState 
{
public:
    virtual void update(float& param, float der, UpdateFunctionParams* params) = 0;
};

#endif // UPDATE_FUNCTION_H
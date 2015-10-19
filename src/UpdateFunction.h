#ifndef UPDATE_FUNCTION_H
#define UPDATE_FUNCTION_H

class UpdateFunctionParams
{
    
};

class UpdateFunctionState 
{
public:
    float der;
    void resetDer() { der = 0.0; }
    virtual void registerDer(float der_here)
    {
        der += der_here;
    }
    virtual void update(float& param, UpdateFunctionParams* params) = 0;
};

#endif // UPDATE_FUNCTION_H
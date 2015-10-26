#ifndef UPDATE_FUNCTION_H
#define UPDATE_FUNCTION_H

#include "../utils/Mat4Df.h"
#include "../utils/Mat4D.h"

class UpdateFunctionParams
{ // for example [velocity] in momentum based update functions
    
};

class UpdateFunction 
{
    
    virtual void update(float& param, float der, UpdateFunctionParams* local_update_params) = 0;
    
    
    template <class UpdatFunctionParams>
    void update(Mat4Df& params, Mat4Df& derivatives, Mat4D<UpdatFunctionParams>& local_params)
    {
        assert(params.getDims() == derivatives.getDims());
        assert(params.getDims() == local_params.getDims());
        for (Mat4Df::iterator it = params.begin(); it != params.end(); ++it)
        {
            update(*it, derivatives.get(it.getPos()), local_params.get(it.getPos()) );
        }
    }
};



#endif // UPDATE_FUNCTION_H
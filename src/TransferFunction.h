#ifndef TRANSFER_FUNCTION_H
#define TRANSFER_FUNCTION_H

#include <cmath>

class TransferFunction
{
public:
    virtual float apply(float in) = 0;
    virtual float der(float i, float o) = 0; //!< do/di which can be computed easily from either the input or the output


};

class SigmoidTransferFunction : public TransferFunction
{
public:
    float apply(float in)
    {
        return 1.0 / (1.0 + exp(-in));
    }
    
    float der(float in, float out)
    {
        return out * (1.0 - out);
    }
};

class TanhTransferFunction : public TransferFunction
{
public:
    float apply(float in)
    {
        return tanh(in);
    }
    float der(float in, float out)
    {
        return (1.0 - out) * (1.0 + out);
    }
};

#endif // TRANSFER_FUNCTION_H
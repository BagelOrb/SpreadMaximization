#ifndef UTILS_DIFFERENTIABLE_FUNCTION_H
#define UTILS_DIFFERENTIABLE_FUNCTION_H

class DifferentiableFunction
{
public:
    virtual float apply(float in) = 0;
    virtual float der(float in) = 0;
};

class Linear : public DifferentiableFunction
{
public:
    float apply(float in) { return in; }
    float der(float in) { return 1; }
};

class Abs : public DifferentiableFunction
{
public:
    float apply(float in) { return std::abs(in); }
    float der(float in) { return (in > 0)? 1 : -1; }
};

class Square : public DifferentiableFunction
{
public:
    float apply(float in) { return in * in; }
    float der(float in) { return 2.0 * in; }
};



#endif // UTILS_DIFFERENTIABLE_FUNCTION_H
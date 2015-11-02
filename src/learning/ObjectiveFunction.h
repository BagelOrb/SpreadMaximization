#ifndef LEARNING_OBJECTIVE_FUNCTION_H
#define LEARNING_OBJECTIVE_FUNCTION_H

#include "../Network.h"

class ObjectiveFunction
{
protected:
    virtual void setOutputDerivatives(Mat3Df& out, Mat3Df& out_ders, unsigned int true_class = 0) = 0;
public:
    virtual void setOutputDerivatives(Network& network, unsigned int true_class = 0)
    {
        LayerState& layer_state = network.network_state.layer_states.back();
        setOutputDerivatives(layer_state.output, layer_state.output_ders, true_class);
    }

};

class TestObjectiveFunction : public ObjectiveFunction
{
protected:
    void setOutputDerivatives(Mat3Df& out, Mat3Df& out_ders, unsigned int true_class = 0)
    {
        out_ders.clear();
        out_ders.set(Pos3(0,0,0), 1.0);
    }
};

/*!
 * An objective function which also acts as a softmax activation function.
 * No softmax layer has to be added to the layers!
 */
class SoftmaxCrossEntropy : public ObjectiveFunction 
{
protected:
    void setOutputDerivatives(Mat3Df& in, Mat3Df& out_ders, unsigned int true_class = 0)
    {
        Mat3Df out(in.getDims()); // output of the softmax layer
        float total_contribution = 0.0;
        for (Mat3Df::iterator in_it = in.begin(); in_it != in.end(); ++in_it)
        {
            float contribution = exp(*in_it);
            total_contribution += contribution;
            out.set(in_it.getPos(), contribution);
        }
        out.applyInPlace([&total_contribution](float in){ return in/total_contribution; });
        
        unsigned int this_class = 0;
        for (Mat3Df::iterator out_der_it = out_ders.begin(); out_der_it != out_ders.end(); ++out_der_it)
        {
            *out_der_it = - out.get(out_der_it.getPos()); // NOTE cross entropy is defined negatively
            if (this_class == true_class)
            {
                *out_der_it += 1; // NOTE cross entropy is defined negatively
            }
            this_class++;
        }
    }
};

#endif // LEARNING_OBJECTIVE_FUNCTION_H
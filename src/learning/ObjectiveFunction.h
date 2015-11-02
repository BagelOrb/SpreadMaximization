#ifndef LEARNING_OBJECTIVE_FUNCTION_H
#define LEARNING_OBJECTIVE_FUNCTION_H

#include "../Network.h"

class ObjectiveFunction
{
protected:
    virtual void setOutputDerivatives(Mat3Df& out, Mat3Df& out_ders) = 0;
public:
    virtual void setOutputDerivatives(Network& network)
    {
        LayerState& layer_state = network.network_state.layer_states.back();
        setOutputDerivatives(layer_state.output, layer_state.output_ders);
    }

};

class TestObjectiveFunction : public ObjectiveFunction
{
protected:
    void setOutputDerivatives(Mat3Df& out, Mat3Df& out_ders)
    {
        out_ders.clear();
        out_ders.set(Pos3(0,0,0), 1.0);
    }
};

#endif // LEARNING_OBJECTIVE_FUNCTION_H
#ifndef NETWORK_PROCESSOR_H
#define NETWORK_PROCESSOR_H

#include "Network.h"
#include "learning/Updater.h"
#include "learning/ObjectiveFunction.h"
#include "Settings.h"

class NetworkProcessor 
{
    ObjectiveFunction* objective_function;
    Updater* updater;
    
public:
    NetworkProcessor(ProcessorSettings settings)
    {
        switch (settings.objective_function_type)
        {
            case ObjectiveFunctionType::Test:
                objective_function = new TestObjectiveFunction();
                break;
        }
        switch (settings.updater_type)
        {
            case UpdaterType::GradientDescent:
                updater = new GradientDescentUpdater(settings.gradient_descent_delta);
                break;
        }
    }
    
    ~NetworkProcessor()
    {
        delete objective_function;
        delete updater;
    }
    
    /*!
     * Signal all layers, get the output derivatives from the objective function, 
     * propagate them back through all layers and update all parameters.
     */
    void process(Network& network, Mat3Df& input, Mat3Df* input_derivatives = nullptr)
    {
        network.network_state.initialize(network.layers, input);
        network.forward();
        objective_function->ObjectiveFunction::setOutputDerivatives(network);
        network.backward(input_derivatives);
        updater->update(network);
    }
    
    

};

#endif // NETWORK_PROCESSOR_H
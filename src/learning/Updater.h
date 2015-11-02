#ifndef LEARNING_UPDATER_H
#define LEARNING_UPDATER_H

#include "../Network.h"
#include "../layer/SubLayer.h"
#include "../utils/Pos.h"

/*!
 * Class for updating the parameters of the network
 */
class Updater
{
public:
    /*!
     * Meta parameters such as the current velocity for momentum
     */
    virtual void initializeMetaParams(Network& network) = 0;
    
    virtual void update(Network& network) = 0;
};

class GradientDescentUpdater : public Updater
{
    double delta;
    
public:
    GradientDescentUpdater(double delta)
    : delta(delta)
    {
    }
    
    void initializeMetaParams(Network& network)
    {
        // gradient descent has no meta parameters
    }
    
    void update(Network& network)
    {
        for (SubLayer* layer : network.layers)
        {
            for (LayerParams& params : layer->layer_params)
            {
                for (Pos4 p : params.dims)
                {
                    params.params.add(p, params.ders.get(p) * delta);
                }
            }
        }
    }
    
    
    
};

#endif // LEARNING_UPDATER_H
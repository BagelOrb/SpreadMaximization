#ifndef UTILS_DATASET_H
#define UTILS_DATASET_H

#include <fstream>

#include <iomanip> // std cerr precision TODO remove line

#include "NoCopy.h"
#include "Dims.h"
#include "Mat3Df.h"
#include "Mat4Df.h"

class Dataset 
{
protected:
    Mat4Df* data; //!< The images of the dataset
    Mat4D<char>* labels; //!< The labels corresponding to the data (if any). A Mat4D with dimensions 1x1x1xN

    
    Dataset()
    : data(nullptr)
    , labels(nullptr)
    {
    }


public:
    Mat3Df getImage(unsigned int idx)
    {
        return (*data)[idx];
    }
    char getLabel(unsigned int idx)
    {
        if (labels)
        {
            return labels->get(Pos4(0,0,0,idx));
        }
        else 
        {
            return 0;
        }
    }

    /*!
     * Get the size of the data set (number of images / labels)
     */
    unsigned int size()
    {
        return data->m;
    }



};




#endif // UTILS_DATASET_H
#ifndef LAYER_SIGNALDATA_H
#define LAYER_SIGNALDATA_H

#include <stdio.h>
#include <string>

#include "opencv2/imgproc/imgproc.hpp"


#include "basics.h"

using namespace cv;

class LayerSignalData 
{
    std::vector<Mat_<val>> convoluted;
    std::vector<Mat_<val>> signalled;
    
    /*!
     * Read an image from file and compute the output of the input layer, i.e. set LayerSignalData::signalled
     */
    LayerSignalData(std::string filename)
    {
        Mat img = imread( filename.c_str() );
        
    }
    
};

#endif // LAYER_SIGNALDATA_H
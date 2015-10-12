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
        for (int chan = 0; chan < 3; chan++)
        {
            signalled.emplace_back(img.rows, img.cols);
        }
        for (int x = 0; x < img.rows; x++)
        {
            for (int y = 0; y < img.cols; y++)
            {
                Vec3b colors = img.at<Vec3b>(x, y);
                for (int chan = 0; chan < 3; chan++)
                {
                    signalled.at(x, y) = val(colors.val[chan]) / 128.0 - 1.0; // ranging from -1 to 1
                }
            }
        }
    }
    
};

#endif // LAYER_SIGNALDATA_H
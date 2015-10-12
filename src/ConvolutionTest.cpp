#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Convolution.h" // TODO remove
#include "SignallingFunction.h" // TODO remove
#include "LayerSignalData.h" // TODO remove line


using namespace cv;

/** @function main */
int main ( int argc, char** argv )
{
    /// Declare variables
    Mat src, dst;

    Mat kernel;
    Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    char* window_name = "filter2D Demo";

    int c;

    /// Load an image
    src = imread( argv[1] );

    if( !src.data )
    { return -1; }

    /// Create window
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );

    /// Initialize arguments for the filter
    anchor = Point( -1, -1 );
    delta = 0;
    ddepth = -1; // output 

    std::cout << src.rows << ", " << src.cols << std::endl;
    
    /// Update kernel size for a normalized box filter
    kernel_size = 101;
//     kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    kernel = getGaussianKernel(kernel_size, 50);

    /// Apply filter
    std::vector<Mat> rgbChannels(3);
    std::vector<Mat> rgbChannels_result(3);
    split(src, rgbChannels);
    for (int col = 0; col < 3; col++)
    {
        filter2D(rgbChannels[col], rgbChannels_result[col], ddepth , kernel, anchor, delta, BORDER_CONSTANT);
        transpose(kernel, kernel);
    }
    rgbChannels_result[0] = rgbChannels[0];
    merge(rgbChannels_result, dst);
    
    
//     std::vector<Mat> rgbChannels_ff(3);
//     split(dst, rgbChannels_ff);
//     rgbChannels_ff[0] = 0;
//     merge(rgbChannels_ff, dst);
    
    std::cout << dst.rows << ", " << dst.cols << std::endl;
    imshow( window_name, dst );
    
    waitKey(0);

    return 0;
}
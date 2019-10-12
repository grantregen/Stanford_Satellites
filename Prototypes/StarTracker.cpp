//
//  StarTracker.cpp
//  Prototypes
//
//  Created by Moritz Stephan on 10/7/19.
//  Copyright © 2019 Moritz Stephan. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    Mat src, src_gray;
    Mat grad;
    String window_name = "Star Detector Prototype";
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    int THRESHOLD = 30;

    /// Load an image
    src = imread( argv[1] );

    if( !src.data )
    { return -1; }
    
    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

    /// Convert it to gray
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    /// Create window
    namedWindow( window_name, WINDOW_AUTOSIZE );

    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /// Gradient X
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
    
    Mat kernel;   // Use the default structuring element (kernel) for erode and dilate

    Mat minImage;
    // Perform min filtering on image using erode
    erode(grad, minImage, kernel);

    Mat maxImage;
    // Perform max filtering on image using dilate
    dilate(grad, maxImage, kernel);

    Mat diff = abs(maxImage - minImage);
    
    // Apply a threshold to only display extremes
    Mat processed;
    threshold( diff, processed, THRESHOLD, 255, 3 );
    
    imshow( window_name, processed );

    waitKey(0);

    return 0;
}


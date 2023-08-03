#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "Algorithm/MyFilter2D.h"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
    
    String imageName( "HappyFish.jpg" ); // by default
    if( argc > 1 )
    {
        imageName = argv[1];
    }

    Mat image;
    image = imread( samples::findFile( imageName ), IMREAD_GRAYSCALE ); // Read the file

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );

    Mat blurredImage;
    GaussianBlur( image, blurredImage, Size( 3, 3 ), BORDER_WRAP );

    Mat gx, gy;
    Sobel( blurredImage, gx, CV_32FC1, 1, 0 );
    Sobel( blurredImage, gy, CV_32FC1, 0, 1 );

    imshow( "gx", gx );
    imshow( "gy", gy );

    Mat phase;
    cv::phase( gx, gy, phase );

    imshow( "phase", phase );

    waitKey(0);
}
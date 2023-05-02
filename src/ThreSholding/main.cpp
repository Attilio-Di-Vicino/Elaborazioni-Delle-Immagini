/**
 * Filtri lineari per la sogliatura
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// g++ main.cpp $(pkg-config --cflags --libs opencv)
// ./a.out giacinto.jpg

#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    String imageName( "HappyFish.jpg" ); // by default
    if( argc > 1 )
    {
        imageName = argv[1];
    }

    Mat image;
    image = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Read the file

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey(0);

    // threshold
    Mat thresholdImage;
    double result = threshold( image, thresholdImage, 10, 100, 3 );
    cout << "Result threshold: " << result << endl;

    imshow( "thresholdImage", thresholdImage );
    waitKey(0);

    return 0;
}
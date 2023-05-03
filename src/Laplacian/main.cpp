#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "../Algorithm/MyFilter2D.h"


// g++ main.cpp $(pkg-config --cflags --libs opencv)
// ./a.out image.png

#include <iostream>
#define DIM 11

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

    // filtro laplaciano 90 gradi
    Mat myFilter90 = ( Mat_<float>( 3, 3 ) << 0,  1,  0,
                                              1, -4,  1,
                                              0,  1,  0 );
    
    // filtro laplaciano 45 gradi
    Mat myFilter45 = ( Mat_<float>( 3, 3 ) << 1,  1,  1,
                                              1, -8,  1,
                                              1,  1,  1 );

    // my laplaciano 90 gradi
    Mat myLaplaciano90;
    MyFilter2D::myFilter2D( image, myLaplaciano90, myFilter90 );

    imshow( "myLaplaciano90", myLaplaciano90 );
    waitKey(0);

    // my laplaciano 45 gradi
    Mat myLaplaciano45;
    MyFilter2D::myFilter2D( image, myLaplaciano45, myFilter45 );

    imshow( "myLaplaciano45", myLaplaciano45 );
    waitKey(0);

    // laplaciano 90 gradi
    Mat laplaciano90;
    Laplacian( image, laplaciano90, image.type(), 1 );

    imshow( "laplaciano90", laplaciano90 );
    waitKey(0);

    // laplaciano 45 gradi
    Mat laplaciano45;
    Laplacian( image, laplaciano45, image.type(), 3 );

    imshow( "laplaciano45", laplaciano45 );
    waitKey(0);

    return 0;
}
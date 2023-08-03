#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "Algorithm/MyFilter2D.h"

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
    image = imread( samples::findFile( imageName ), IMREAD_GRAYSCALE ); // Read the file

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey(0);

    // oggetto mat utilizzato come filtro
    Mat myFilter = ( Mat_<float>( 3, 3 ) << 1,  1,  1,
                                            1, -8,  1,
                                            1,  1,  1);

    // blur
    Mat blurDefault;
    blur( image, blurDefault, Size( 3, 3 ) );

    imshow( "blurDefault", blurDefault );
    waitKey(0);

    // ottengo la maschera
    Mat mask = abs( image - blurDefault );

    imshow( "mask", mask );
    waitKey(0);

    // aggiungo la maschera
    Mat unSharpMask = abs( image + mask );

    imshow( "unSharpMask", unSharpMask );
    waitKey(0);


    return 0;
}
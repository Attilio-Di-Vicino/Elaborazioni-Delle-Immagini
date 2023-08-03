/**
 * Filtri lineari per lo smoothing ovvero filtri media utilizzati per la sfocatura
 * o rimozione del rumore su di un'immagine
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

    // blur
    Mat blurDefault;
    blur( image, blurDefault, Size( 11, 11 ) );

    imshow( "blurDefault", blurDefault );
    waitKey(0);

    // Gaussian blur
    Mat gaussianBlurDefault;
    GaussianBlur( image, gaussianBlurDefault, Size( 11, 11 ), 0, 0 );

    imshow( "gaussianBlurDefault", gaussianBlurDefault );
    waitKey(0);

    // box filter
    Mat boxFilterDefault;
    boxFilter( image, boxFilterDefault, image.type(), Size( 11, 11 ) );

    imshow( "boxFilterDefault", boxFilterDefault );
    waitKey(0);

    return 0;
}
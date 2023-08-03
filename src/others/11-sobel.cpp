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

    // Rimuovi il rumore sfocando con un filtro gaussiano (dimensione del kernel = 3)
    GaussianBlur( image, image, Size (3, 3), 0, 0, BORDER_DEFAULT );

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey(0);

    // filtro sobel gradiente x
    Mat myFilterGX = ( Mat_<float>( 3, 3 ) << -1,  -2,  -1,
                                               0,   0,   0,
                                               1,   2,   1 );
    
    // filtro sobel gradiente y
    Mat myFilterGY = ( Mat_<float>( 3, 3 ) << -1,  0,  1,
                                              -2,  0,  2,
                                              -1,  0,  1 );

    // filtro laplaciano 45 gradi
    Mat myFilter45 = ( Mat_<float>( 3, 3 ) << 1,  1,  1,
                                              1, -8,  1,
                                              1,  1,  1 );

    // my sobel 90 gradiente x
    Mat mySobelGX;
    MyFilter2D::myFilter2D( image, mySobelGX, myFilterGX );

    // my sobel 90 gradiente y
    Mat mySobelGY;
    MyFilter2D::myFilter2D( image, mySobelGY, myFilterGY );

    Mat myAbsSobelX, myAbsSobelY, mySobel;

    // riconversione in CV_8U
    convertScaleAbs( mySobelGX, myAbsSobelX );
    convertScaleAbs( mySobelGY, myAbsSobelY );

    // Cerchiamo di approssimare il gradiente sommando entrambi i gradienti direzionali 
    // (nota che questo non è affatto un calcolo esatto! ma va bene per i nostri scopi).
    addWeighted( myAbsSobelX, 0.5, myAbsSobelY, 0.5, 0, mySobel );

    // Mat mySobel = abs( mySobelGX + mySobelGY );

    imshow( "mySobel", mySobel );
    waitKey(0);

    // sobel x
    Mat sobelX;
    Sobel( image, sobelX, image.type(), 1, 0 );

    // imshow( "sobelX", sobelX );
    // waitKey(0);

    // sobel y
    Mat sobelY;
    Sobel( image, sobelY, image.type(), 0, 1 );

    // imshow( "sobelY", sobelY );
    // waitKey(0);

    Mat absSobelX, absSobelY, sobel;

    // riconversione in CV_8U
    convertScaleAbs( sobelX, absSobelX );
    convertScaleAbs( sobelY, absSobelY );

    // Cerchiamo di approssimare il gradiente sommando entrambi i gradienti direzionali 
    // (nota che questo non è affatto un calcolo esatto! ma va bene per i nostri scopi).
    addWeighted( absSobelX, 0.5, absSobelY, 0.5, 0, sobel );

    imshow( "sobel", sobel );
    waitKey(0);

    // effettuo laplaciano 45 gradi su riposta sobel
    // my laplaciano 45 gradi
    Mat myLaplaciano45;
    MyFilter2D::myFilter2D( sobel, myLaplaciano45, myFilter45 );

    imshow( "myLaplaciano45", myLaplaciano45 );
    waitKey(0);

    Mat sharpening = abs( image - myLaplaciano45 );
    imshow( "sharpening", sharpening );
    waitKey(0);

    return 0;
}